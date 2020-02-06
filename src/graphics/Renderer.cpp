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

#include "graphics/Renderer.hpp"

#include "graphics/VertexDataConverter.hpp"
#include "Console.hpp"

namespace ngine::graphics {
    void Renderer::_addTriangles(Vertex *vertices_, int count_, bool translate_) {
        // Check array size
        if (count_ > MAX_TRIANGLE_VERTICES)
            Console::Fail("Renderer", "Too many vertices for one batch.");

        // Check if the batch will be full
        if (m_vertexCount + count_ >= MAX_TRIANGLE_VERTICES || m_indexCount + count_ >= MAX_TRIANGLE_VERTICES)
            renderBatch();

        // Add vertices and indices
        for (auto i = 0; i < count_; i++) {
            m_vertexArray[m_vertexCount + i] = vertices_[i];
            if (translate_) {
                m_vertexArray[m_vertexCount + i].Position = vertices_[i].Position.transform(
                        m_matrixStack[m_matrixStackCounter] * m_graphicsDevice->getViewMatrix());
            }
            m_indexArray[m_indexCount + i] = m_vertexCount + i;
        }

        m_vertexCount += count_;
        m_indexCount += count_;

        // Check if the batch is full
        if (m_vertexCount + 1 >= MAX_TRIANGLE_VERTICES || m_indexCount + 1 >= MAX_TRIANGLE_VERTICES)
            renderBatch();
    }

    void Renderer::_addIndexedTriangles(Vertex *vertices_, int vCount_, unsigned short *indices_, int iCount_,
                                        bool translate_) {
        // Check array sizes
        if (vCount_ > MAX_TRIANGLE_VERTICES || iCount_ > MAX_TRIANGLE_VERTICES) {
            Console::Fail("Renderer", "Too many vertices and/or indices for one batch.");
        }

        // Check if the batch will be full
        if (m_vertexCount + vCount_ >= MAX_TRIANGLE_VERTICES || m_indexCount + iCount_ >= MAX_TRIANGLE_VERTICES)
            renderBatch();

        // Add vertices
        for (auto i = 0; i < vCount_; i++) {
            m_vertexArray[m_vertexCount + i] = vertices_[i];
            if (translate_) {
                m_vertexArray[m_vertexCount + i].Position = vertices_[i].Position.transform(
                        m_matrixStack[m_matrixStackCounter] * m_graphicsDevice->getViewMatrix());
            }
        }

        // Add indices
        for (auto i = 0; i < iCount_; i++)
            m_indexArray[m_indexCount + i] = m_vertexCount + indices_[i];

        // Increase counters
        m_vertexCount += vCount_;
        m_indexCount += iCount_;

        // Check if the batch is full
        if (m_vertexCount + 1 >= MAX_TRIANGLE_VERTICES || m_indexCount + 1 >= MAX_TRIANGLE_VERTICES)
            renderBatch();
    }

