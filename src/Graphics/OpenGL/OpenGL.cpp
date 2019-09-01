#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)

#include "OpenGL.h"

// OpenGL 2.1 works similarly to 3.3
#if defined(GRAPHICS_OPENGL21)
#define GRAPHICS_OPENGL33
#endif

// Platform specifics
#if defined(GRAPHICS_OPENGL33)

#include <glad/glad.h>

#elif defined(GRAPHICS_OPENGLES2)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1 // UWP???
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#endif

#if defined(PLATFORM_DESKTOP)

#include <GLFW/glfw3.h>

#elif defined(PLATFORM_UWP)
#include <angle_windowsstore.h>
#endif

#include <cstring>

// Fix missing definitions (Straight from rlgl)
#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION         0x8B8C
#endif

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT     0x83F0
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT    0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT    0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT    0x83F3
#endif
#ifndef GL_ETC1_RGB8_OES
#define GL_ETC1_RGB8_OES                    0x8D64
#endif
#ifndef GL_COMPRESSED_RGB8_ETC2
#define GL_COMPRESSED_RGB8_ETC2             0x9274
#endif
#ifndef GL_COMPRESSED_RGBA8_ETC2_EAC
#define GL_COMPRESSED_RGBA8_ETC2_EAC        0x9278
#endif
#ifndef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG  0x8C00
#endif
#ifndef GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8C02
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_4x4_KHR
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR     0x93b0
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_8x8_KHR
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR     0x93b7
#endif

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT   0x84FF
#endif

#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT       0x84FE
#endif

#if defined(GRAPHICS_OPENGLES2)
#define glClearDepth                glClearDepthf
#define GL_READ_FRAMEBUFFER         GL_FRAMEBUFFER
#define GL_DRAW_FRAMEBUFFER         GL_FRAMEBUFFER
#endif

#if defined(GRAPHICS_OPENGL21)
#define GL_LUMINANCE                        0x1909
#define GL_LUMINANCE_ALPHA                  0x190A
#endif

// GLES2 extensions
#if defined(GRAPHICS_OPENGLES2)
// NOTE: VAO functionality is exposed through extensions (OES)
static PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
static PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
static PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
#endif

namespace NerdThings::Ngine::Graphics::OpenGL {
    // Shader Related

    std::shared_ptr<GLShaderProgram> GL::_CurrentShaderProgram = nullptr;
    std::shared_ptr<GLShaderProgram> GL::_DefaultShaderProgram = nullptr;

    // Draw Batching Related Fields

    int GL::_CurrentBuffer = 0;
    float GL::_CurrentDepth = -1.0f;
    std::unique_ptr<GLDrawCall[]> GL::_DrawCalls = nullptr; // TODO: Is this the best type to use??
    int GL::_DrawCounter = 0;
    GLDynamicBuffer GL::_VertexData[];

    // Matrix Related Fields

    TMatrix *GL::_CurrentMatrix = nullptr;
    GLMatrixMode GL::_CurrentMatrixMode = MATRIX_NULL;
    TMatrix GL::_MatrixStack[];
    int GL::_MatrixStackCounter = 0;
    TMatrix GL::_ModelView = TMatrix::Identity;
    TMatrix GL::_Projection = TMatrix::Identity;
    TMatrix GL::_TransformMatrix = TMatrix::Identity;
    bool GL::_UseTransformMatrix = false;

    // Feature Related Fields

    float GL::MaxAnisotropicLevel = 0.0f;
    int GL::MaxDepthBits = 16;
    bool GL::TexAnisotropicFilterSupported = false;
    bool GL::TexCompDXTSupported = false;
    bool GL::TexCompETC1Supported = false;
    bool GL::TexCompETC2Supported = false;
    bool GL::TexCompPVRTSupported = false;
    bool GL::TexCompASTCSupported = false;
    bool GL::TexDepthSupported = false;
    bool GL::TexFloatSupported = false;
    bool GL::TexMirrorClampSupported = false;
    bool GL::TexNPOTSupported = false;
    bool GL::VAOSupported = false;

    // Textures
    std::shared_ptr<GLTexture> GL::DefaultTexture = nullptr;

    // Vertex Array Methods

    void GL::GenVertexArrays(int n, unsigned int *arrays) {
        glGenVertexArrays(n, arrays);
    }

    void GL::BindVertexArray(unsigned int array) {
        glBindVertexArray(array);
    }

    void GL::DeleteVertexArrays(int n, unsigned int *arrays) {
        glDeleteVertexArrays(n, arrays);
    }

    // Internal Methods

