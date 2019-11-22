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
    void Renderer::__CreateBuffers() {
        // Create quad indices
        for (auto i = 0; i < MAX_VBO_SIZE / 4; i++) {
            _QuadIndices[i * 6 + 0] = (GLushort) (i * 4 + 0);
            _QuadIndices[i * 6 + 1] = (GLushort) (i * 4 + 1);
            _QuadIndices[i * 6 + 2] = (GLushort) (i * 4 + 3);
            _QuadIndices[i * 6 + 4] = (GLushort) (i * 4 + 2);
            _QuadIndices[i * 6 + 3] = (GLushort) (i * 4 + 1);
            _QuadIndices[i * 6 + 5] = (GLushort) (i * 4 + 3);
        }

        // Create quad VAO
        if (_GraphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            glGenVertexArrays(1, &_QuadVAO);
            glBindVertexArray(_QuadVAO);
        }

        // Create quad VBO's
        glGenBuffers(2, &_QuadBuffersVBO[0]);

        // Enable attrib arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // Array buffer
        glBindBuffer(GL_ARRAY_BUFFER, _QuadBuffersVBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_Vertices[0]) * MAX_VBO_SIZE, _Vertices, GL_DYNAMIC_DRAW);

        // Vertices
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              (GLvoid *) offsetof(VertexData, Position));

        // Colors
        glVertexAttribPointer(1,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              (GLvoid *) offsetof(VertexData, Color));

        // Tex coords
        glVertexAttribPointer(2,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              (GLvoid *) offsetof(VertexData, TexCoords));

        // Bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _QuadBuffersVBO[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_QuadIndices[0]) * MAX_INDEX_VBO_SIZE, _QuadIndices,
                     GL_STATIC_DRAW);

        // Unbind VAO then unbind buffers
        if (_GraphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            glBindVertexArray(0);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Renderer::__CreateDefaultShader() {
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
        _DefaultShader = new Shader(vertexShaderSrc, fragmentShaderSrc);
        auto compiled = _DefaultShader->Compile();

        if (!compiled) {
            delete _DefaultShader;
            throw std::runtime_error("Default shader failed to compile!");
        }

        // Create shader program
        _DefaultShaderProgram = new ShaderProgram(_DefaultShader);
        auto linked = _DefaultShaderProgram->Link();

        // Create default texture (for shader)
        unsigned char pixels[4] = {255, 255, 255, 255};
        _DefaultTexture = new Texture2D(_GraphicsDevice, pixels, 1, 1);
    }

    void Renderer::__SortQueue() {
        std::sort(_Queue[BUCKET_Z_NEGATIVE].begin(), _Queue[BUCKET_Z_NEGATIVE].end(), __SortPredicate);
        std::sort(_Queue[BUCKET_Z_POSITIVE].begin(), _Queue[BUCKET_Z_POSITIVE].end(), __SortPredicate);
    }

    bool Renderer::__SortPredicate(Renderer::RenderBatchItem a_, Renderer::RenderBatchItem b_) {
        return a_.Z < b_.Z;
    }

    void Renderer::__ProcessQueue() {
        __ProcessBucket(BUCKET_Z_NEGATIVE);
        __ProcessBucket(BUCKET_Z_NEUTRAL);
        __ProcessBucket(BUCKET_Z_POSITIVE);
    }

    void Renderer::__ProcessBucket(RenderBucket bucket_) {
        if (!_Queue[bucket_].empty()) {
            if (_Enable2DDepthTest) {
                glEnable(GL_DEPTH_TEST);
                glDepthMask(true);
            } else {
                glEnable(GL_DEPTH_TEST);
                glDepthMask(false);
            }

            for (auto obj : _Queue[bucket_]) {
                __ProcessBatch(obj);
            }

            Flush();
        }
    }

    void Renderer::__ProcessBatch(Renderer::RenderBatchItem i_) {
        // Pre-render checks.
        if (_VertexCount + i_.Size > MAX_VBO_SIZE || _CurrentMode != i_.Mode) {
            // Check that the item being rendered is not too big.
            if (i_.Size > MAX_VBO_SIZE) {
                throw std::runtime_error("Too many vertices in one object.");
            }

            // Render all of the old mode
            Flush();

            // Change mode now the last mode is rendered.
            _CurrentMode = i_.Mode;
        }

        // Get data
        const VertexData *vertexData = i_.VDat;

        // Add vertices
        for (auto i = 0; i < i_.Size; i++) {
            _Vertices[i + _VertexCount] = vertexData[i];
            _Vertices[i + _VertexCount].Position = vertexData[i].Position.Transform(i_.ModMat);
        }
        _VertexCount += i_.Size;

        // Add to the items batch.
        if (!_CurrentBatchItems.empty()) {
            auto lastI = _CurrentBatchItems.size() - 1;
            auto b = _CurrentBatchItems.back();

            if (b.Texture == i_.Texture
                && b.Shader == i_.Shader
                && b.Z == i_.Z) {
                _CurrentBatchItems[lastI].Size += i_.Size;
            } else _CurrentBatchItems.push_back(i_);
        } else _CurrentBatchItems.push_back(i_);
    }

    void Renderer::__DrawBatchedQuads() {
        // Upload to VBO
        if (_GraphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO)) {
            // Bind VAO
            glBindVertexArray(_QuadVAO);

            // Set VBO data
            glBindBuffer(GL_ARRAY_BUFFER, _QuadBuffersVBO[0]);

#ifdef glMapBuffer
            glBufferData(GL_ARRAY_BUFFER, sizeof(_Vertices[0]) * _VertexCount, nullptr, GL_DYNAMIC_DRAW);
            void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            memcpy(buf, _Vertices, sizeof(_Vertices[0]) * _VertexCount);
            glUnmapBuffer(GL_ARRAY_BUFFER);
#else
            glBufferData(GL_ARRAY_BUFFER, sizeof(_Vertices[0]) * _VertexCount, _Vertices, GL_DYNAMIC_DRAW);
#endif

            // Unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        } else {
            // Enable attrib arrays
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            // Bind buffer
            glBindBuffer(GL_ARRAY_BUFFER, _QuadBuffersVBO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_Vertices[0]) * _VertexCount, _Vertices, GL_DYNAMIC_DRAW);

            // Vertices
            glVertexAttribPointer(0,
                                  3,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  sizeof(VertexData),
                                  (GLvoid *) offsetof(VertexData, Position));

            // Colors
            glVertexAttribPointer(1,
                                  4,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  sizeof(VertexData),
                                  (GLvoid *) offsetof(VertexData, Color));

            // Tex coords
            glVertexAttribPointer(2,
                                  2,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  sizeof(VertexData),
                                  (GLvoid *) offsetof(VertexData, TexCoords));
        }

        // Bind indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _QuadBuffersVBO[1]);

        // Get MVP matrix (technically just projection as model view is already accounted for.)
        auto MVP = _GraphicsDevice->GetProjectionMatrix();

        // Draw vertices in the batch
        Texture2D *lastTex = nullptr;
        Texture2D *curTex = nullptr;
        ShaderProgram *lastProg = nullptr;
        ShaderProgram *curProg = nullptr;
        int fromIndex = 0;

        for (auto quad : _CurrentBatchItems) {
            // Skip empty quads
            if (quad.Size == 0) continue;

            // Apply shader
            auto sp = quad.Shader;
            if (sp != lastProg || curProg == nullptr) {
                // Set current program
                if (sp == nullptr) {
                    curProg = _DefaultShaderProgram;
                } else {
                    curProg = sp;
                }

                // Set last program
                lastProg = sp;

                // Use the program
                curProg->Use();

                // Set texture unit
                glUniform1i(curProg->GetUniformLocation("NGU_TEXTURE"), 0);

                // Set MVP
                glUniformMatrix4fv(curProg->GetUniformLocation("NGU_MATRIX_MVP"), 1, false, MVP.ToFloatArray().data());
            }

            // Apply texture
            if (quad.Texture != lastTex || curTex == nullptr) {
                // Save last texture
                lastTex = quad.Texture;

                // Set current texture
                curTex = lastTex == nullptr ? _DefaultTexture : lastTex;
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, curTex->_ID);
            }

            // Draw quad
            glDrawElements(GL_TRIANGLES, quad.Size / 4 * 6, GL_UNSIGNED_SHORT,
                           (GLvoid *) (fromIndex * sizeof(_QuadIndices[0])));

            // Increment index
            fromIndex += quad.Size / 4 * 6;
        }
    }

    void Renderer::__AddToQueue(Renderer::RenderBatchItem i_) {
        // Add to the relevant bucket.
        RenderBucket b;

        if (i_.Z < 0) {
            b = BUCKET_Z_NEGATIVE;
        } else if (i_.Z > 0) {
            b = BUCKET_Z_POSITIVE;
        } else {
            b = BUCKET_Z_NEUTRAL;
        }

        _Queue[b].push_back(i_);
    }

    Renderer::Renderer(GraphicsDevice *graphicsDevice_)
            : _GraphicsDevice(graphicsDevice_) {
        // Set the default clear color
        SetClearColor(Color::Black);

        // Initialize the 3 render buckets
        _Queue.resize(3);

        // Create buffers
        __CreateBuffers();

        // Create shader
        __CreateDefaultShader();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    Renderer::~Renderer() {
        delete _DefaultShaderProgram;
        delete _DefaultShader;
        delete _DefaultTexture;
    }

    void Renderer::Clear() {
        // Enable depth writing
        glDepthMask(true);

        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Disable depth writing
        glDepthMask(false);
    }

    void Renderer::SetClearColor(const Color &clearColor_) {
        glClearColor(clearColor_.R,
                     clearColor_.G,
                     clearColor_.B,
                     clearColor_.A);
    }

    void Renderer::Render() {
        // Starting
        _Rendering = true;

        // Sort buckets
        __SortQueue();

        // Process queue
        __ProcessQueue();

        // Clean
        Clean();

        // Finished
        _Rendering = false;
    }

    void Renderer::Clean() {
        _Queue[BUCKET_Z_NEGATIVE].clear();
        _Queue[BUCKET_Z_NEUTRAL].clear();
        _Queue[BUCKET_Z_POSITIVE].clear();
    }

    void Renderer::Flush() {
        // Render with the current mode
        switch (_CurrentMode) {
            case MODE_QUADS:
                __DrawBatchedQuads();
                break;
            case MODE_TRIANGLES:
                break;
        }

        // Clear the batch.
        _VertexCount = 0;
        _CurrentBatchItems.clear();
    }

    void Renderer::AddRenderObject(RenderObject *obj_) {
        // Check
        if (_Rendering) throw std::runtime_error("Cannot add object while rendering.");

        switch (obj_->GetType()) {
            case RenderObject::OBJECT_QUAD: {
                auto quad = (QuadsObject *) obj_;
                __AddToQueue({quad->GetQuadCount() * 4, quad->GetModelView(), quad->GetVertices(), quad->GetTexture(),
                              quad->GetShaderProgram(), quad->GetZIndex(), MODE_QUADS});
            } break;
            case RenderObject::OBJECT_PRIMITIVE:
                break;
            case RenderObject::OBJECT_TRIANGLE:
                break;
            default: throw std::runtime_error("Invalid object provided to renderer.");
        }
    }

    bool Renderer::GetDepthTest2D() {
        return _Enable2DDepthTest;
    }

    void Renderer::SetDepthTest2D(bool depthTest_) {
        if (_Rendering) throw std::runtime_error("Cannot add object while rendering.");
        _Enable2DDepthTest = depthTest_;
    }
}