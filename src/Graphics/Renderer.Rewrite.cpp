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
        // Create VAO (if supported)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            glGenVertexArrays(1, &m_VAO);
            glBindVertexArray(m_VAO);
            Logger::Notice("Renderer", "Created VAO with ID %i.", m_VAO);
        }

        // Enable vertex attrib arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // Create vertex buffer
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        Logger::Notice("Renderer", "Created VBO with ID %i.", m_VBO);

        // Write null data
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * VBO_SIZE, nullptr, GL_STREAM_DRAW);

        // Configure vertex attrib arrays
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Position));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, TexCoords));

        // Unbind VAO (if enabled)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO))
            glBindVertexArray(0);

        // Unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Renderer::_deleteBuffers() {
        // Unbind VAO (if enabled)
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO))
            glBindVertexArray(0);

        // Unbind buffers
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Delete VAOs
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            Logger::Notice("Renderer", "Deleting VAO.");
            glDeleteVertexArrays(1, &m_VAO);
        }

        // Delete VBO
        Logger::Notice("Renderer", "Deleting VBO.");
        glDeleteBuffers(1, &m_VBO);

        // Set all to 0
        m_VAO = 0;
        m_VBO = 0;
    }

    void Renderer::_bindBuffers() {
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            // Bind VAO.
            glBindVertexArray(m_VAO);
        } else {
            // Enable vertex attrib arrays
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            // Bind triangle VBO
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

            // Configure vertex attrib arrays
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Position));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Color));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, TexCoords));
        }
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

    void Renderer::_writeBuffer() {
        // Bind buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        // Send data to buffer
#if defined(glMapBuffer)
        // Orphan data
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * VBO_SIZE, nullptr, GL_STREAM_DRAW);

        // Replace changed vertex data
        void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(buf, m_vertices, sizeof(VertexData) * m_vertexCount);
        glUnmapBuffer(GL_ARRAY_BUFFER);
#elif defined(glBufferSubData)
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexCount, m_vertices);
#else
        glBufferData(GL_ARRAY_BUFFER, sizeof(Rendering::VertexData) * VBO_SIZE, m_vertices, GL_STREAM_DRAW);
