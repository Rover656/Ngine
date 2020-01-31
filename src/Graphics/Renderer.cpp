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

#include "Graphics/Renderer.hpp"

#include "Graphics/VertexDataTool.hpp"
#include "Console.hpp"

#if defined(PLATFORM_DESKTOP)

#include <glad/glad.h>

#elif defined(PLATFORM_UWP)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1

// Include latest GLES header
#include <GLES3/gl31.h>
// Add GLES2 extensions
#include <GLES2/gl2ext.h>

#include "../../../third-party/ANGLE/EGL/egl.h"
#endif

namespace Ngine::Graphics {
    void Renderer::_addTriangles(Vertex *vertices_, int count_, bool translate_) {
        // Check array size
        if (count_ > MAX_TRIANGLE_VERTICES)
            Console::Fail("Renderer", "Too many vertices for one batch.");

        // Check if the batch will be full
        if (m_vertexCount + count_ >= MAX_TRIANGLE_VERTICES || m_indexCount + count_ >= MAX_TRIANGLE_VERTICES)
            RenderBatch();

        // Add vertices and indices
        for (auto i = 0; i < count_; i++) {
            m_vertexArray[m_vertexCount + i] = vertices_[i];
            if (translate_) {
                m_vertexArray[m_vertexCount + i].Position = vertices_[i].Position.Transform(
                        m_matrixStack[m_matrixStackCounter] * m_graphicsDevice->GetViewMatrix());
            }
            m_indexArray[m_indexCount + i] = m_vertexCount + i;
        }

        m_vertexCount += count_;
        m_indexCount += count_;

        // Check if the batch is full
        if (m_vertexCount + 1 >= MAX_TRIANGLE_VERTICES || m_indexCount + 1 >= MAX_TRIANGLE_VERTICES)
            RenderBatch();
    }

