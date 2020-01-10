/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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
                "in vec2 NG_VertexPos;\n"
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
                "    gl_Position = NGU_MATRIX_MVP*vec4(NG_VertexPos, 0.0, 1.0);\n"
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * MAX_BUFFER_SIZE, nullptr, GL_STREAM_DRAW);

        // Configure vertex attrib arrays
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Position));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, TexCoords));

        // Create IBO
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

        // Write null data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * MAX_BUFFER_SIZE, nullptr, GL_STREAM_DRAW);

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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Delete VAOs
        if (m_graphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            Logger::Notice("Renderer", "Deleting VAO.");
            glDeleteVertexArrays(1, &m_VAO);
        }

        // Delete VBO
        Logger::Notice("Renderer", "Deleting VBO.");
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_IBO);

        // Set all to 0
        m_VAO = 0;
        m_VBO = 0;
        m_IBO = 0;
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

            // Bind buffers
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

            // Configure vertex attrib arrays
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*) offsetof(VertexData, Position));
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

            // Unbind IBO.
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    void Renderer::_writeBuffer(unsigned int buffer_, unsigned int bufferType_, void *data_, int size_, int count_) {
        // Bind buffer
        glBindBuffer(bufferType_, buffer_);

        // Send data to buffer
#if defined(glMapBuffer)
        // Orphan data
        glBufferData(bufferType_, size_ * MAX_BUFFER_SIZE, nullptr, GL_STREAM_DRAW);

        // Replace changed vertex data
        void *buf = glMapBuffer(bufferType_, GL_WRITE_ONLY);
        memcpy(buf, data_, size_ * count_);
        glUnmapBuffer(bufferType_);
#elif defined(glBufferSubData)
        glBufferSubData(bufferType_, 0, size_ * count_, data_);
#else
        glBufferData(bufferType_, size_ * MAX_BUFFER_SIZE, data_, GL_STREAM_DRAW);
#endif
    }

    void Renderer::_addVertices(PrimitiveType type_, VertexData *vertices_, int count_) {
        // Check vertex array size
        if (count_ >= MAX_BUFFER_SIZE)
            Logger::Fail("Renderer", "Vertex array is too big!");

        // Draw if we'd meet buffer maximums
        if (m_vertexCount + count_ >= MAX_BUFFER_SIZE) {
            Render();
        }

        // Add vertices to vertex array
        if (type_ == PrimitiveType::Quads) {
            // Check we have the correct number of vertices
            if (count_ % 4 != 0)
                Logger::Fail("Renderer", "Quads instruction given wrong number of vertices.");

            // Draw if we'd meet buffer maximums
            if (m_indexCount + count_ / 4 * 6 >= MAX_BUFFER_SIZE) {
                if (count_ / 4 * 6 >= MAX_BUFFER_SIZE)
                    Logger::Fail("Renderer", "Too many indices would be created by this action.");
                Render();
            }

            // Convert from quads to triangles
            for (auto i = 0; i < floor(count_ / 4); i++) {
                m_indices[m_indexCount + i * 6 + 0] = m_vertexCount + i * 4 + 0;
                m_indices[m_indexCount + i * 6 + 1] = m_vertexCount + i * 4 + 1;
                m_indices[m_indexCount + i * 6 + 2] = m_vertexCount + i * 4 + 3;
                m_indices[m_indexCount + i * 6 + 3] = m_vertexCount + i * 4 + 1;
                m_indices[m_indexCount + i * 6 + 4] = m_vertexCount + i * 4 + 2;
                m_indices[m_indexCount + i * 6 + 5] = m_vertexCount + i * 4 + 3;
            }
            m_indexCount += count_ / 4 * 6;
        } else if (type_ == PrimitiveType::TriangleFan) {
            // Convert from triangle fan to triangles
            auto a = vertices_[0];

            // Draw if we'd meet buffer maximums
            if (m_indexCount + (count_ - 1) * 3 >= MAX_BUFFER_SIZE) {
                if ((count_ - 1) * 3 >= MAX_BUFFER_SIZE)
                    Logger::Fail("Renderer", "Too many indices would be created by this action.");
                Render();
            }

            // Write vertices to buffer
            for (auto i = 0; i + 2 < count_; i++) {
                m_indices[m_indexCount + i + 0] = m_vertexCount;
                m_indices[m_indexCount + i + 1] = m_vertexCount + i + 1;
                m_indices[m_indexCount + i + 2] = m_vertexCount + i + 2;
            }
            m_indexCount += (count_ - 1) * 3;
        } else {
            // Write standard indices
            for (auto i = 0; i < count_; i++)
                m_indices[m_indexCount + i] = m_vertexCount + i;
            m_indexCount += count_;
        }

        // Write vertices to buffer
        for (auto i = 0; i < count_; i++) {
            m_vertices[m_vertexCount + i] = vertices_[i];
            m_vertices[m_vertexCount + i].Position = m_vertices[m_vertexCount + i].Position.Transform(m_graphicsDevice->GetModelViewMatrix());
        }
        m_vertexCount += count_;

        // Draw if at max vertex count
        if (m_vertexCount == MAX_BUFFER_SIZE || m_indexCount == MAX_BUFFER_SIZE) {
            Render();
        }
    }

    void
    Renderer::_addIndexedVertices(PrimitiveType type_, VertexData *vertices_, int vCount_, unsigned short *indices_,
                                  int iCount_) {
        // TODO BEFORE MERGE: Add Quad and triangle fan conversion.
        // Add vertices to vertex array
        if (type_ == PrimitiveType::Quads) {
            Logger::Fail("Renderer", "Quads to triangle indices not implemented.");
        } else if (type_ == PrimitiveType::TriangleFan) {
            Logger::Fail("Renderer", "Triangle fan to triangle indices not implemented.");
        } else {
            // Write standard indices
            for (auto i = 0; i < iCount_; i++)
                m_indices[m_indexCount + i] = m_vertexCount + indices_[i];
            m_indexCount += iCount_;
        }

        // Write vertices to buffer
        for (auto i = 0; i < vCount_; i++) {
            m_vertices[m_vertexCount + i] = vertices_[i];
            m_vertices[m_vertexCount + i].Position = m_vertices[m_vertexCount + i].Position.Transform(m_graphicsDevice->GetModelViewMatrix());
        }
        m_vertexCount += vCount_;

        // Draw if at max vertex count
        if (m_vertexCount == MAX_BUFFER_SIZE || m_indexCount == MAX_BUFFER_SIZE) {
            Render();
        }
    }

    void Renderer::_renderBatch() {
        // Dont bother if there's nothing
        if (m_vertexCount == 0 || m_indexCount == 0) return;

        // Get projection.
        auto MVP = m_graphicsDevice->GetProjectionMatrix();

        // Upload data
        _writeBuffer(m_VBO, GL_ARRAY_BUFFER, m_vertices, sizeof(VertexData), m_vertexCount);
        _writeBuffer(m_IBO, GL_ELEMENT_ARRAY_BUFFER, m_indices, sizeof(unsigned short), m_indexCount);

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
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, 0);

        // Unbind buffers
        _unbindBuffers();

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Stop using shader
        glUseProgram(0);

        // Clear data
        m_vertexCount = 0;
        m_indexCount = 0;
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

        // Add to graphics device
        m_graphicsDevice->m_attachedRenderers.push_back(this);

        // The default matrix
        m_matrixStackCounter = 0;
        m_matrixStack[0] = Matrix::Identity;

        Logger::Notice("Renderer", "Finished creating renderer.");
    }

    Renderer::~Renderer() {
        // Remove from graphics device
        auto renderers = &m_graphicsDevice->m_attachedRenderers;
        renderers->erase(std::remove(renderers->begin(), renderers->end(), this), renderers->end());

        // Delete buffers
        _deleteBuffers();

        // Delete resources.
        delete m_whiteTexture;
        delete m_defaultShaderProgram;
    }

    void Renderer::Add(std::vector<VertexData> vertices_, PrimitiveType primitiveType_, Texture2D *texture_,
                       ShaderProgram *shader_) {
        // If the texture, shader don't match, push a render.
        if (m_currentTexture != texture_
            || m_currentShader != shader_)
            Render();

        // Set current shader, texture and primitive
        m_currentTexture = texture_;
        m_currentShader = shader_;

        // Add
        _addVertices(primitiveType_, vertices_.data(), vertices_.size());
    }

    void Renderer::AddIndexed(std::vector<VertexData> vertices_, std::vector<unsigned short> indices_,
                              PrimitiveType primitiveType_, Texture2D *texture_, ShaderProgram *shader_) {
        // If the texture, shader and primitive type don't match, push a render.
        if (m_currentTexture != texture_
            || m_currentShader != shader_)
            Render();

        // Set current shader, texture and primitive
        m_currentTexture = texture_;
        m_currentShader = shader_;

        // Add
        _addIndexedVertices(primitiveType_, vertices_.data(), vertices_.size(), indices_.data(), indices_.size());
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

        if (m_vertexCount < MAX_BUFFER_SIZE) {
            // Build vertex data
            VertexData vDat;
            vDat.Position = pos_.Transform(m_matrixStack[m_matrixStackCounter]);
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
        if (m_vertexCount == MAX_BUFFER_SIZE) {
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

    void Renderer::PushMatrix() {
        // Check limits
        if (m_matrixStackCounter + 1 >= MATRIX_STACK_SIZE)
            Logger::Fail("Renderer", "Matrix stack overflow.");

        // Increase stack counter and write current matrix to it.
        m_matrixStackCounter++;
        m_matrixStack[m_matrixStackCounter] = m_matrixStack[m_matrixStackCounter - 1];
    }

    void Renderer::PopMatrix() {
        if (m_matrixStackCounter > 0) m_matrixStackCounter--;
    }

    void Renderer::SetMatrix(const Matrix &mat_) {
        m_matrixStack[m_matrixStackCounter] = mat_;
    }

    void Renderer::LoadIdentity() {
        SetMatrix(Matrix::Identity);
    }

    void Renderer::MultMatrix(const Matrix &mat_) {
        m_matrixStack[m_matrixStackCounter] = mat_ * m_matrixStack[m_matrixStackCounter];
    }

    void Renderer::Translate(const Vector3 &translation_) {
        MultMatrix(Matrix::Translate(translation_));
    }

    void Renderer::Rotate(const Angle &rotation_, const Vector3 &axis_) {
        MultMatrix(Matrix::Rotate(rotation_, axis_));
    }

    void Renderer::Scale(const Vector3 &scale_) {
        MultMatrix(Matrix::Scale(scale_.X, scale_.Y, scale_.Z));
    }
}

#endif