#endif
    }

    void Renderer::_addVertices(Renderer::PrimitiveType type_, VertexData *vertices_, int count_) {
        // Check vertex array size
        if (count_ > VBO_SIZE)
            Logger::Fail("Renderer", "Vertex array is too big!");

        // Add vertices to vertex array
        if (type_ == PRIMITIVE_QUADS) {
            // Convert from quads to triangles
            for (auto i = 0; i < count_ / 4; i++) {
                m_vertices[m_vertexCount + i] = vertices_[i];

                m_vertices[m_vertexCount + i * 6 + 0] = vertices_[i * 4 + 0];
                m_vertices[m_vertexCount + i * 6 + 1] = vertices_[i * 4 + 1];
                m_vertices[m_vertexCount + i * 6 + 2] = vertices_[i * 4 + 3];
                m_vertices[m_vertexCount + i * 6 + 3] = vertices_[i * 4 + 1];
                m_vertices[m_vertexCount + i * 6 + 4] = vertices_[i * 4 + 2];
                m_vertices[m_vertexCount + i * 6 + 5] = vertices_[i * 4 + 3];
            }

            // Increase count
            m_vertexCount += count_ / 4 * 6;
        } else {
            for (auto i = 0; i < count_; i++) {
                m_vertices[m_vertexCount + i] = vertices_[i];
            }

            // Increase count
            m_vertexCount += count_;
        }

        // Draw if at max vertex count
        if (m_vertexCount == VBO_SIZE) {
            Render();
        }
    }

    void Renderer::_renderBatch() {
        // Dont bother if there's nothing
        if (m_vertexCount == 0) return;

        // Get projection.
        auto MVP = m_graphicsDevice->GetModelViewMatrix() * m_graphicsDevice->GetProjectionMatrix();

        // Upload data
        _writeBuffer();

        // Bind the buffers
        _bindBuffers();

        // Get shader and texture
        auto shader = m_currentShader != nullptr ? m_currentShader : m_defaultShaderProgram;
        auto tex = m_currentTexture != nullptr ? m_currentTexture : m_whiteTexture;

        // Use program
        glUseProgram(shader->ID);

        // Set the MVP matrix. It is just the projection as model view is already applied in vertices
        glUniformMatrix4fv(shader->GetUniformLocation("NGU_MATRIX_MVP"), 1, GL_FALSE, MVP.ToFloatArray().data());

        // Set the texture sampler
        glUniform1i(shader->GetUniformLocation("NGU_TEXTURE"), 0);

        // Bind texture
        glBindTexture(GL_TEXTURE_2D, tex->m_ID);

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

        // Unbind buffers
        _unbindBuffers();

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Stop using shader
        glUseProgram(0);

        // Clear data
        m_vertexCount = 0;
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

    void Renderer::Add(std::vector<VertexData> vertices_, Renderer::PrimitiveType primitiveType_, Texture2D *texture_,
                       ShaderProgram *shader_) {
        // If the texture, shader and primitive type don't match, push a render.
        if (m_currentTexture != texture_
            || m_currentShader != shader_)
            Render();

        // Set current shader, texture and primitive
        m_currentTexture = texture_;
        m_currentShader = shader_;
        m_currentPrimitiveType = primitiveType_;

        // Add
        _addVertices(primitiveType_, vertices_.data(), vertices_.size());
    }

    void Renderer::Begin(PrimitiveType primitiveType_, Texture2D *texture_, ShaderProgram *shader_) {
        // Stop rendering.
        if (m_rendering)
            Logger::Fail("Renderer", "Cannot start a new batch while rendering.");

        // Check we aren't midway
        if (m_midBatch)
            Logger::Fail("Renderer", "Cannot begin a second batch before finishing the last one.");

        // If we are changing texture, primitive type or shader, draw
        if (m_currentTexture != texture_
            || m_currentShader != shader_)
            Render();

        // We are now running a batch
        m_midBatch = true;
        m_currentTexture = texture_;
        m_currentShader = shader_;
        m_currentPrimitiveType = primitiveType_;
    }

    void Renderer::Vertex(VertexData vertexData_) {
        // Add vertex data
        m_tempVertexData[m_tempVertexDataCount] = vertexData_;

        // Increment
        m_tempVertexDataCount++;
    }

    void Renderer::Vertex(const Vector2 &pos_, const Vector2 &texCoord_, const Color &color_) {
        if (!m_midBatch)
            Logger::Fail("Renderer", "A batch must be started before adding to it.");

        if (m_vertexCount < VBO_SIZE) {
            // Build vertex data
            VertexData vDat;
            vDat.Position = Vector3(pos_.X, pos_.Y, 0);
            vDat.TexCoords = texCoord_;
            vDat.Color = color_;

            Vertex(vDat);
        } else Logger::Fail("Renderer", "Buffer overflow.");
    }

    void Renderer::End() {
        if (!m_midBatch)
            Logger::Fail("Renderer", "Start a batch before finishing it.");

        // Add vertices to batch
        _addVertices(m_currentPrimitiveType, m_tempVertexData, m_tempVertexDataCount);
        m_tempVertexDataCount = 0;

        // Draw if at max vertex count
        if (m_vertexCount == VBO_SIZE) {
            Render();
        }

        // No longer batching.
        m_midBatch = false;
    }

    void Renderer::Render() {
        // Check we aren't already rendering
        if (m_rendering)
            Logger::Fail("Renderer", "Renderer is already rendering. Renderer should only be used on the main thread anyway.");

        // Make sure we're not mid batch.
        if (m_midBatch)
            Logger::Fail("Renderer", "Must end the current batch before rendering.");

        // Rendering now!
        m_rendering = true;

        // Configure the current framebuffer for rendering
        m_graphicsDevice->SetupFramebuffer();

        // Enable OpenGL Capabilities
        _enableGLCapabilities();

        // Render the batch for this framebuffer
        _renderBatch();

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