    void GL::DrawBuffersDefault() {
        TMatrix matProjection = _Projection;
        TMatrix matModelView = _ModelView;

        if (_VertexData[_CurrentBuffer].VCounter > 0) {
            _CurrentShaderProgram->Use();

            TMatrix matMVP = _ModelView * _Projection;
            auto mvpDat = matMVP.ToFloatArray();

            glUniformMatrix4fv(_CurrentShaderProgram->Locations[LOCATION_MATRIX_MVP], 1, false, mvpDat.get());
            glUniform1i(_CurrentShaderProgram->Locations[LOCATION_TEXTURE], 0);

            int vertexOffset = 0;
            if (VAOSupported) _VertexData[_CurrentBuffer].VAO->Bind();
            else {
                // Bind vertex attrib: position
                _VertexData[_CurrentBuffer].VBO[0]->Bind();
                glVertexAttribPointer(_CurrentShaderProgram->Locations[LOCATION_VERTEX_POSITION], 3, GL_FLOAT, 0, 0,
                                      nullptr);
                glEnableVertexAttribArray(_CurrentShaderProgram->Locations[LOCATION_VERTEX_POSITION]);

                // Bind vertex attrib: texcoord
                _VertexData[_CurrentBuffer].VBO[1]->Bind();
                glVertexAttribPointer(_CurrentShaderProgram->Locations[LOCATION_VERTEX_TEXCOORD], 2, GL_FLOAT, 0, 0,
                                      nullptr);
                glEnableVertexAttribArray(_CurrentShaderProgram->Locations[LOCATION_VERTEX_TEXCOORD]);

                // Bind vertex attrib: color
                _VertexData[_CurrentBuffer].VBO[2]->Bind();
                glVertexAttribPointer(_CurrentShaderProgram->Locations[LOCATION_VERTEX_COLOR], 4, GL_FLOAT, 0, 0,
                                      nullptr);
                glEnableVertexAttribArray(_CurrentShaderProgram->Locations[LOCATION_VERTEX_COLOR]);

                _VertexData[_CurrentBuffer].VBO[3]->Bind();
            }

            glActiveTexture(GL_TEXTURE0);

            for (auto i = 0; i < _DrawCounter; i++) {
                auto call = _DrawCalls[i];
                if (_DrawCalls[i].Texture == nullptr) continue;
                _DrawCalls[i].Texture->Bind();

                if ((_DrawCalls[i].Mode == PRIMITIVE_LINES) || (_DrawCalls[i].Mode == PRIMITIVE_TRIANGLES))
                    glDrawArrays(_DrawCalls[i].Mode, vertexOffset,
                                 _DrawCalls[i].VertexCount);
                else {
#if defined(GRAPHICS_OPENGL33)
                    glDrawElements(GL_TRIANGLES, _DrawCalls[i].VertexCount / 4 * 6, GL_UNSIGNED_INT,
                                   (GLvoid *) (sizeof(GLuint) * vertexOffset / 4 * 6));
#elif defined(GRAPHICS_OPENGLES2)
                    glDrawElements(GL_TRIANGLES, _DrawCalls[i].VertexCount / 4 * 6, GL_UNSIGNED_SHORT,
                               (GLvoid *) (sizeof(GLushort) * vertexOffset / 4 * 6));
#endif
                }

                vertexOffset += (_DrawCalls[i].VertexCount + _DrawCalls[i].VertexAlignment);
            }

            if (!VAOSupported) {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            // Unbind textures
            glBindTexture(GL_TEXTURE_2D, 0);

            if (VAOSupported) glBindVertexArray(0);

            // Unbind program
            glUseProgram(0);

            // Reset everything
            _VertexData[_CurrentBuffer].VCounter = 0;
            _VertexData[_CurrentBuffer].TCCounter = 0;
            _VertexData[_CurrentBuffer].CCounter = 0;

            // Reset depth for next draw
            _CurrentDepth = -1.0f;

            // Restore projection/modelview matrices
            _Projection = matProjection;
            _ModelView = matModelView;

            // Reset draws array
            for (int i = 0; i < MAX_DRAWCALL_REGISTERED; i++) {
                _DrawCalls[i].Mode = PRIMITIVE_QUADS;
                _DrawCalls[i].VertexCount = 0;
                _DrawCalls[i].Texture = DefaultTexture;
            }

            _DrawCounter = 1;

            // Change to next buffer in the list
            _CurrentBuffer++;
            if (_CurrentBuffer >= MAX_BATCH_BUFFERING) _CurrentBuffer = 0;
        }
    }

    void GL::LoadBuffersDefault() {
        for (int i = 0; i < MAX_BATCH_BUFFERING; i++) {
            _VertexData[i].Vertices = std::make_unique<float[]>(3 * 4 * MAX_BATCH_ELEMENTS);
            _VertexData[i].TexCoords = std::make_unique<float[]>(2 * 4 * MAX_BATCH_ELEMENTS);
            _VertexData[i].Colors = std::make_unique<float[]>(4 * 4 * MAX_BATCH_ELEMENTS);
#if defined(GRAPHICS_OPENGL33)
            _VertexData[i].Indices = std::make_unique<unsigned int[]>(6 * MAX_BATCH_ELEMENTS);
#elif defined(GRAPHICS_OPENGLES2)
            _VertexData[i].Indices = std::make_unique<unsigned short[]>(6 * MAX_BATCH_ELEMENTS);
#endif

            for (int j = 0; j < (3 * 4 * MAX_BATCH_ELEMENTS); j++) _VertexData[i].Vertices[j] = 0.0f;
            for (int j = 0; j < (2 * 4 * MAX_BATCH_ELEMENTS); j++) _VertexData[i].TexCoords[j] = 0.0f;
            for (int j = 0; j < (4 * 4 * MAX_BATCH_ELEMENTS); j++) _VertexData[i].Colors[j] = 0;

            int k = 0;

            // Indices can be initialized right now
            for (int j = 0; j < (6 * MAX_BATCH_ELEMENTS); j += 6) {
                _VertexData[i].Indices[j] = 4 * k;
                _VertexData[i].Indices[j + 1] = 4 * k + 1;
                _VertexData[i].Indices[j + 2] = 4 * k + 2;
                _VertexData[i].Indices[j + 3] = 4 * k;
                _VertexData[i].Indices[j + 4] = 4 * k + 2;
                _VertexData[i].Indices[j + 5] = 4 * k + 3;

                k++;
            }

            _VertexData[i].VCounter = 0;
            _VertexData[i].TCCounter = 0;
            _VertexData[i].CCounter = 0;
        }

        for (int i = 0; i < MAX_BATCH_BUFFERING; i++) {
            if (VAOSupported) {
                _VertexData[i].VAO = std::make_unique<GLVertexArray>();
                _VertexData[i].VAO->Bind();
            }

            _VertexData[i].VBO = std::make_unique<std::unique_ptr<GLBuffer>[]>(4);
            _VertexData[i].VBO[0] = std::make_unique<GLBuffer>(BUFFER_VERTEX);
            _VertexData[i].VBO[0]->Bind();
            _VertexData[i].VBO[0]->SetData(_VertexData[i].Vertices.get(), sizeof(float) * 3 * 4 * MAX_BATCH_ELEMENTS);
            glEnableVertexAttribArray(_CurrentShaderProgram->Locations[LOCATION_VERTEX_POSITION]);
            glVertexAttribPointer(_CurrentShaderProgram->Locations[LOCATION_VERTEX_POSITION], 3, GL_FLOAT, 0, 0,
                                  nullptr);

            _VertexData[i].VBO[1] = std::make_unique<GLBuffer>(BUFFER_VERTEX);
            _VertexData[i].VBO[1]->Bind();
            _VertexData[i].VBO[1]->SetData(_VertexData[i].TexCoords.get(), sizeof(float) * 2 * 4 * MAX_BATCH_ELEMENTS);
            glEnableVertexAttribArray(_CurrentShaderProgram->Locations[LOCATION_VERTEX_TEXCOORD]);
            glVertexAttribPointer(_CurrentShaderProgram->Locations[LOCATION_VERTEX_TEXCOORD], 2, GL_FLOAT, 0, 0,
                                  nullptr);

            _VertexData[i].VBO[2] = std::make_unique<GLBuffer>(BUFFER_VERTEX);
            _VertexData[i].VBO[2]->Bind();
            _VertexData[i].VBO[2]->SetData(_VertexData[i].Colors.get(), sizeof(float) * 4 * 4 * MAX_BATCH_ELEMENTS);
            glEnableVertexAttribArray(_CurrentShaderProgram->Locations[LOCATION_VERTEX_COLOR]);
            glVertexAttribPointer(_CurrentShaderProgram->Locations[LOCATION_VERTEX_COLOR], 4, GL_FLOAT, 0, 0, nullptr);

            _VertexData[i].VBO[3] = std::make_unique<GLBuffer>(BUFFER_INDEX, USAGE_STATIC);
            _VertexData[i].VBO[3]->Bind();
#if defined(GRAPHICS_OPENGL33)
            _VertexData[i].VBO[3]->SetData(_VertexData[i].Indices.get(), sizeof(unsigned int) * 6 * MAX_BATCH_ELEMENTS);
#elif defined(GRAPHICS_OPENGLES2)
            _VertexData[i].VBO[3]->SetData(_VertexData[i].Indices.get(), sizeof(unsigned short) * 6 * MAX_BATCH_ELEMENTS);
#endif

            if (VAOSupported) glBindVertexArray(0);
        }
    }

    void GL::LoadDefaultShader() {
        // Shader sources
        std::string vertexShaderSrc =
#if defined(GRAPHICS_OPENGL21)
                "#version 120\n"
#elif defined(GRAPHICS_OPENGLES2)
                "#version 100\n"
#endif
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21)
                "attribute vec3 vertexPosition;\n"
                "attribute vec2 vertexTexCoord;\n"
                "attribute vec4 vertexColor;\n"
                "varying vec2 fragTexCoord;\n"
                "varying vec4 fragColor;\n"
#elif defined(GRAPHICS_OPENGL33)
                "#version 330\n"
                "in vec3 vertexPosition;\n"
                "in vec2 vertexTexCoord;\n"
                "in vec4 vertexColor;\n"
                "out vec2 fragTexCoord;\n"
                "out vec4 fragColor;\n"
                #endif
                "uniform mat4 mvp;\n"
                "void main()\n"
                "{\n"
                "    fragTexCoord = vertexTexCoord;\n"
                "    fragColor = vertexColor;\n"
                "    gl_Position = mvp*vec4(vertexPosition, 1.0);\n"
                "}\n";
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
                "uniform sampler2D texture;\n"
                "void main()\n"
                "{\n"
                "    vec4 texelColor = texture2D(texture, fragTexCoord);\n"
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21)
                "    gl_FragColor = texelColor*fragColor;\n"
#elif defined(GRAPHICS_OPENGL33)
                "    finalColor = texelColor*fragColor;\n"
#endif
                "}\n";

