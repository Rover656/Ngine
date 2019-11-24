/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Renderer.h"

#include "OpenGLDefines.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
    void Renderer::_enableGLCapabilities() {
        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::_createDefaultShader(){
        // Vertex source
        std::string vertexShaderSrc =
#if defined(GRAPHICS_OPENGL21)
                "#version 120\n"
#elif defined(GRAPHICS_OPENGLES2)
                "#version 100\n"
#endif
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21)
                "attribute vec3 NG_VertexPos;\n"
                "attribute vec2 NG_VertexTexCoord;\n"
                "attribute vec4 NG_VertexColor;\n"
                "varying vec2 fragTexCoord;\n"
                "varying vec4 fragColor;\n"
#elif defined(GRAPHICS_OPENGL33)
                "#version 330\n"
                "in vec3 NG_VertexPos;\n"
                "in vec2 NG_VertexTexCoord;\n"
                "in vec4 NG_VertexColor;\n"
                "out vec2 fragTexCoord;\n"
                "out vec4 fragColor;\n"
#endif
                "uniform mat4 NGU_MATRIX_MVP;\n"
                "void main()\n"
                "{\n"
                "    fragTexCoord = NG_VertexTexCoord;\n"
                "    fragColor = NG_VertexColor;\n"
                "    gl_Position = NGU_MATRIX_MVP*vec4(NG_VertexPos, 1.0);\n"
                "}\n";

        // Fragment source
        std::string fragmentShaderSrc =
#if defined(GRAPHICS_OPENGL21)
                "#version 120\n"
#elif defined(GRAPHICS_OPENGLES2)
                "#version 100\n"
                "precision mediump float;\n"
#endif
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21)
                "varying vec2 fragTexCoord;\n"
                "varying vec4 fragColor;\n"
#else
                "#version 330\n"
                "in vec2 fragTexCoord;\n"
                "in vec4 fragColor;\n"
                "out vec4 finalColor;\n"
#endif
                "uniform sampler2D NGU_TEXTURE;\n"
                "void main()\n"
                "{\n"
                "    vec4 texelColor = texture2D(NGU_TEXTURE, fragTexCoord);\n"
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21)
                "    gl_FragColor = texelColor*fragColor;\n"
#elif defined(GRAPHICS_OPENGL33)
                "    finalColor = texelColor*fragColor;\n"
#endif
                "}\n";

        // Create shader
        auto shader = new Shader(vertexShaderSrc, fragmentShaderSrc);
        auto compiled = shader->Compile();

        if (!compiled) {
            delete shader;
            throw std::runtime_error("Default shader failed to compile!");
        }

        // Create shader program
        m_defaultShaderProgram = new ShaderProgram(shader);
        auto linked = m_defaultShaderProgram->Link();
        delete shader;

        if (!linked) {
            delete m_defaultShaderProgram;
            m_defaultShaderProgram = nullptr;
            throw std::runtime_error("Default shader program failed to link!");
        }

        // Create default texture (for shader)
        unsigned char pixels[4] = {255, 255, 255, 255};
        m_whiteTexture = new Texture2D(m_graphicsDevice, pixels, 1, 1);
    }

    void Renderer::_createQuadBuffers() {
        // Create VAO (if supported)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            glGenVertexArrays(1, &m_quadVAO);
            glBindVertexArray(m_quadVAO);
        }

        // Enable vertex attrib arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // Create quad vertex buffer
        glGenBuffers(1, &m_quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);

        // Write null data
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * VBO_SIZE, nullptr, GL_STREAM_DRAW);

        // Configure vertex attrib arrays
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Position));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, TexCoords));

        // Create quad index buffer
        glGenBuffers(1, &m_quadIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);

        // Default index data
        GLuint indices[QUAD_IBO_SIZE];
        for (auto i = 0; i < VBO_SIZE / 4; i++) {
            indices[i * 6 + 0] = i * 4 + 0;
            indices[i * 6 + 1] = i * 4 + 1;
            indices[i * 6 + 2] = i * 4 + 3;
            indices[i * 6 + 4] = i * 4 + 2;
            indices[i * 6 + 3] = i * 4 + 1;
            indices[i * 6 + 5] = i * 4 + 3;
        }

        // Send default data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * QUAD_IBO_SIZE, indices, GL_STATIC_DRAW);

        // Unbind VAO (if enabled)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            glBindVertexArray(0);
        }

        // Unbind buffers
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Renderer::_deleteQuadBuffers() {
        // Unbind VAO (if enabled)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO))
            glBindVertexArray(0);

        // Unbind buffers
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Delete VAO
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO))
            glDeleteVertexArrays(1, &m_quadVAO);

        // Delete VBO
        glDeleteBuffers(1, &m_quadVBO);

        // Delete IBO
        glDeleteBuffers(1, &m_quadIBO);

        // Set all to 0
        m_quadVAO = 0;
        m_quadVBO = 0;
        m_quadIBO = 0;
    }

    void Renderer::_updateQuadBuffers() {
        // Bind buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);

        // Miscellaneous ways of sending data to the buffer.
#if defined(glMapBuffer)
        // Orphan data
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * VBO_SIZE, nullptr, GL_STREAM_DRAW);

        // Replace the vertices that have changed.
        // This will not update any more vertices than necessary.
        void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(buf, m_quadVertices, sizeof(VertexData) * m_quadVerticesCount);
        glUnmapBuffer(GL_ARRAY_BUFFER);