    Renderer::Renderer(GraphicsDevice *graphicsDevice_) : m_graphicsDevice(graphicsDevice_) {
        // Create vertex buffer
        m_VBO = new Buffer(m_graphicsDevice, BufferType::Vertex, BufferUsage::Dynamic);

        // Write null data
        m_VBO->write<Vertex>(nullptr, MAX_TRIANGLE_VERTICES);

        // Create IBO
        m_IBO = new Buffer(m_graphicsDevice, BufferType::Index, BufferUsage::Dynamic);

        // Write null data
        m_IBO->write<unsigned short>(nullptr, MAX_TRIANGLE_VERTICES);

        // Create vertex layout TODO: Tidy API a little
        m_layout = new VertexLayout(m_graphicsDevice, m_VBO, m_IBO);
        m_layout->addElement({
                                     "", // DX
                                     0, // GL
                                     3,
                                     VertexLayout::VertexElementType::Float,
                                     sizeof(Vertex),
                                     offsetof(Vertex, Position)
                             });
        m_layout->addElement({
                                     "",
                                     2,
                                     2,
                                     VertexLayout::VertexElementType::Float,
                                     sizeof(Vertex),
                                     offsetof(Vertex, TexCoords)
                             });
        m_layout->addElement({
                                     "",
                                     1,
                                     4,
                                     VertexLayout::VertexElementType::Float,
                                     sizeof(Vertex),
                                     offsetof(Vertex, Color)
                             });
        m_layout->configure();

        // Create default shader

        const char *vSrc = nullptr;
        const char *fSrc = nullptr;

        auto major = GraphicsDevice::GetTargetAPIMajorVersion();
        auto minor = GraphicsDevice::GetTargetAPIMinorVersion();
        switch (GraphicsDevice::GetTargetAPI()) {
            case GraphicsAPI::OpenGL:
                if (major >= 3) {
                    vSrc = GL33_V_SHADER;
                    fSrc = GL33_F_SHADER;
                }
                break;
            case GraphicsAPI::OpenGLES:
                if (major == 2) {
                    vSrc = GLES2_V_SHADER;
                    fSrc = GLES2_F_SHADER;
                } else if (major == 3) {
                    vSrc = GL33_V_SHADER;
                    fSrc = GL33_F_SHADER;
                }
                break;
            case GraphicsAPI::DirectX:
                break;
        }

        if (vSrc == nullptr || fSrc == nullptr)
            Console::Fail("Renderer", "Shader not available for current graphics API.");

        // Create shader
        auto vShader = new Shader(m_graphicsDevice, ShaderType::Vertex, vSrc);
        auto fShader = new Shader(m_graphicsDevice, ShaderType::Fragment, fSrc);

        // Create shader program
        m_defaultShader = new ShaderProgram(m_graphicsDevice, vShader, fShader);

        // Initialize data structures
        ShaderDataStructure MVP;
        MVP.Name = "NGINE_MATRIX_MVP";
        MVP.Type = ShaderDataType::Matrix;

        ShaderDataStructure texture;
        texture.Name = "NGINE_TEXTURE";
        texture.Type = ShaderDataType::Int;

        // Setup uniforms
        m_defaultShader->addUniform(MVP);
        m_defaultShader->addUniform(texture);
        m_defaultShader->finalize();

        // Create program state
        m_defaultShaderState = new ShaderProgramState(m_defaultShader);
        int defaultTextureUnit = 0;
        m_defaultShaderState->setUniform("NGINE_TEXTURE", &defaultTextureUnit);

        // Create default texture (for shader)
        unsigned char pixels[4] = {255, 255, 255, 255};
        m_whiteTexture = new Texture2D(m_graphicsDevice, pixels, 1, 1);

        // The default matrix
        m_matrixStackCounter = 0;
        m_matrixStack[0] = Matrix::Identity;

        // Add to graphics device for tracking
        m_graphicsDevice->m_attachedRenderers.push_back(this);
    }

    Renderer::~Renderer() {
        // Remove from graphics device
        m_graphicsDevice->m_attachedRenderers.erase(
                std::remove(m_graphicsDevice->m_attachedRenderers.begin(), m_graphicsDevice->m_attachedRenderers.end(),
                            this), m_graphicsDevice->m_attachedRenderers.end());

        // Stop using internal layout (if enabled).
        m_layout->stop();

        // Unbind buffers
        m_VBO->unbind();
        m_IBO->unbind();

        // Delete Layout
        delete m_layout;

        // Delete VBO
        delete m_VBO;
        delete m_IBO;

        // Set all to null
        m_layout = nullptr;
        m_VBO = nullptr;
        m_IBO = nullptr;
    }

    void Renderer::addTriangles(Vertex *vertices_, int count_) {
        _addTriangles(vertices_, count_, true);
    }

    void Renderer::addIndexedTriangles(Vertex *vertices_, int vCount_, unsigned short *indices_, int iCount_) {
        _addIndexedTriangles(vertices_, vCount_, indices_, iCount_, true);
    }

    void Renderer::beginVertices(PrimitiveType type_) {
        if (m_buildingVertices)
            Console::Fail("Renderer", "Cannot begin pushing vertices until last vertices have been finished.");
        m_buildingVertices = true;
        m_builtType = type_;
    }

    void Renderer::endVertices() {
        // Check type
        if (m_builtType != PrimitiveType::Triangles) {
            // Convert
            unsigned short *indices;
            auto count = VertexDataConverter::GetTriangleIndices(m_builtType, m_builtVertexCount, &indices);

            // Push
            _addIndexedTriangles(m_builtVertices, m_builtVertexCount, indices, count);

            // Delete
            delete[] indices;
        } else {
            // Push raw triangles
            _addTriangles(m_builtVertices, m_builtVertexCount);
        }

        m_builtVertexCount = 0;
        m_buildingVertices = false;
    }

    void Renderer::pushVertex(Vertex vertex_) {
        // Save vertex
        m_builtVertices[m_builtVertexCount] = vertex_;

        // Translate right now
        m_builtVertices[m_builtVertexCount].Position = vertex_.Position.transform(
                m_matrixStack[m_matrixStackCounter] * m_graphicsDevice->getViewMatrix());

        m_builtVertexCount++;
    }

    void Renderer::setTexture(Texture2D *texture_) {
        if (m_currentTexture != texture_) {
            renderBatch();
            m_currentTexture = texture_;
        }
    }

    void Renderer::setShader(ShaderProgramState *state_) {
        if (m_currentShaderState != state_) {
            renderBatch();
            m_currentShaderState = state_;
        }
    }

