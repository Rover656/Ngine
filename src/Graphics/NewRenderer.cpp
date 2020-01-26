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

#include "Graphics/NewRenderer.hpp"

#if defined(PLATFORM_DESKTOP)
#include <glad/glad.h>
#elif defined(PLATFORM_UWP) && defined(GRAPHICS_OPENGLES2)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1 // UWP???
#include "../../../third-party/ANGLE/GLES2/gl2.h"
#include "../../../third-party/ANGLE/GLES2/gl2ext.h"
#include "../../../third-party/ANGLE/EGL/egl.h"
#endif

#include "Console.hpp"

namespace Ngine::Graphics {
    void VertexDataTool::GenerateRect(float width_, float height_, Color color_, std::vector<Vertex> &vertexArray_,
                                      std::vector<unsigned short> &indexArray_) {
        auto vertStart = vertexArray_.size();
        vertexArray_.push_back({{0, 0, 0},
                                {0, 0},
                                color_});
        vertexArray_.push_back({{0, height_, 0},
                                {0, 1},
                                color_});
        vertexArray_.push_back({{width_, height_, 0},
                                {1, 1},
                                color_});
        vertexArray_.push_back({{width_, 0, 0},
                                {1, 0},
                                color_});

        indexArray_.push_back(vertStart + 0);
        indexArray_.push_back(vertStart + 1);
        indexArray_.push_back(vertStart + 3);
        indexArray_.push_back(vertStart + 1);
        indexArray_.push_back(vertStart + 2);
        indexArray_.push_back(vertStart + 3);
    }

    NewRenderer::NewRenderer(GraphicsDevice *graphicsDevice_) : m_graphicsDevice(graphicsDevice_) {
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
                "    gl_Position = NGU_MATRIX_MVP * vec4(NG_VertexPos, 1.0);\n"
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
    }

    NewRenderer::~NewRenderer() {
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

    void NewRenderer::AddTriangles(Vertex *vertices_, int count_) {
        // Check array size
        if (count_ > MAX_TRIANGLE_VERTICES)
            Console::Fail("Renderer", "Too many vertices for one batch.");

        // Check if the batch will be full
        if (m_vertexCount + count_ >= MAX_TRIANGLE_VERTICES || m_indexCount + count_ >= MAX_TRIANGLE_VERTICES)
            RenderBatch();

        // Add vertices and indices
        for (auto i = 0; i < count_; i++) {
            m_vertexArray[m_vertexCount + i] = vertices_[i];
            m_vertexArray[m_vertexCount + i].Position = vertices_[i].Position.Transform(
                    m_matrixStack[m_matrixStackCounter]);
            m_indexArray[m_indexCount + i] = m_vertexCount + i;
        }
        m_vertexCount += count_;
        m_indexCount += count_;

        // Check if the batch is full
        if (m_vertexCount + 1 >= MAX_TRIANGLE_VERTICES || m_indexCount + 1 >= MAX_TRIANGLE_VERTICES)
            RenderBatch();
    }

    void NewRenderer::AddIndexedTriangles(Vertex *vertices_, int vCount_, unsigned short *indices_, int iCount_) {
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
            m_vertexArray[m_vertexCount + i].Position = vertices_[i].Position.Transform(
                    m_matrixStack[m_matrixStackCounter]);
        }

        // Add indices
        for (auto i = 0; i < iCount_; i++) m_indexArray[m_indexCount + i] = m_vertexCount + indices_[i];

        // Increase counters
        m_vertexCount += vCount_;
        m_indexCount += iCount_;

        // Check if the batch is full
        if (m_vertexCount + 1 >= MAX_TRIANGLE_VERTICES || m_indexCount + 1 >= MAX_TRIANGLE_VERTICES)
            RenderBatch();
    }

    void NewRenderer::SetTexture(Texture2D *texture_) {
        if (m_currentTexture != texture_) {
            RenderBatch();
            m_currentTexture = texture_;
        }
    }

    void NewRenderer::SetShader(ShaderProgram *shader_) {
        if (m_currentShader != shader_) {
            RenderBatch();
            m_currentShader = shader_;
        }
    }

    void NewRenderer::RenderBatch() {
        // No data, no render
        if (m_vertexCount == 0 || m_indexCount == 0) return;

        // Write each buffer
        m_VBO->Write<Vertex>(m_vertexArray, m_vertexCount, true);
        m_IBO->Write<unsigned short>(m_indexArray, m_indexCount, true);

        // Render our batch buffers
        RenderBufferIndexed(m_layout, m_VBO, m_IBO, m_indexCount,
                            m_currentTexture != 0 ? m_currentTexture : m_whiteTexture,
                            m_currentShader != nullptr ? m_currentShader : m_defaultShader);

        // Clear data
        m_vertexCount = 0;
        m_indexCount = 0;
    }

    void NewRenderer::RenderBuffer(VertexLayout *layout_, Buffer *VBO_, int count_, Texture2D *texture_,
                                   ShaderProgram *shader_) {
        RenderBufferIndexed(layout_, VBO_, nullptr, count_, texture_, shader_);
    }

    void NewRenderer::RenderBufferIndexed(VertexLayout *layout_, Buffer *VBO_, Buffer *IBO_, int count_,
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

    void NewRenderer::PushMatrix() {
        // Check limits
        if (m_matrixStackCounter + 1 >= MATRIX_STACK_SIZE)
            Console::Fail("Renderer", "Matrix stack overflow.");

        // Increase stack counter and write current matrix to it.
        m_matrixStackCounter++;
        m_matrixStack[m_matrixStackCounter] = m_matrixStack[m_matrixStackCounter - 1];
    }

    void NewRenderer::PopMatrix() {
        if (m_matrixStackCounter > 0) m_matrixStackCounter--;
    }

    void NewRenderer::SetMatrix(const Matrix &mat_) {
        m_matrixStack[m_matrixStackCounter] = mat_;
    }

    void NewRenderer::LoadIdentity() {
        SetMatrix(Matrix::Identity);
    }

    void NewRenderer::MultiplyMatrix(const Matrix &mat_) {
        m_matrixStack[m_matrixStackCounter] = mat_ * m_matrixStack[m_matrixStackCounter];
    }

    void NewRenderer::Translate(const Vector3 &translation_) {
        MultiplyMatrix(Matrix::Translate(translation_));
    }

    void NewRenderer::Rotate(const Angle &rotation_, const Vector3 &axis_) {
        MultiplyMatrix(Matrix::Rotate(rotation_, axis_));
    }

    void NewRenderer::Scale(const Vector3 &scale_) {
        MultiplyMatrix(Matrix::Scale(scale_.X, scale_.Y, scale_.Z));
    }

    GraphicsDevice *NewRenderer::GetGraphicsDevice() {
        return m_graphicsDevice;
    }
}