        // Load shaders
        auto vertexShader = std::make_shared<GLShader>(vertexShaderSrc, SHADER_VERTEX);
        if (vertexShader->IsDirty()) {
            ConsoleMessage("Failed to compile internal vertex shader.", "FATAL", "OpenGL");
            throw std::runtime_error("ERROR, INTERNAL SHADER FAILED TO COMPILE!");
        }

        auto fragmentShader = std::make_shared<GLShader>(fragmentShaderSrc, SHADER_FRAGMENT);
        if (vertexShader->IsDirty()) {
            ConsoleMessage("Failed to compile internal fragment shader.", "FATAL", "OpenGL");
            throw std::runtime_error("ERROR, INTERNAL SHADER FAILED TO COMPILE!");
        }

        // Load program
        _DefaultShaderProgram = std::make_shared<GLShaderProgram>(fragmentShader, vertexShader);

        if (_DefaultShaderProgram->IsDirty()) {
            ConsoleMessage("Failed to link internal shader.", "FATAL", "OpenGL");
            throw std::runtime_error("ERROR, INTERNAL SHADER FAILED TO COMPILE!");
        }
        ConsoleMessage("Loaded internal shader.", "NOTICE", "OpenGL");
    }

    void GL::UpdateBuffersDefault() {
        if (_VertexData[_CurrentBuffer].VCounter > 0) {
            // Activate elements VAO
            if (VAOSupported) _VertexData[_CurrentBuffer].VAO->Bind();

            // Vertex positions buffer
            _VertexData[_CurrentBuffer].VBO[0]->Bind();
            _VertexData[_CurrentBuffer].VBO[0]->SetData(_VertexData[_CurrentBuffer].Vertices.get(),
                                                        sizeof(float) * 3 * 4 * MAX_BATCH_ELEMENTS);

            _VertexData[_CurrentBuffer].VBO[1]->Bind();
            _VertexData[_CurrentBuffer].VBO[1]->SetData(_VertexData[_CurrentBuffer].TexCoords.get(),
                                                        sizeof(float) * 2 * 4 * MAX_BATCH_ELEMENTS);

            _VertexData[_CurrentBuffer].VBO[2]->Bind();
            _VertexData[_CurrentBuffer].VBO[2]->SetData(_VertexData[_CurrentBuffer].Colors.get(),
                                                        sizeof(float) * 4 * 4 * MAX_BATCH_ELEMENTS);

            if (VAOSupported) glBindVertexArray(0); // Need VAO class to have this???
        }
    }

    // Matrix Related Methods

    void GL::LoadIdentity() {
        *_CurrentMatrix = TMatrix::Identity;
    }

    void GL::MatrixMode(enum GLMatrixMode mode_) {
        switch (mode_) {
            case MATRIX_MODELVIEW:
                _CurrentMatrix = &_ModelView;
                break;
            case MATRIX_PROJECTION:
                _CurrentMatrix = &_Projection;
                break;
            default:
                throw std::runtime_error("Matrix Mode not supported.");
        }

        _CurrentMatrixMode = mode_;
    }

    void GL::MultMatrix(TMatrix matrix_) {
        *_CurrentMatrix = (*_CurrentMatrix) * matrix_;
    }

    void GL::Ortho(float left_, float right_, float bottom_, float top_, float znear_, float zfar_) {
        auto matrixOrtho = TMatrix::Orthographic(left_, right_, bottom_, top_, znear_, zfar_);
        *_CurrentMatrix = *_CurrentMatrix * matrixOrtho;
    }

    void GL::PopMatrix() {
        if (_MatrixStackCounter > 0) {
            TMatrix matrix = _MatrixStack[_MatrixStackCounter - 1];
            *_CurrentMatrix = matrix;
            _MatrixStackCounter--;
        } else if (_MatrixStackCounter > 0 && _CurrentMatrixMode == MATRIX_MODELVIEW) {
            _CurrentMatrix = &_ModelView;
            _UseTransformMatrix = false;
        } else {
            throw std::runtime_error("Nothing to pop off the matrix stack.");
        }
    }

    void GL::PushMatrix() {
        if (_MatrixStackCounter > MAX_MATRIX_STACK_SIZE) throw std::runtime_error("Matrix stack overflow.");

        if (_CurrentMatrixMode == MATRIX_MODELVIEW) {
            _UseTransformMatrix = true;
            _CurrentMatrix = &_TransformMatrix;
        }

        _MatrixStack[_MatrixStackCounter] = *_CurrentMatrix;
        _MatrixStackCounter++;
    }

    void GL::Rotate(float angle_, TVector3 axis_) {
        auto rotation = TMatrix::Rotate(axis_, angle_);
        *_CurrentMatrix = rotation * (*_CurrentMatrix);
    }

    void GL::Scale(TVector3 scale_) {
        auto scale = TMatrix::Scale(scale_.X, scale_.Y, scale_.Z);
        *_CurrentMatrix = scale * (*_CurrentMatrix);
    }

    void GL::SetMatrix(TMatrix matrix_) {
        *_CurrentMatrix = matrix_;
    }

    void GL::Translate(TVector3 translation_) {
        auto translate = TMatrix::Translate(translation_);
        *_CurrentMatrix = translate * (*_CurrentMatrix);
    }

    // Vertex Methods

    void GL::Begin(GLPrimitiveMode mode_) {
        if (_DrawCalls[_DrawCounter - 1].Mode != mode_) {
            if (_DrawCalls[_DrawCounter - 1].VertexCount > 0) {
                if (_DrawCalls[_DrawCounter - 1].Mode == PRIMITIVE_LINES)
                    _DrawCalls[_DrawCounter - 1].VertexAlignment = ((_DrawCalls[_DrawCounter - 1].VertexCount < 4)
                                                                    ? _DrawCalls[_DrawCounter - 1].VertexCount :
                                                                    _DrawCalls[
                                                                            _DrawCounter -
                                                                            1].VertexCount %
                                                                    4);
                else if (_DrawCalls[_DrawCounter - 1].Mode == PRIMITIVE_TRIANGLES)
                    _DrawCalls[_DrawCounter - 1].VertexAlignment = ((_DrawCalls[_DrawCounter - 1].VertexCount < 4) ? 1
                                                                                                                   : (
                                                                            4 -
                                                                            (_DrawCalls[_DrawCounter - 1].VertexCount %
                                                                             4)));
                else _DrawCalls[_DrawCounter - 1].VertexAlignment = 0;

                if (AtBufferLimit(_DrawCalls[_DrawCounter - 1].VertexAlignment)) Draw();
                else {
                    auto va = _DrawCalls[_DrawCounter - 1].VertexAlignment;
                    _VertexData[_CurrentBuffer].VCounter += va;
                    _VertexData[_CurrentBuffer].CCounter += va;
                    _VertexData[_CurrentBuffer].TCCounter += va;

                    _DrawCounter++;
                }
            }

            if (_DrawCounter >= MAX_DRAWCALL_REGISTERED) Draw();

            _DrawCalls[_DrawCounter - 1].Mode = mode_;
            _DrawCalls[_DrawCounter - 1].VertexCount = 0;
            _DrawCalls[_DrawCounter - 1].Texture = DefaultTexture;
        }
    }

    void GL::Color(TColor color_) {
        if (_VertexData[_CurrentBuffer].CCounter < (MAX_BATCH_ELEMENTS * 4)) {
            _VertexData[_CurrentBuffer].Colors[4 * _VertexData[_CurrentBuffer].CCounter] = color_.RedF();
            _VertexData[_CurrentBuffer].Colors[4 * _VertexData[_CurrentBuffer].CCounter + 1] = color_.GreenF();
            _VertexData[_CurrentBuffer].Colors[4 * _VertexData[_CurrentBuffer].CCounter + 2] = color_.BlueF();
            _VertexData[_CurrentBuffer].Colors[4 * _VertexData[_CurrentBuffer].CCounter + 3] = color_.AlphaF();
            _VertexData[_CurrentBuffer].CCounter++;
        } else throw std::runtime_error("Buffer overflow.");
    }

    void GL::End() {
        if (_VertexData[_CurrentBuffer].VCounter != _VertexData[_CurrentBuffer].CCounter) {
            int addColors = _VertexData[_CurrentBuffer].VCounter - _VertexData[_CurrentBuffer].CCounter;

            for (auto i = 0; i < addColors; i++) {
                _VertexData[_CurrentBuffer].Colors[4 *
                                                   _VertexData[_CurrentBuffer].CCounter] = _VertexData[_CurrentBuffer].Colors[
                        4 * _VertexData[_CurrentBuffer].CCounter - 4];
                _VertexData[_CurrentBuffer].Colors[4 * _VertexData[_CurrentBuffer].CCounter +
                                                   1] = _VertexData[_CurrentBuffer].Colors[
                        4 * _VertexData[_CurrentBuffer].CCounter - 3];
                _VertexData[_CurrentBuffer].Colors[4 * _VertexData[_CurrentBuffer].CCounter +
                                                   2] = _VertexData[_CurrentBuffer].Colors[
                        4 * _VertexData[_CurrentBuffer].CCounter - 2];
                _VertexData[_CurrentBuffer].Colors[4 * _VertexData[_CurrentBuffer].CCounter +
                                                   3] = _VertexData[_CurrentBuffer].Colors[
                        4 * _VertexData[_CurrentBuffer].CCounter - 1];
                _VertexData[_CurrentBuffer].CCounter++;
            }
        }

        if (_VertexData[_CurrentBuffer].VCounter != _VertexData[_CurrentBuffer].TCCounter) {
            int addTexCoords = _VertexData[_CurrentBuffer].VCounter - _VertexData[_CurrentBuffer].TCCounter;

            for (auto i = 0; i < addTexCoords; i++) {
                _VertexData[_CurrentBuffer].TexCoords[2 * _VertexData[_CurrentBuffer].TCCounter] = 0.0f;
                _VertexData[_CurrentBuffer].TexCoords[2 * _VertexData[_CurrentBuffer].TCCounter + 1] = 0.0f;
                _VertexData[_CurrentBuffer].TCCounter++;
            }
        }

        _CurrentDepth += (1.0f / 20000.0f);

        if ((_VertexData[_CurrentBuffer].VCounter) >= (MAX_BATCH_ELEMENTS * 4 - 4)) {
            // Restore stack
            for (auto i = _MatrixStackCounter; i >= 0; i--) PopMatrix();
            Draw();
        }
    }

    void GL::StopUsingTexture() {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        if (_VertexData[_CurrentBuffer].VCounter >= (MAX_BATCH_ELEMENTS*4)) Draw();
#endif
    }

    void GL::TexCoord(TVector2 coord_) {
        if (_VertexData[_CurrentBuffer].TCCounter < (MAX_BATCH_ELEMENTS * 4)) {
            _VertexData[_CurrentBuffer].TexCoords[2 * _VertexData[_CurrentBuffer].TCCounter] = coord_.X;
            _VertexData[_CurrentBuffer].TexCoords[2 * _VertexData[_CurrentBuffer].TCCounter + 1] = coord_.Y;
            _VertexData[_CurrentBuffer].TCCounter++;
        } else throw std::runtime_error("Buffer overflow.");
    }

    void GL::UseTexture(std::shared_ptr<GLTexture> texture_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)

        auto call = _DrawCalls[_DrawCounter - 1];

        // If the current draw call uses a different texture and has data, get ready to open a new draw call
        if (_DrawCalls[_DrawCounter - 1].Texture != texture_) {
            if (_DrawCalls[_DrawCounter - 1].VertexCount > 0) {
                // Make sure current vertex count is aligned a multiple of 4,
                if (_DrawCalls[_DrawCounter - 1].Mode == PRIMITIVE_LINES)
                    _DrawCalls[_DrawCounter - 1].VertexAlignment = ((_DrawCalls[_DrawCounter - 1].VertexCount < 4)
                                                                    ? _DrawCalls[_DrawCounter - 1].VertexCount :
                                                                    _DrawCalls[_DrawCounter - 1].VertexCount % 4);
                else if (_DrawCalls[_DrawCounter - 1].Mode == PRIMITIVE_TRIANGLES)
                    _DrawCalls[_DrawCounter - 1].VertexAlignment = ((_DrawCalls[_DrawCounter - 1].VertexCount < 4) ? 1
                                                                                                                   :
                                                                           (4 - (_DrawCalls[_DrawCounter -
                                                                                            1].VertexCount % 4)));
                else _DrawCalls[_DrawCounter - 1].VertexAlignment = 0;

                if (AtBufferLimit(_DrawCalls[_DrawCounter - 1].VertexAlignment)) Draw();
                else {
                    _VertexData[_CurrentBuffer].VCounter += _DrawCalls[_DrawCounter - 1].VertexAlignment;
                    _VertexData[_CurrentBuffer].CCounter += _DrawCalls[_DrawCounter - 1].VertexAlignment;
                    _VertexData[_CurrentBuffer].TCCounter += _DrawCalls[_DrawCounter - 1].VertexAlignment;

                    _DrawCounter++;
                }
            }

            // Draw if we reached our limit
            if (_DrawCounter >= MAX_DRAWCALL_REGISTERED) Draw();

            _DrawCalls[_DrawCounter - 1].Texture = texture_;
            _DrawCalls[_DrawCounter - 1].VertexCount = 0;
        }
