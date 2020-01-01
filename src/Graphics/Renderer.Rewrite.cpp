/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#include "Renderer.hpp"

#ifdef USE_EXPERIMENTAL_RENDERER

#include "../Logger.hpp"
#include "Rendering/QuadRenderable.hpp"
#include "OpenGL.hpp"

namespace NerdThings::Ngine::Graphics {
    void Renderer::_enableGLCapabilities() {
        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::_createDefaultShader() {
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
        Logger::Notice("Renderer", "Creating internal shader.");
        auto shader = new Shader(vertexShaderSrc, fragmentShaderSrc);
        auto compiled = shader->Compile();

        if (!compiled) {
            delete shader;
            Logger::Fail("Renderer", "Failed to compile internal shader!");
            throw std::runtime_error("Default shader failed to compile!");
        }

        // Create shader program
        m_defaultShaderProgram = new ShaderProgram(shader);
        auto linked = m_defaultShaderProgram->Link();
        delete shader;

        if (!linked) {
            delete m_defaultShaderProgram;
            m_defaultShaderProgram = nullptr;
            Logger::Fail("Renderer", "Failed to link internal shader program!");
            throw std::runtime_error("Default shader program failed to link!");
        }
        Logger::Notice("Renderer", "Created internal shader.");

        // Create default texture (for shader)
        unsigned char pixels[4] = {255, 255, 255, 255};
        m_whiteTexture = new Texture2D(m_graphicsDevice, pixels, 1, 1);
        Logger::Notice("Renderer", "Created blank 1x1 texture.");
    }

    void Renderer::_createBuffers() {
        // TODO: Look at cleaning up common code?
        // Create triangle VAO (if supported)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            glGenVertexArrays(1, &m_triangleVAO);
            glBindVertexArray(m_triangleVAO);
            Logger::Notice("Renderer", "Created triangle VAO with ID %i.", m_triangleVAO);
        }

        // Enable vertex attrib arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // Create triangle vertex buffer
        glGenBuffers(1, &m_triangleVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);
        Logger::Notice("Renderer", "Created triangle VBO with ID %i.", m_triangleVBO);

        // Write null data
        glBufferData(GL_ARRAY_BUFFER, sizeof(Rendering::VertexData) * VBO_SIZE, nullptr, GL_STREAM_DRAW);

        // Configure vertex attrib arrays
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, Position));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, Color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, TexCoords));

        // Unbind VAO (if enabled)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO))
            glBindVertexArray(0);

        // Unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Create quad VAO (if supported)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            glGenVertexArrays(1, &m_quadVAO);
            glBindVertexArray(m_quadVAO);
            Logger::Notice("Renderer", "Created quad VAO with ID %i.", m_quadVAO);
        }

        // Enable vertex attrib arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // Create quad vertex buffer
        glGenBuffers(1, &m_quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
        Logger::Notice("Renderer", "Created quad VBO with ID %i.", m_quadVBO);

        // Write null data
        glBufferData(GL_ARRAY_BUFFER, sizeof(Rendering::VertexData) * VBO_SIZE, nullptr, GL_STREAM_DRAW);

        // Configure vertex attrib arrays
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, Position));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, Color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, TexCoords));

        // Create quad index buffer
        glGenBuffers(1, &m_quadIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);

        // Default index data
        GLuint indices[QUAD_IBO_SIZE];
        for (auto i = 0; i < VBO_SIZE / 4; i++) {
            indices[i * 6 + 0] = i * 4 + 0;
            indices[i * 6 + 1] = i * 4 + 1;
            indices[i * 6 + 2] = i * 4 + 3;
            indices[i * 6 + 3] = i * 4 + 1;
            indices[i * 6 + 4] = i * 4 + 2;
            indices[i * 6 + 5] = i * 4 + 3;
        }

        // Send default data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * QUAD_IBO_SIZE, indices, GL_STATIC_DRAW);

        // Unbind VAO (if enabled)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO))
            glBindVertexArray(0);

        // Unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Renderer::_deleteBuffers() {
        // Unbind VAO (if enabled)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO))
            glBindVertexArray(0);

        // Unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Delete VAOs
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            glDeleteVertexArrays(1, &m_triangleVAO);
            glDeleteVertexArrays(1, &m_quadVAO);
        }

        // Delete triangle VBO
        glDeleteBuffers(1, &m_triangleVBO);

        // Delete quad VBO and IBO
        glDeleteBuffers(1, &m_quadVBO);
        glDeleteBuffers(1, &m_quadIBO);

        // Set all to 0
        m_triangleVAO = 0;
        m_triangleVBO = 0;
        m_quadVAO = 0;
        m_quadVBO = 0;
        m_quadIBO = 0;
    }

    void Renderer::_bindTriangleBuffers() {
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            // Bind VAO.
            glBindVertexArray(m_triangleVAO);
        } else {
            // Enable vertex attrib arrays
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            // Bind triangle VBO
            glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);

            // Configure vertex attrib arrays
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, Position));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, Color));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, TexCoords));
        }
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

            // Bind quad VBO and IBO
            glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);

            // Configure vertex attrib arrays
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, Position));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, Color));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Rendering::VertexData), (GLvoid*) offsetof(Rendering::VertexData, TexCoords));
        }
    }

    void Renderer::_writeBuffer(unsigned int vbo_) {
        // Bind buffer
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);

        // Send data to buffer
