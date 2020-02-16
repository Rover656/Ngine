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
#include "Window.hpp"

namespace ngine::graphics {
    void Renderer::_addTriangles(Vertex *vertices, int count, bool translate) {
        // Check array size
        if (count > MAX_TRIANGLE_VERTICES)
            Console::Fail("Renderer", "Too many vertices for one batch.");

        // Check if the batch will be full
        if (m_vertexCount + count >= MAX_TRIANGLE_VERTICES || m_indexCount + count >= MAX_TRIANGLE_VERTICES)
            renderBatch();

        // Add vertices and indices
        for (auto i = 0; i < count; i++) {
            m_vertexArray[m_vertexCount + i] = vertices[i];
            if (translate) {
                m_vertexArray[m_vertexCount + i].Position = vertices[i].Position.transform(
                        m_graphicsDevice->getViewMatrix() * m_matrixStack[m_matrixStackCounter]);
            }
            m_indexArray[m_indexCount + i] = m_vertexCount + i;
        }

        m_vertexCount += count;
        m_indexCount += count;

        // Check if the batch is full
        if (m_vertexCount + 1 >= MAX_TRIANGLE_VERTICES || m_indexCount + 1 >= MAX_TRIANGLE_VERTICES)
            renderBatch();
    }

    void Renderer::_addIndexedTriangles(Vertex *vertices, int vCount, unsigned short *indices, int iCount,
                                        bool translate) {
        // Check array sizes
        if (vCount > MAX_TRIANGLE_VERTICES || iCount > MAX_TRIANGLE_VERTICES) {
            Console::Fail("Renderer", "Too many vertices and/or indices for one batch.");
        }

        // Check if the batch will be full
        if (m_vertexCount + vCount >= MAX_TRIANGLE_VERTICES || m_indexCount + iCount >= MAX_TRIANGLE_VERTICES)
            renderBatch();

        // Add vertices
        for (auto i = 0; i < vCount; i++) {
            m_vertexArray[m_vertexCount + i] = vertices[i];
            if (translate) {
                m_vertexArray[m_vertexCount + i].Position = vertices[i].Position.transform(
                        m_graphicsDevice->getViewMatrix() * m_matrixStack[m_matrixStackCounter]);
            }
        }

        // Add indices
        for (auto i = 0; i < iCount; i++)
            m_indexArray[m_indexCount + i] = m_vertexCount + indices[i];

        // Increase counters
        m_vertexCount += vCount;
        m_indexCount += iCount;

        // Check if the batch is full
        if (m_vertexCount + 1 >= MAX_TRIANGLE_VERTICES || m_indexCount + 1 >= MAX_TRIANGLE_VERTICES)
            renderBatch();
    }