#endif
    }

    void GL::Vertex(TVector2 pos_) {
        Vertex({pos_.X, pos_.Y, _CurrentDepth});
    }

    void GL::Vertex(TVector3 pos_) {
        if (_UseTransformMatrix) pos_ = pos_.Transform(_TransformMatrix);

        // Verify limit
        if (_VertexData[_CurrentBuffer].VCounter < (MAX_BATCH_ELEMENTS * 4)) {
            _VertexData[_CurrentBuffer].Vertices[3 * _VertexData[_CurrentBuffer].VCounter] = pos_.X;
            _VertexData[_CurrentBuffer].Vertices[3 * _VertexData[_CurrentBuffer].VCounter + 1] = pos_.Y;
            _VertexData[_CurrentBuffer].Vertices[3 * _VertexData[_CurrentBuffer].VCounter + 2] = pos_.Z;
            _VertexData[_CurrentBuffer].VCounter++;

            _DrawCalls[_DrawCounter - 1].VertexCount++;
        } else throw std::runtime_error("Buffer overflow.");
    }

    // Buffering Methods

    bool GL::AtBufferLimit(int vertexCount_) {
        bool overflow = false;
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL33)
        if ((_VertexData[_CurrentBuffer].VCounter + vertexCount_) >= (MAX_BATCH_ELEMENTS * 4)) overflow = true;