#if defined(glMapBuffer)
        // Orphan data
        glBufferData(GL_ARRAY_BUFFER, sizeof(Rendering::VertexData) * VBO_SIZE, nullptr, GL_STREAM_DRAW);

        // Replace changed vertex data
        void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(buf, m_vertices, sizeof(Rendering::VertexData) * m_vertexCount);
        glUnmapBuffer(GL_ARRAY_BUFFER);
#elif defined(glBufferSubData)
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexCount, m_vertices);
#else
        glBufferData(GL_ARRAY_BUFFER, sizeof(Rendering::VertexData) * VBO_SIZE, m_vertices, GL_STREAM_DRAW);
#endif
    }

    void Renderer::_unbindBuffers() {
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

    bool Renderer::_sortPredicate(const BatchItem &a_, const BatchItem &b_) {
        return a_.Depth < b_.Depth;
    }

    void Renderer::_render(RenderTarget *target_) {
        // Start by sorting the batch
        std::sort(m_batches[target_][Z_NEG].begin(), m_batches[target_][Z_NEG].end(), &Renderer::_sortPredicate);
        std::sort(m_batches[target_][Z_POS].begin(), m_batches[target_][Z_POS].end(), &Renderer::_sortPredicate);

        // RenderBatched the batches
        _renderBucket(target_, Z_NEG);
        _renderBucket(target_, Z_NEU);
        _renderBucket(target_, Z_POS);
    }

    void Renderer::_renderBucket(RenderTarget *target_, Renderer::BatchBucket bucket_) {
        if (!m_batches[target_][bucket_].empty()) {
            // RenderBatched each renderable
            for (const auto &item : m_batches[target_][bucket_]) {
                _processBatchItem(item);
            }

            // RenderBatched
            _flush();
        }
    }

    void Renderer::_processBatchItem(const Renderer::BatchItem &item_) {
        // Check if we need to flush due to lack of space or change of primitive type.
        if (m_vertexCount + item_.Vertices.size() > VBO_SIZE || (m_currentPrimitiveType != item_.PrimitiveType && m_vertexCount > 0)) {
            if (item_.Vertices.size() > VBO_SIZE)
                throw std::runtime_error("Batch item vertex count exceeds maximum!");
            _flush();
        }

        // Set current primitive type
        m_currentPrimitiveType = item_.PrimitiveType;

        // Push vertices to batch.
        for (auto i = 0; i < item_.VertexCount; i++)
            m_vertices[m_vertexCount + i] = item_.Vertices[i];

        // Increase vertex count
        m_vertexCount += item_.VertexCount;

        // Add to the processed items list
        if (!m_processedItems.empty()) {
            auto last = m_processedItems.size() - 1;
            auto back = m_processedItems.back();

            // Vertices are ignored here because they're now in a shared pool.
            // Depth is ignored because we are already in depth order.
            if (back.Shader == item_.Shader
                && back.Texture == item_.Texture
                && back.PrimitiveType == item_.PrimitiveType) {
                // Add to existing item.
                m_processedItems[last].VertexCount += item_.VertexCount;
                return;
            }
        }
        m_processedItems.push_back(item_);
    }

    void Renderer::_flush() {
        if (m_vertexCount == 0) return;

        // Get projection.
        auto projection = m_graphicsDevice->GetProjectionMatrix();

        if (m_currentPrimitiveType == PRIMITIVE_TRIANGLES) {
            // Upload triangle data
            _writeBuffer(m_triangleVBO);

            // Bind the buffers
            _bindTriangleBuffers();
        } else if (m_currentPrimitiveType == PRIMITIVE_QUADS) {
            // Upload quad data
            _writeBuffer(m_quadVBO);

            // Bind the buffers
            _bindQuadBuffers();
        } else {
            Logger::Fail("Renderer", "Unknown primitive type is being used in flush method!");
            throw std::runtime_error("Unknown primitive type is being used in flush method!");
        }

        // Loop over the batches to be drawn
        ShaderProgram *curProg = nullptr;
        ShaderProgram *actualCurProg = nullptr;
        Texture2D *curTex = nullptr;
        Texture2D *actualCurTex = nullptr;
        int fromIndex = 0;

        for (const auto &item : m_processedItems) {
            if (actualCurProg != item.Shader || curProg == nullptr) {
                actualCurProg = item.Shader;
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
            if (actualCurTex != item.Texture || curTex == nullptr) {
                actualCurTex = item.Texture;
                if (actualCurTex != nullptr)
                    curTex = actualCurTex;
                else curTex = m_whiteTexture;

                // Bind texture
                glBindTexture(GL_TEXTURE_2D, curTex->m_ID);
            }

            // Draw
            if (m_currentPrimitiveType == PRIMITIVE_TRIANGLES) {
                // Draw triangles
                glDrawArrays(GL_TRIANGLES, fromIndex, item.VertexCount);
            } else if (m_currentPrimitiveType == PRIMITIVE_QUADS) {
                // Draw indexed quads/triangles
                glDrawElements(GL_TRIANGLES, item.VertexCount / 4 * 6, GL_UNSIGNED_INT, (GLvoid*) fromIndex);
            }

            // Increase the from index
            fromIndex += item.VertexCount;
        }

        // Unbind buffers
        _unbindBuffers();

        // Clear data
        m_vertexCount = 0;
        m_processedItems.clear();
    }

    Renderer::Renderer(GraphicsDevice *graphicsDevice_)
            : m_graphicsDevice(graphicsDevice_) {
        Logger::Notice("Renderer", "Creating renderer...");

        // Check graphics device.
        if (m_graphicsDevice == nullptr)
            throw std::runtime_error("Graphics device cannot be null!");

        // Create default shader.
        _createDefaultShader();

        // Create buffers for rendering.
        _createBuffers();

        Logger::Notice("Renderer", "Finished creating renderer.");
    }

    Renderer::~Renderer() {
        // Delete buffers
        _deleteBuffers();

        // Delete resources.
        delete m_whiteTexture;
        delete m_defaultShaderProgram;
    }

    void Renderer::Begin(PrimitiveType primitiveType_, Texture2D *texture_, int depth_, ShaderProgram *shader_) {
        // Stop rendering.
        if (m_rendering)
            throw std::runtime_error("Cannot start a new batch while rendering.");

        // Check we aren't midway
        if (m_midBatch)
            throw std::runtime_error("Cannot begin a second batch before finishing the last one.");

        // Get the current target
        auto curTarget = m_graphicsDevice->GetCurrentTarget();

        // We are now running a batch
        m_midBatch = true;
        m_currentBatchTarget = curTarget;

        // Ensure the batch exists for this target
        if (m_batches.find(curTarget) == m_batches.end()) {
            m_batches.insert({curTarget, {{}, {}, {}}});
        }

        // Create the new batch item.
        m_currentBatch = BatchItem();
        m_currentBatch.PrimitiveType = primitiveType_;
        m_currentBatch.Texture = texture_;
        m_currentBatch.Depth = depth_;
        m_currentBatch.Shader = shader_;
    }

    void Renderer::Vertex(Vector2 pos_, Vector2 texCoord_, Color color_) {
        if (!m_midBatch)
            throw std::runtime_error("Start a batch before adding a vertex.");

        // Build vertex data
        Rendering::VertexData vDat;
        vDat.Position = Vector3(pos_.X, pos_.Y, 0);
        vDat.TexCoords = texCoord_;
        vDat.Color = color_;

        // ModelView transform
        vDat.Position = vDat.Position.Transform(m_graphicsDevice->GetModelViewMatrix());

        // Add to batch
        m_currentBatch.Vertices.push_back(vDat);
    }

    void Renderer::End() {
        if (!m_midBatch)
            throw std::runtime_error("Start a batch before finishing it.");

        // Save vertex count
        m_currentBatch.VertexCount = m_currentBatch.Vertices.size();

        if (m_currentBatch.Depth < 0) {
            m_batches[m_currentBatchTarget][Z_NEG].push_back(m_currentBatch);
        } else if (m_currentBatch.Depth > 0) {
            m_batches[m_currentBatchTarget][Z_POS].push_back(m_currentBatch);
        } else m_batches[m_currentBatchTarget][Z_NEU].push_back(m_currentBatch);

        // No longer batching.
        m_midBatch = false;
    }

    void Renderer::RenderBatched() {
        // Check we aren't already rendering
        if (m_rendering)
            throw std::runtime_error("Another thread is already rendering. Renderer should only be used on the main thread anyway.");

        // Make sure we're not mid batch.
        if (m_midBatch)
            throw std::runtime_error("Must end the current batch before rendering.");

        // Rendering now!
        m_rendering = true;

        // Configure the current framebuffer for rendering
        m_graphicsDevice->SetupFramebuffer();

        // Enable OpenGL Capabilities
        _enableGLCapabilities();

        // Get the current framebuffer
        auto curFramebuffer = m_graphicsDevice->GetCurrentTarget();

        // RenderBatched the batch for this framebuffer
        _render(curFramebuffer);

        // Clear the batch
        m_batches[curFramebuffer][Z_NEG].clear();
        m_batches[curFramebuffer][Z_POS].clear();
        m_batches[curFramebuffer][Z_NEU].clear();

        // We're done
        m_rendering = false;
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

#endif