    void Renderer::renderBatch() {
        // No data, no render
        if (m_vertexCount == 0 || m_indexCount == 0) return;

        // Write each buffer
        m_VBO->write<Vertex>(m_vertexArray, m_vertexCount, true);
        m_IBO->write<unsigned short>(m_indexArray, m_indexCount, true);

        // Render our batch buffers
        renderBufferIndexed(m_layout, m_VBO, m_IBO, m_indexCount,
                            m_currentTexture != nullptr ? m_currentTexture : m_whiteTexture,
                            m_currentShaderState != nullptr ? m_currentShaderState : m_defaultShaderState);

        // Clear data
        m_vertexCount = 0;
        m_indexCount = 0;
    }

    void Renderer::renderBuffer(VertexLayout *layout_, Buffer *VBO_, int count_, Texture2D *texture_,
                                ShaderProgramState *state_) {
        renderBufferIndexed(layout_, VBO_, nullptr, count_, texture_, state_);
    }

    void Renderer::renderBufferIndexed(VertexLayout *layout_, Buffer *VBO_, Buffer *IBO_, int count_,
                                       Texture2D *texture_, ShaderProgramState *state_) {
        // Check buffer types
        if (VBO_->Type != BufferType::Vertex || (IBO_ != nullptr && IBO_->Type != BufferType::Index))
            Console::Fail("Renderer", "Incorrect buffers provided to RenderBufferIndexed");

        // Configure the current framebuffer for rendering
        m_graphicsDevice->setupFramebuffer();

        // Set predefined uniforms (only if they are on the lowest level, i.e. not nested inside of another structure).
        auto MVP = m_graphicsDevice->getProjectionMatrix();
        state_->setUniform("NGINE_MATRIX_MVP", MVP.toFloatArray().data()); // TODO: Option for how the data is aligned so DX11 and OpenGL don't argue.
        int texUnit = 0;
        state_->setUniform("NGINE_TEXTURE", &texUnit);

        // Use layout
        layout_->use();

        // Get graphics API
        auto api = m_graphicsDevice->getAPI();

        // Prepare for 2D
        api->prepareFor2D();

        // Use shader
        api->bindShaderProgramState(state_);

        // Bind texture TODO: Should we ever consider the possibility of multiple textures?
        api->bindTexture(texture_);

        // Draw all elements
        if (IBO_ != nullptr)
            api->drawIndexed(count_, 0);
        else
            api->draw(count_, 0);

        // Unbind buffers
        m_layout->stop();

        // Unbind texture
        api->bindTexture(nullptr);

        // Stop using shader
        api->bindShaderProgram(nullptr);
    }

    void Renderer::pushMatrix() {
        // Check limits
        if (m_matrixStackCounter + 1 >= MATRIX_STACK_SIZE)
            Console::Fail("Renderer", "Matrix stack overflow.");

        // Increase stack counter and write current matrix to it.
        m_matrixStackCounter++;
        m_matrixStack[m_matrixStackCounter] = m_matrixStack[m_matrixStackCounter - 1];
    }

    void Renderer::popMatrix() {
        if (m_matrixStackCounter > 0) m_matrixStackCounter--;
    }

    void Renderer::setMatrix(const Matrix &mat_) {
        m_matrixStack[m_matrixStackCounter] = mat_;
    }

    void Renderer::loadIdentity() {
        setMatrix(Matrix::Identity);
    }

    void Renderer::multiplyMatrix(const Matrix &mat_) {
        m_matrixStack[m_matrixStackCounter] = mat_ * m_matrixStack[m_matrixStackCounter];
    }

    void Renderer::translate(const Vector3 &translation_) {
        multiplyMatrix(Matrix::Translate(translation_));
    }

    void Renderer::rotate(const Angle &rotation_, const Vector3 &axis_) {
        multiplyMatrix(Matrix::Rotate(rotation_, axis_));
    }

    void Renderer::scale(const Vector3 &scale_) {
        multiplyMatrix(Matrix::Scale(scale_.X, scale_.Y, scale_.Z));
    }

    bool Renderer::willFit(PrimitiveType type_, int elements_) {
        switch (type_) {
            case PrimitiveType::Triangles:
                return elements_ < MAX_TRIANGLE_VERTICES;
                break;
            case PrimitiveType::TriangleFan:
                return (elements_ - 1) * 3 < MAX_TRIANGLE_VERTICES;
                break;
            case PrimitiveType::Quads:
                return elements_ / 4 * 6 < MAX_TRIANGLE_VERTICES;
                break;
        }
    }

    GraphicsDevice *Renderer::getGraphicsDevice() {
        return m_graphicsDevice;
    }
}