    void Renderer::_addIndexedTriangles(Vertex *vertices_, int vCount_, unsigned short *indices_, int iCount_,
                                        bool translate_) {
        // Check array sizes
        if (vCount_ > MAX_TRIANGLE_VERTICES || iCount_ > MAX_TRIANGLE_VERTICES) {
            Console::Fail("Renderer", "Too many vertices and/or indices for one batch.");
        }

        // Check if the batch will be full
        if (m_vertexCount + vCount_ >= MAX_TRIANGLE_VERTICES || m_indexCount + iCount_ >= MAX_TRIANGLE_VERTICES)
            RenderBatch();

        // Add vertices
        for (auto i = 0; i < vCount_; i++) {
            m_vertexArray[m_vertexCount + i] = vertices_[i];
            if (translate_) {
                m_vertexArray[m_vertexCount + i].Position = vertices_[i].Position.Transform(
                        m_matrixStack[m_matrixStackCounter] * m_graphicsDevice->GetViewMatrix());
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
            RenderBatch();
    }

    Renderer::Renderer(GraphicsDevice *graphicsDevice_) : m_graphicsDevice(graphicsDevice_) {
        // Create vertex buffer
        m_VBO = new Buffer(m_graphicsDevice, BufferType::Vertex, BufferUsage::Dynamic);

        // Write null data
        m_VBO->Write<Vertex>(nullptr, MAX_TRIANGLE_VERTICES);

        // Create IBO
        m_IBO = new Buffer(m_graphicsDevice, BufferType::Index, BufferUsage::Dynamic);

        // Write null data
        m_IBO->Write<unsigned short>(nullptr, MAX_TRIANGLE_VERTICES);

        // Create vertex layout TODO: Tidy API a little
        m_layout = new VertexLayout(m_graphicsDevice, m_VBO, m_IBO);
        m_layout->AddElement({
                                     "", // DX
                                     0, // GL
                                     3,
                                     VertexLayout::VertexElementType::Float,
                                     sizeof(Vertex),
                                     offsetof(Vertex, Position)
                             });
        m_layout->AddElement({
                                     "",
                                     2,
                                     2,
                                     VertexLayout::VertexElementType::Float,
                                     sizeof(Vertex),
                                     offsetof(Vertex, TexCoords)
                             });
        m_layout->AddElement({
                                     "",
                                     1,
                                     4,
                                     VertexLayout::VertexElementType::Float,
                                     sizeof(Vertex),
                                     offsetof(Vertex, Color)
                             });
        m_layout->Configure();

        // Create default shader

        const char *vSrc = nullptr;
        const char *fSrc = nullptr;

        auto major = GraphicsDevice::GetTargetAPIMajorVersion();
        auto minor = GraphicsDevice::GetTargetAPIMinorVersion();
        switch (GraphicsDevice::GetTargetAPI()) {
            case GraphicsAPI::OpenGL:
                if (major == 2) {
                    vSrc = GL21_V_SHADER;
                    fSrc = GL21_F_SHADER;
                } else if (major >= 3) {
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
        auto shader = new Shader(vSrc, fSrc);
        auto compiled = shader->Compile();

        if (!compiled) {
            delete shader;
            Console::Fail("Renderer", "Failed to compile internal shader!");
        }

        // Create shader program
        m_defaultShader = new ShaderProgram(shader);
        auto linked = m_defaultShader->Link();
        delete shader;

        if (!linked) {
            delete m_defaultShader;
            m_defaultShader = nullptr;
            Console::Fail("Renderer", "Failed to link internal shader program!");
        }

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
        m_layout->Stop();

        // Unbind buffers
        m_VBO->Unbind();
        m_IBO->Unbind();

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

    void Renderer::AddTriangles(Vertex *vertices_, int count_) {
        _addTriangles(vertices_, count_, true);
    }

    void Renderer::AddIndexedTriangles(Vertex *vertices_, int vCount_, unsigned short *indices_, int iCount_) {
        _addIndexedTriangles(vertices_, vCount_, indices_, iCount_, true);
    }

    void Renderer::BeginVertices(PrimitiveType type_) {
        if (m_buildingVertices)
            Console::Fail("Renderer", "Cannot begin pushing vertices until last vertices have been finished.");
        m_buildingVertices = true;
        m_builtType = type_;
    }

    void Renderer::EndVertices() {
        // Check type
        if (m_builtType != PrimitiveType::Triangles) {
            // Convert
            unsigned short *indices;
            auto count = VertexDataTool::GetTriangleIndices(m_builtType, m_builtVertexCount, &indices);

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

    void Renderer::PushVertex(Vertex vertex_) {
        // Save vertex
        m_builtVertices[m_builtVertexCount] = vertex_;

        // Translate right now
        m_builtVertices[m_builtVertexCount].Position = vertex_.Position.Transform(
                m_matrixStack[m_matrixStackCounter] * m_graphicsDevice->GetViewMatrix());

        m_builtVertexCount++;
    }

    void Renderer::SetTexture(Texture2D *texture_) {
        if (m_currentTexture != texture_) {
            RenderBatch();
            m_currentTexture = texture_;
        }
    }

    void Renderer::SetShader(ShaderProgram *shader_) {
        if (m_currentShader != shader_) {
            RenderBatch();
            m_currentShader = shader_;
        }
    }

    void Renderer::RenderBatch() {
        // No data, no render
        if (m_vertexCount == 0 || m_indexCount == 0) return;

        // Write each buffer
        m_VBO->Write<Vertex>(m_vertexArray, m_vertexCount, true);
        m_IBO->Write<unsigned short>(m_indexArray, m_indexCount, true);

        // Render our batch buffers
        RenderBufferIndexed(m_layout, m_VBO, m_IBO, m_indexCount,
                            m_currentTexture != nullptr ? m_currentTexture : m_whiteTexture,
                            m_currentShader != nullptr ? m_currentShader : m_defaultShader);

        // Clear data
        m_vertexCount = 0;
        m_indexCount = 0;
    }

    void Renderer::RenderBuffer(VertexLayout *layout_, Buffer *VBO_, int count_, Texture2D *texture_,
                                ShaderProgram *shader_) {
        RenderBufferIndexed(layout_, VBO_, nullptr, count_, texture_, shader_);
    }

    void Renderer::RenderBufferIndexed(VertexLayout *layout_, Buffer *VBO_, Buffer *IBO_, int count_,
                                       Texture2D *texture_, ShaderProgram *shader_) {
        // Check buffer types
        if (VBO_->Type != BufferType::Vertex || (IBO_ != nullptr && IBO_->Type != BufferType::Index))
            Console::Fail("Renderer", "Incorrect buffers provided to RenderBufferIndexed");

        // Configure the current framebuffer for rendering
        m_graphicsDevice->SetupFramebuffer();

        // Get MVP
        auto MVP = m_graphicsDevice->GetProjectionMatrix();

        // Use layout
        layout_->Use();

        // Get graphics API
        auto api = m_graphicsDevice->GetAPI();

        // Prepare for 2D
        api->PrepareFor2D();

        // Use shader
        api->BindShader(shader_);

        // Set the MVP matrix. It is just the projection as model view is already applied in vertices
        glUniformMatrix4fv(shader_->GetUniformLocation("NGU_MATRIX_MVP"), 1, GL_FALSE, MVP.ToFloatArray().data());

        // Bind texture
        api->BindTexture(texture_);

        // Draw all elements
        if (IBO_ != nullptr)
            api->DrawIndexed(count_, 0);
        else
            api->Draw(count_, 0);

        // Unbind buffers
        m_layout->Stop();

        // Unbind texture
        api->BindTexture(nullptr);

        // Stop using shader
        api->BindShader(nullptr);
    }

    void Renderer::PushMatrix() {
        // Check limits
        if (m_matrixStackCounter + 1 >= MATRIX_STACK_SIZE)
            Console::Fail("Renderer", "Matrix stack overflow.");

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

    void Renderer::MultiplyMatrix(const Matrix &mat_) {
        m_matrixStack[m_matrixStackCounter] = mat_ * m_matrixStack[m_matrixStackCounter];
    }

    void Renderer::Translate(const Vector3 &translation_) {
        MultiplyMatrix(Matrix::Translate(translation_));
    }

    void Renderer::Rotate(const Angle &rotation_, const Vector3 &axis_) {
        MultiplyMatrix(Matrix::Rotate(rotation_, axis_));
    }

    void Renderer::Scale(const Vector3 &scale_) {
        MultiplyMatrix(Matrix::Scale(scale_.X, scale_.Y, scale_.Z));
    }

    bool Renderer::WillFit(PrimitiveType type_, int elements_) {
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

    GraphicsDevice *Renderer::GetGraphicsDevice() {
        return m_graphicsDevice;
    }
}