#elif defined(glBufferSubData)
        // Copy the required data into the vertex array.
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_quadVerticesCount, m_quadVertices);
#else
        // Copy all of the data into the vertex array.
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * VBO_SIZE, m_quadVertices, GL_STREAM_DRAW);
#endif

        // Unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Renderer::_bindQuadBuffers() {
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            // Bind VAO.
            glBindVertexArray(m_quadVAO);
        } else {
            // Enable vertex attrib arrays
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            // Bind quad VBO
            glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);

            // Configure vertex attrib arrays
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Position));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Color));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, TexCoords));
        }

        // Bind IBO.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);
    }

    void Renderer::_unbindQuadBuffers() {
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            // Unbind VAO.
            glBindVertexArray(0);
        } else {
            // Unbind VBO.
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        // Unbind IBO.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Renderer::_renderQuadBuffers() {
        // Upload vertex data
        _updateQuadBuffers();

        // Bind buffers
        _bindQuadBuffers();

        // Get projection matrix
        auto projection = m_graphicsDevice->GetProjectionMatrix();

        // Loop variables
        ShaderProgram *curProg = nullptr;
        ShaderProgram *actualCurProg = nullptr;
        Texture2D *curTex = nullptr;
        Texture2D *actualCurTex = nullptr;
        int fromIndex = 0;

        for (const auto &quad : m_quads) {
            // Bind a shader program if required
            if (actualCurProg != quad.Obj->GetShaderProgram() || curProg == nullptr) {
                actualCurProg = quad.Obj->GetShaderProgram();
                if (actualCurProg != nullptr)
                    curProg = actualCurProg;
                else curProg = m_defaultShaderProgram;

                // Use program
                glUseProgram(curProg->ID);

                // Set the MVP matrix. It is just the projection as model view is already applied in vertices
                glUniformMatrix4fv(curProg->GetUniformLocation("NGU_MATRIX_MVP"), 1, GL_FALSE, projection.ToFloatArray().data());

                // Set the texture sampler
                glUniform1i(curProg->GetUniformLocation("NGU_TEXTURE"), 0);
            }

            // Bind a texture if required
            if (actualCurTex != quad.Obj->GetTexture() || curTex == nullptr) {
                actualCurTex = quad.Obj->GetTexture();
                if (actualCurTex != nullptr)
                    curTex = actualCurTex;
                else curTex = m_whiteTexture;

                // Bind texture
                glBindTexture(GL_TEXTURE_2D, curTex->_ID);
            }

            // Draw quads
            glDrawElements(GL_TRIANGLES, quad.Count * 6, GL_UNSIGNED_INT, (GLvoid*) fromIndex);

            // Increase the from index
            fromIndex += quad.Count * 6;
        }

        // Unbind buffers
        _unbindQuadBuffers();
    }

    void Renderer::_sortBucket(Renderer::RenderBucket bucket_) {
        std::sort(m_renderQueue[bucket_].begin(), m_renderQueue[bucket_].end(), _sortPredicate);
    }

    bool Renderer::_sortPredicate(Renderable *a_, Renderable *b_) {
        return a_->GetZIndex() < b_->GetZIndex();
    }

    void Renderer::_processQueue() {
        // Sort the Z negative and positive buckets.
        _sortBucket(BUCKET_Z_NEGATIVE);
        _sortBucket(BUCKET_Z_POSITIVE);

        // Render the buckets.
        _processBucket(BUCKET_Z_NEGATIVE);
        _processBucket(BUCKET_Z_NEUTRAL);
        _processBucket(BUCKET_Z_POSITIVE);
    }

    void Renderer::_processBucket(Renderer::RenderBucket bucket_) {
        // Check if the queue has something to render
        if (!m_renderQueue[bucket_].empty()) {
            // Render each renderable
            for (const auto &renderable : m_renderQueue[bucket_]) {
                _processRenderable(renderable);
            }

            // Push to framebuffer.
            _flush();
        }
    }

    void Renderer::_processRenderable(Renderable *renderable_) {
        switch (renderable_->GetType()) {
            case Renderable::OBJECT_QUAD: {
                // Get quad
                auto quad = (QuadRenderable *) renderable_;

                // Check we have space
                if (m_quadVerticesCount + quad->GetQuadCount() * 4 > VBO_SIZE) {
                    if (quad->GetQuadCount() * 4 > VBO_SIZE) throw std::runtime_error("The quad contains too many vertices.");
                    _flush();
                }

                // Add vertices
                auto verts = quad->GetVertices();
                auto mview = quad->GetModelView();
                for (auto i = 0; i < quad->GetQuadCount() * 4; i++) {
                    m_quadVertices[m_quadVerticesCount + i] = verts[i];
                    m_quadVertices[m_quadVerticesCount + i].Position = verts[i].Position.Transform(mview);
                }
                m_quadVerticesCount += quad->GetQuadCount() * 4;

                // Look for similar batch.
                if (!m_quads.empty()) {
                    auto last = m_quads.size() - 1;
                    auto back = m_quads.back();

                    if (back.Obj->GetZIndex() == quad->GetZIndex()
                        && back.Obj->GetShaderProgram() == quad->GetShaderProgram()
                        && back.Obj->GetTexture() == quad->GetTexture()) {
                        m_quads[last].Count += quad->GetQuadCount();
                        break;
                    }
                }

                // Add to batch.
                m_quads.push_back({quad->GetQuadCount(), quad});
            } break;
        }
    }

    void Renderer::_flush() {
        // Render the quad buffers.
        _renderQuadBuffers();

        // Clear the queues for the next render.
        m_renderQueue[BUCKET_Z_NEGATIVE].clear();
        m_renderQueue[BUCKET_Z_NEUTRAL].clear();
        m_renderQueue[BUCKET_Z_POSITIVE].clear();
        m_quadVerticesCount = 0;
        m_quads.clear();
    }

    Renderer::Renderer(GraphicsDevice *graphicsDevice_)
            : m_graphicsDevice(graphicsDevice_) {
        // Check graphics device.
        if (m_graphicsDevice == nullptr)
            throw std::runtime_error("Graphics device cannot be null!");

        // Create default shader.
        _createDefaultShader();

        // Create quad buffers.
        _createQuadBuffers();
    }

    Renderer::~Renderer() {
        // Delete quad buffers.
        _deleteQuadBuffers();

        // Delete resources.
        delete m_whiteTexture;
        delete m_defaultShaderProgram;
    }

    void Renderer::Add(Renderable *obj_) {
        // Add to the queue.
        if (obj_->GetZIndex() < 0) m_renderQueue[BUCKET_Z_NEGATIVE].push_back(obj_);
        else if (obj_->GetZIndex() > 0) m_renderQueue[BUCKET_Z_POSITIVE].push_back(obj_);
        else m_renderQueue[BUCKET_Z_NEUTRAL].push_back(obj_);
    }

    void Renderer::Render() {
        // Enable OpenGL Capabilities
        _enableGLCapabilities();

        // Process the queue.
        _processQueue();
    }

    void Renderer::Clear() {
        // Clear color and depth buffers.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::SetClearColor(Color color_) {
        // Set the clear color.
        glClearColor(color_.R, color_.G, color_.B, color_.A);
    }
}