#endif
        return overflow;
    }

    void GL::Draw() {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        if (_VertexData[_CurrentBuffer].VCounter > 0) {
            UpdateBuffersDefault();
            DrawBuffersDefault();
        }
#endif
    }

    // Management Methods

    void GL::Cleanup() {
        for (auto i = 0; i < MAX_BATCH_BUFFERING; i++)
            _VertexData[i] = GLDynamicBuffer();
    }

    void GL::Init() {
        // Init GLAD
#if defined(GRAPHICS_OPENGL33)
        // Init glad for the first time
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            glfwTerminate();
            ConsoleMessage("Failed to init GLAD.", "FATAL", "OpenGL");
            throw std::runtime_error("Failed to init GLAD.");
        }
#endif

        // Load extensions
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        int numExt = 0;

#if defined(GRAPHICS_OPENGL33)
        // Supported by default
        VAOSupported = true;

        // Default supported
        TexNPOTSupported = true;
        TexFloatSupported = true;
        TexDepthSupported = true;

        // Get extension count
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

        // Allocate extensions list
        const char **extList = new const char *[numExt];

        // Get strings
        for (auto i = 0; i < numExt; i++) extList[i] = (char *) glGetStringi(GL_EXTENSIONS, i);
#elif defined(GRAPHICS_OPENGLES2)
        // Allocate 2kb of strings
        const char **extList = new const char*[512];

        // Get extension strings
        char *extensions = (char *)glGetString(GL_EXTENSIONS);
        int len = strlen(extensions);

        for (auto i = 0; i < len; i++) {
            if (i == ' ') {
                extList[numExt] = &extensions[i + 1];
                numExt++;
            }
        }