    Renderer::Renderer(GraphicsDevice *graphicsDevice) : m_graphicsDevice(graphicsDevice) {
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

        // Init shader sources
        const char *vSrc = nullptr;
        const char *fSrc = nullptr;

        // Determine shader
        auto window = m_graphicsDevice->getWindow();
        auto major = window->getContextAPIMajorVersion();
        auto minor = window->getContextAPIMinorVersion();
        switch (window->getContextAPI()) {
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

    void Renderer::addTriangles(Vertex *vertices, int count) {
        _addTriangles(vertices, count, true);
    }

    void Renderer::addIndexedTriangles(Vertex *vertices, int vCount, unsigned short *indices, int iCount) {
        _addIndexedTriangles(vertices, vCount, indices, iCount, true);
    }

    void Renderer::beginVertices(PrimitiveType type) {
        if (m_buildingVertices)
            Console::Fail("Renderer", "Cannot begin pushing vertices until last vertices have been finished.");
        m_buildingVertices = true;
        m_builtType = type;
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

    void Renderer::pushVertex(Vertex vertex) {
        // Save vertex
        m_builtVertices[m_builtVertexCount] = vertex;

        // Translate right now
        m_builtVertices[m_builtVertexCount].Position = vertex.Position.transform(
                m_graphicsDevice->getViewMatrix() * m_matrixStack[m_matrixStackCounter]);

        m_builtVertexCount++;
    }

    void Renderer::setCoordinateSystem(CoordinateSystem system) {
        if (m_currentCoordSystem != system) {
            // Force render
            renderBatch();
            m_currentCoordSystem = system;
        }
    }

    CoordinateSystem Renderer::getCoordinateSystem() {
        return m_currentCoordSystem;
    }

    void Renderer::setTexture(Texture2D *texture) {
        if (m_currentTexture != texture) {
            renderBatch();
            m_currentTexture = texture;
        }
    }

    void Renderer::setShader(ShaderProgramState *state) {
        if (m_currentShaderState != state) {
            renderBatch();
            m_currentShaderState = state;
        }
    }

    void Renderer::setModelViewMatrix(Matrix modelView) {
        // TODO: Comparing of matrices
        renderBatch();
        m_currentModelView = modelView;
    }

    void Renderer::renderBatch() {
        // No data, no render
        if (m_vertexCount == 0 || m_indexCount == 0) return;

        // Write each buffer
        m_VBO->write<Vertex>(m_vertexArray, m_vertexCount, true);
        m_IBO->write<unsigned short>(m_indexArray, m_indexCount, true);

        // Render our batch buffers
        renderBufferIndexed(m_graphicsDevice->getCurrentViewport(), m_currentCoordSystem, m_layout, m_VBO, m_IBO,
                            m_indexCount, m_currentTexture != nullptr ? m_currentTexture : m_whiteTexture,
                            m_currentShaderState != nullptr ? m_currentShaderState : m_defaultShaderState);

        // Clear data
        m_vertexCount = 0;
        m_indexCount = 0;
    }

    void
    Renderer::renderBuffer(const Viewport *viewport, CoordinateSystem coordSys, VertexLayout *layout, Buffer *VBO_,
                           int count, Texture2D *texture, ShaderProgramState *state) {
        renderBufferIndexed(viewport, coordSys, layout, VBO_, nullptr, count, texture, state);
    }

    void Renderer::renderBufferIndexed(const Viewport *viewport, CoordinateSystem coordSys, VertexLayout *layout,
                                       Buffer *VBO, Buffer *IBO, int count, Texture2D *texture,
                                       ShaderProgramState *state_) {
        // Check buffer types
        if (VBO->Type != BufferType::Vertex || (IBO != nullptr && IBO->Type != BufferType::Index))
            Console::Fail("Renderer", "Incorrect buffers provided to RenderBufferIndexed");

        // Check viewport
        if (viewport == nullptr)
            Console::Fail("Renderer", "Viewport cannot be null!");

        // Get graphics API
        auto api = m_graphicsDevice->getAPI();

        // Configure viewport
        api->configureViewport(viewport->getX(), viewport->getY(), viewport->getWidth(), viewport->getHeight());

        // Set predefined uniforms (only if they are on the lowest level, i.e. not nested inside of another structure).
        auto MVP = viewport->getProjection(coordSys) * m_currentModelView;
        state_->setUniform("NGINE_MATRIX_MVP",
                           MVP.toFloatArray().data()); // TODO: Option for how the data is aligned so DX11 and OpenGL don't argue.
        int texUnit = 0;
        state_->setUniform("NGINE_TEXTURE", &texUnit);

        // Use layout
        layout->use();

        // Prepare for 2D
        api->prepareFor2D();

        // Use shader
        api->bindShaderProgramState(state_);

        // Bind texture TODO: Should we ever consider the possibility of multiple textures?
        api->bindTexture(texture);

        // Draw all elements
        if (IBO != nullptr)
            api->drawIndexed(count, 0);
        else
            api->draw(count, 0);

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

    void Renderer::setMatrix(const Matrix &mat) {
        m_matrixStack[m_matrixStackCounter] = mat;
    }

    void Renderer::loadIdentity() {
        setMatrix(Matrix::Identity);
    }

    void Renderer::multiplyMatrix(const Matrix &mat) {
        m_matrixStack[m_matrixStackCounter] = m_matrixStack[m_matrixStackCounter] * mat;
    }

    void Renderer::translate(const Vector3 &translation) {
        multiplyMatrix(Matrix::Translate(translation));
    }

    void Renderer::rotate(const Angle &rotation, const Vector3 &axis) {
        multiplyMatrix(Matrix::Rotate(rotation, axis));
    }

    void Renderer::scale(const Vector3 &scale) {
        multiplyMatrix(Matrix::Scale(scale.X, scale.Y, scale.Z));
    }

    bool Renderer::willFit(PrimitiveType type, int elements) {
        switch (type) {
            case PrimitiveType::Triangles:
                return elements < MAX_TRIANGLE_VERTICES;
                break;
            case PrimitiveType::TriangleFan:
                return (elements - 1) * 3 < MAX_TRIANGLE_VERTICES;
                break;
            case PrimitiveType::Quads:
                return elements / 4 * 6 < MAX_TRIANGLE_VERTICES;
                break;
        }
    }

    GraphicsDevice *Renderer::getGraphicsDevice() {
        return m_graphicsDevice;
    }
}
