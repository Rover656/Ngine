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
        if (_GraphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO) && _UseVAO) {
            glGenVertexArrays(1, &_QuadVAO);
            glBindVertexArray(_QuadVAO);
        }

        // Create quad VBO's
        glGenBuffers(2, &_QuadBuffersVBO[0]);

        // Array buffer
        glBindBuffer(GL_ARRAY_BUFFER, _QuadBuffersVBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_QuadVertices[0]) * MAX_VBO_SIZE, _QuadVertices, GL_DYNAMIC_DRAW);

        // Vertices
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              (GLvoid *) offsetof(VertexData, Position));

        // Colors
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(VertexData),
                              (GLvoid *) offsetof(VertexData, Color));

        // Tex coords
        glEnableVertexAttribArray(2);
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
        if (_GraphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO) && _UseVAO) {
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

    void Renderer::__SortBuckets() {
        std::sort(_Queue[BUCKET_Z_NEGATIVE].begin(), _Queue[BUCKET_Z_NEGATIVE].end(), __SortPredicate);
        std::sort(_Queue[BUCKET_Z_POSITIVE].begin(), _Queue[BUCKET_Z_POSITIVE].end(), __SortPredicate);
    }

    bool Renderer::__SortPredicate(RenderObject *a_, RenderObject *b_) {
        return a_->GetZIndex() < b_->GetZIndex();
    }

    void Renderer::__ProcessQueue() {
        __ProcessBucket(BUCKET_Z_NEGATIVE);
        __ProcessBucket(BUCKET_Z_NEUTRAL);
        __ProcessBucket(BUCKET_Z_POSITIVE);
    }

    void Renderer::__ProcessBucket(int queue_) {
        if (!_Queue[queue_].empty()) {
            if (_Enable2DDepthTest) {
                glEnable(GL_DEPTH_TEST);
                glDepthMask(true);
            } else {
                glEnable(GL_DEPTH_TEST);
                glDepthMask(false);
            }

            for (auto obj : _Queue[queue_]) {
                __ProcessObject(obj);
            }

            Flush();
        }
    }

    void Renderer::__ProcessObject(RenderObject *obj_) {
        auto type = obj_->GetType();

        if (type == RenderObject::OBJECT_QUAD) {
            // Add to quads batch.
            auto quad = (QuadsObject *) obj_;

            if (false || (_QuadCount + quad->GetQuadCount()) * 4 > MAX_VBO_SIZE) {
                if (quad->GetQuadCount() < 0 || quad->GetQuadCount() * 4 > MAX_VBO_SIZE) {
                    throw std::runtime_error("Too many quads in one object.");
                }
                __DrawQuadsBatch();
            }

            // Add to the quads batch.
            auto lastQuadIDX = _BatchedQuads.size() - 1;
            if (!_BatchedQuads.empty()
                && _BatchedQuads[lastQuadIDX].GetZIndex() == quad->GetZIndex()
                && _BatchedQuads[lastQuadIDX].GetShaderProgram() == quad->GetShaderProgram()
                && _BatchedQuads[lastQuadIDX].GetTexture() == quad->GetTexture()) {
                // Reuse existing batch. Reduces the number of iterations done when rendering.
                _BatchedQuads[lastQuadIDX].SetQuadCount(_BatchedQuads[lastQuadIDX].GetQuadCount() + quad->GetQuadCount());
            } else {
                // Add into quad batch
                _BatchedQuads.push_back(*quad);
            }

            // Get data
            const Matrix &modelView = quad->GetModelView();
            const VertexData *vertexData = quad->GetVertices();

            // Add to batch
            for (auto i = 0; i < quad->GetQuadCount() * 4; i++) {
                _QuadVertices[i + _QuadCount * 4] = vertexData[i];
                _QuadVertices[i + _QuadCount * 4].Position = vertexData[i].Position.Transform(modelView);
            }
            _QuadCount += quad->GetQuadCount();
        }
    }

    void Renderer::__DrawQuadsBatch() {
        // Skip if nothing
        if (_QuadCount == 0 || _BatchedQuads.empty()) return;

        // Upload to VBO
        if (_GraphicsDevice->GetGLSupportFlag(GraphicsDevice::GL_VAO) && _UseVAO) {
#if defined(GRAPHICS_OPENGL33)
            // Bind VAO
            glBindVertexArray(_QuadVAO);

            // Set VBO data
            glBindBuffer(GL_ARRAY_BUFFER, _QuadBuffersVBO[0]);

            glBufferData(GL_ARRAY_BUFFER, sizeof(_QuadVertices[0]) * _QuadCount * 4, nullptr, GL_DYNAMIC_DRAW);
            void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            memcpy(buf, _QuadVertices, sizeof(_QuadVertices[0]) * _QuadCount * 4);
            glUnmapBuffer(GL_ARRAY_BUFFER);

            // Unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
        } else {
            // TODO: This is broken
            glBindBuffer(GL_ARRAY_BUFFER, _QuadBuffersVBO[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_QuadVertices[0]) * _QuadCount * 4, _QuadVertices, GL_DYNAMIC_DRAW);
            __ErrorReport();

            // Vertices
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0,
                                  3,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  sizeof(VertexData),
                                  (GLvoid *) offsetof(VertexData, Position));

            __ErrorReport();

            // Colors
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1,
                                  4,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  sizeof(VertexData),
                                  (GLvoid *) offsetof(VertexData, Color));

            __ErrorReport();

            // Tex coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2,
                                  2,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  sizeof(VertexData),
                                  (GLvoid *) offsetof(VertexData, TexCoords));

            __ErrorReport();
        }

        // Get MVP matrix (technically just projection as model view is already accounted for.)
        auto MVP = _GraphicsDevice->GetProjectionMatrix();

        // Draw vertices in the batch
        Texture2D *lastTex = nullptr;
        Texture2D *curTex = nullptr;
        ShaderProgram *lastProg = nullptr;
        ShaderProgram *curProg = nullptr;
        int fromIndex = 0;

        for (auto quad : _BatchedQuads) {
            // Skip empty quads
            if (quad.GetQuadCount() == 0) continue;

            // Apply shader
            auto sp = quad.GetShaderProgram();
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
                __ErrorReport();

                // Set MVP
                glUniformMatrix4fv(curProg->GetUniformLocation("NGU_MATRIX_MVP"), 1, false, MVP.ToFloatArray().data());
                __ErrorReport();
            }

            // Apply texture
            if (quad.GetTexture() != lastTex || curTex == nullptr) {
                // Save last texture
                lastTex = quad.GetTexture();

                // Set current texture
                curTex = lastTex == nullptr ? _DefaultTexture : lastTex;
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, curTex->_ID);
            }

            // Draw quad
            glDrawElements(GL_TRIANGLES, (GLsizei) quad.GetQuadCount() * 6, GL_UNSIGNED_SHORT,
                           (GLvoid *) (fromIndex * sizeof(_QuadIndices[0])));

            __ErrorReport();

            // Increment index
            fromIndex += quad.GetQuadCount() * 6;
        }
    }

    void Renderer::__ErrorReport() {
        auto err = glGetError();
        if (err != 0) ConsoleMessage("OpenGL Error: " + std::to_string(err), "ERR", "Renderer");
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
        __SortBuckets();

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

        // Clean quad batch
        _BatchedQuads.clear();
        _QuadCount = 0;
    }

    void Renderer::Flush() {
        __DrawQuadsBatch();
    }

    void Renderer::AddRenderObject(RenderObject *obj_) {
        // Check
        if (_Rendering) throw std::runtime_error("Cannot add object while rendering.");
        // TODO: Check type

        // Add to the relevant bucket.
        if (obj_->GetZIndex() < 0) {
            _Queue[BUCKET_Z_NEGATIVE].push_back(obj_);
        } else if (obj_->GetZIndex() > 0) {
            _Queue[BUCKET_Z_POSITIVE].push_back(obj_);
        } else {
            _Queue[BUCKET_Z_NEUTRAL].push_back(obj_);
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