#endif
        // Search for extensions
        for (auto i = 0; i < numExt; i++) {
#if defined (GRAPHICS_OPENGLES2)
            // Check for VAO support

            if (strcmp(extList[i], "GL_OES_vertex_array_object") == 0)
            {
                // The extension is supported by our hardware and driver, try to get related functions pointers
                glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
                glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
                glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");

                if ((glGenVertexArrays != NULL) && (glBindVertexArray != NULL) && (glDeleteVertexArrays != NULL)) VAOSupported = true;
            }

            // Check NPOT textures support
            if (strcmp(extList[i], "GL_OES_texture_npot") == 0) TexNPOTSupported = true;

            // Check texture float support
            if (strcmp(extList[i], "GL_OES_texture_float") == 0) TexFloatSupported = true;

            // Check depth texture support
            if ((strcmp(extList[i], "GL_OES_depth_texture") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_depth_texture") == 0)) TexDepthSupported = true;

            if (strcmp(extList[i], "GL_OES_depth24") == 0) MaxDepthBits = 24;
            if (strcmp(extList[i], "GL_OES_depth32") == 0) MaxDepthBits = 32;
#endif
            // DDS texture compression support
            if ((strcmp(extList[i], "GL_EXT_texture_compression_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBKIT_WEBGL_compressed_texture_s3tc") == 0))
                TexCompDXTSupported = true;

            // ETC1 texture compression support
            if ((strcmp(extList[i], "GL_OES_compressed_ETC1_RGB8_texture") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_etc1") == 0))
                TexCompETC1Supported = true;

            // ETC2/EAC texture compression support
            if (strcmp(extList[i], "GL_ARB_ES3_compatibility") == 0) TexCompETC2Supported = true;

            // PVR texture compression support
            if (strcmp(extList[i], "GL_IMG_texture_compression_pvrtc") == 0) TexCompPVRTSupported = true;

            // ASTC texture compression support
            if (strcmp(extList[i], "GL_KHR_texture_compression_astc_hdr") == 0) TexCompASTCSupported = true;

            // Anisotropic texture filter
            if (strcmp(extList[i], (const char *) "GL_EXT_texture_filter_anisotropic") == 0) {
                TexAnisotropicFilterSupported = true;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaxAnisotropicLevel);
            }

            // Clamp mirror wrap mode supported
            if (strcmp(extList[i], (const char *) "GL_EXT_texture_mirror_clamp") == 0) TexMirrorClampSupported = true;
        }
#endif

        // Load shader
        LoadDefaultShader();
        _CurrentShaderProgram = _DefaultShaderProgram;

        unsigned char pixels[4] = {255, 255, 255, 255};   // 1 pixel RGBA (4 bytes)
        DefaultTexture = std::make_unique<GLTexture>(1, 1, pixels, 1, UNCOMPRESSED_R8G8B8A8);

        // Init draw calls tracking system
        _DrawCalls = std::make_unique<GLDrawCall[]>(MAX_DRAWCALL_REGISTERED);

        for (int i = 0; i < MAX_DRAWCALL_REGISTERED; i++) {
            _DrawCalls[i].Mode = PRIMITIVE_QUADS;
            _DrawCalls[i].VertexCount = 0;
            _DrawCalls[i].VertexAlignment = 0;
            _DrawCalls[i].Texture = DefaultTexture;
        }

        _DrawCounter = 1;

        LoadBuffersDefault();

        _Projection = TMatrix::Identity;
        _ModelView = TMatrix::Identity;
        _CurrentMatrix = &_ModelView;

        // Init OpenGL states
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Clear
        glClearDepth(1.0f);
        ClearColor(TColor(0, 0, 0, 1));
        Clear();

        ConsoleMessage("Finished initializing OpenGL API.", "NOTICE", "OpenGL");
    }

    // Other OpenGL Methods

    void GL::Clear() {
        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GL::ClearColor(TColor color_) {
        // Set clear color
        glClearColor(color_.RedF(), color_.GreenF(), color_.BlueF(), color_.AlphaF());
    }

    void GL::GetGLTextureFormats(int format_, unsigned int *glInternalFormat_, unsigned int *glFormat_,
                                 unsigned int *glType_) {
        *glInternalFormat_ = -1;
        *glFormat_ = -1;
        *glType_ = -1;

        switch (format_) {
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
            // NOTE: on OpenGL ES 2.0 (WebGL), internalFormat must match format and options allowed are: GL_LUMINANCE, GL_RGB, GL_RGBA
        case UNCOMPRESSED_GRAYSCALE:
            *glInternalFormat_ = GL_LUMINANCE;
            *glFormat_ = GL_LUMINANCE;
            *glType_ = GL_UNSIGNED_BYTE;
            break;
        case UNCOMPRESSED_GRAY_ALPHA:
            *glInternalFormat_ = GL_LUMINANCE_ALPHA;
            *glFormat_ = GL_LUMINANCE_ALPHA;
            *glType_ = GL_UNSIGNED_BYTE;
            break;
        case UNCOMPRESSED_R5G6B5:
            *glInternalFormat_ = GL_RGB;
            *glFormat_ = GL_RGB;
            *glType_ = GL_UNSIGNED_SHORT_5_6_5;
            break;
        case UNCOMPRESSED_R8G8B8:
            *glInternalFormat_ = GL_RGB;
            *glFormat_ = GL_RGB;
            *glType_ = GL_UNSIGNED_BYTE;
            break;
        case UNCOMPRESSED_R5G5B5A1:
            *glInternalFormat_ = GL_RGBA;
            *glFormat_ = GL_RGBA;
            *glType_ = GL_UNSIGNED_SHORT_5_5_5_1;
            break;
        case UNCOMPRESSED_R4G4B4A4:
            *glInternalFormat_ = GL_RGBA;
            *glFormat_ = GL_RGBA;
            *glType_ = GL_UNSIGNED_SHORT_4_4_4_4;
            break;
        case UNCOMPRESSED_R8G8B8A8:
            *glInternalFormat_ = GL_RGBA;
            *glFormat_ = GL_RGBA;
            *glType_ = GL_UNSIGNED_BYTE;
            break;
        case UNCOMPRESSED_R32:
            // NOTE: Requires extension OES_texture_float
            if (TexFloatSupported) {
                *glInternalFormat_ = GL_LUMINANCE;
                *glFormat_ = GL_LUMINANCE;
                *glType_ = GL_FLOAT;
            }
            break;
        case UNCOMPRESSED_R32G32B32:
            // NOTE: Requires extension OES_texture_float
            if (TexFloatSupported) {
                *glInternalFormat_ = GL_RGB;
                *glFormat_ = GL_RGB;
                *glType_ = GL_FLOAT;
            }
            break;
        case UNCOMPRESSED_R32G32B32A32:
            // NOTE: Requires extension OES_texture_float
            if (TexFloatSupported) {
                *glInternalFormat_ = GL_RGBA;
                *glFormat_ = GL_RGBA;
                *glType_ = GL_FLOAT;
            }
            break;
#elif defined(GRAPHICS_OPENGL33)
            case UNCOMPRESSED_GRAYSCALE:
                *glInternalFormat_ = GL_R8;
                *glFormat_ = GL_RED;
                *glType_ = GL_UNSIGNED_BYTE;
                break;
            case UNCOMPRESSED_GRAY_ALPHA:
                *glInternalFormat_ = GL_RG8;
                *glFormat_ = GL_RG;
                *glType_ = GL_UNSIGNED_BYTE;
                break;
            case UNCOMPRESSED_R5G6B5:
                *glInternalFormat_ = GL_RGB565;
                *glFormat_ = GL_RGB;
                *glType_ = GL_UNSIGNED_SHORT_5_6_5;
                break;
            case UNCOMPRESSED_R8G8B8:
                *glInternalFormat_ = GL_RGB8;
                *glFormat_ = GL_RGB;
                *glType_ = GL_UNSIGNED_BYTE;
                break;
            case UNCOMPRESSED_R5G5B5A1:
                *glInternalFormat_ = GL_RGB5_A1;
                *glFormat_ = GL_RGBA;
                *glType_ = GL_UNSIGNED_SHORT_5_5_5_1;
                break;
            case UNCOMPRESSED_R4G4B4A4:
                *glInternalFormat_ = GL_RGBA4;
                *glFormat_ = GL_RGBA;
                *glType_ = GL_UNSIGNED_SHORT_4_4_4_4;
                break;
            case UNCOMPRESSED_R8G8B8A8:
                *glInternalFormat_ = GL_RGBA8;
                *glFormat_ = GL_RGBA;
                *glType_ = GL_UNSIGNED_BYTE;
                break;
            case UNCOMPRESSED_R32:
                if (TexFloatSupported) *glInternalFormat_ = GL_R32F;
                *glFormat_ = GL_RED;
                *glType_ = GL_FLOAT;
                break;
            case UNCOMPRESSED_R32G32B32:
                if (TexFloatSupported) *glInternalFormat_ = GL_RGB32F;
                *glFormat_ = GL_RGB;
                *glType_ = GL_FLOAT;
                break;
            case UNCOMPRESSED_R32G32B32A32:
                if (TexFloatSupported) *glInternalFormat_ = GL_RGBA32F;
                *glFormat_ = GL_RGBA;
                *glType_ = GL_FLOAT;
                break;
#endif
            case COMPRESSED_DXT1_RGB:
                if (TexCompDXTSupported) *glInternalFormat_ = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
                break;
            case COMPRESSED_DXT1_RGBA:
                if (TexCompDXTSupported) *glInternalFormat_ = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                break;
            case COMPRESSED_DXT3_RGBA:
                if (TexCompDXTSupported) *glInternalFormat_ = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                break;
            case COMPRESSED_DXT5_RGBA:
                if (TexCompDXTSupported) *glInternalFormat_ = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                break;
            case COMPRESSED_ETC1_RGB:
                if (TexCompETC1Supported) *glInternalFormat_ = GL_ETC1_RGB8_OES;
                break;                      // NOTE: Requires OpenGL ES 2.0 or OpenGL 4.3
            case COMPRESSED_ETC2_RGB:
                if (TexCompETC2Supported) *glInternalFormat_ = GL_COMPRESSED_RGB8_ETC2;
                break;               // NOTE: Requires OpenGL ES 3.0 or OpenGL 4.3
            case COMPRESSED_ETC2_EAC_RGBA:
                if (TexCompETC2Supported) *glInternalFormat_ = GL_COMPRESSED_RGBA8_ETC2_EAC;
                break;     // NOTE: Requires OpenGL ES 3.0 or OpenGL 4.3
            case COMPRESSED_PVRT_RGB:
                if (TexCompPVRTSupported) *glInternalFormat_ = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
                break;    // NOTE: Requires PowerVR GPU
            case COMPRESSED_PVRT_RGBA:
                if (TexCompPVRTSupported) *glInternalFormat_ = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
                break;  // NOTE: Requires PowerVR GPU
            case COMPRESSED_ASTC_4x4_RGBA:
                if (TexCompASTCSupported) *glInternalFormat_ = GL_COMPRESSED_RGBA_ASTC_4x4_KHR;
                break;  // NOTE: Requires OpenGL ES 3.1 or OpenGL 4.3
            case COMPRESSED_ASTC_8x8_RGBA:
                if (TexCompASTCSupported) *glInternalFormat_ = GL_COMPRESSED_RGBA_ASTC_8x8_KHR;
                break;  // NOTE: Requires OpenGL ES 3.1 or OpenGL 4.3
            default:
                throw std::runtime_error("Unsupported format.");
        }
    }

    GLVersion GL::GetGLVersion() {
#if defined(GRAPHICS_OPENGL21)
        return OPENGL_21;
#elif defined(GRAPHICS_OPENGL33)
        return OPENGL_33;
#elif defined(GRAPHICS_OPENGLES2)
        return OPENGL_ES2;
#endif
        return OPENGL_UNKNOWN;
    }

    void GL::Viewport(int x_, int y_, int width_, int height_) {
        // Set viewport
        glViewport(x_, y_, width_, height_);
    }
}
#endif
