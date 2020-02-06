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

#include "PlatformGLAPI.hpp"

#if defined(API_OPENGL_ENABLED) || defined(API_OPENGLES_ENABLED)

#if defined(GLAD)
#include <glad/glad.h>
#endif

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#elif defined(EGL)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1 

#if !defined(GLAD)
// Include latest GLES header
#include <GLES3/gl31.h>
// Add GLES2 extensions
#include <GLES2/gl2ext.h>
#endif

#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

// Fix any missing GL definitions
#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#endif
#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif
#ifndef GL_ETC1_RGB8_OES
#define GL_ETC1_RGB8_OES 0x8D64
#endif
#ifndef GL_COMPRESSED_RGB8_ETC2
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#endif
#ifndef GL_COMPRESSED_RGBA8_ETC2_EAC
#define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#endif
#ifndef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG 0x8C00
#endif
#ifndef GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8C02
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_4x4_KHR
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0x93b0
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_8x8_KHR
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR 0x93b7
#endif
#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif
#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

#if defined(PLATFORM_UWP)
#include <angle_windowsstore.h>
#include "UWP/GameApp.hpp"
#endif

#include "graphics/Buffer.hpp"
#include "graphics/Shader.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/RenderTarget.hpp"
#include "graphics/VertexLayout.hpp"
#include "Console.hpp"

#include <cstring>

#if defined(GLAD)
static PFNGLGENVERTEXARRAYSPROC glGenVertexArraysOES;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArrayOES;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArraysOES;
#endif

namespace ngine::graphics::API {
    void
    PlatformGLAPI::_getTextureFormats(PixelFormat format_, unsigned int *glInternalFormat_, unsigned int *glFormat_,
                                      unsigned int *glType_) {
        // Set to -1 for error checking
        *glInternalFormat_ = -1;
        *glFormat_ = -1;
        *glType_ = -1;

#if defined(API_OPENGLES_ENABLED)
        // GLES2 and 3 specific
        if (m_GLES2 || m_GLES3) {
            switch (format_) {
                case UNCOMPRESSED_GRAYSCALE:
                    *glInternalFormat_ = GL_LUMINANCE;
                    *glFormat_ = GL_LUMINANCE;
                    *glType_ = GL_UNSIGNED_BYTE;
                    return;
                case UNCOMPRESSED_GRAY_ALPHA:
                    *glInternalFormat_ = GL_LUMINANCE_ALPHA;
                    *glFormat_ = GL_LUMINANCE_ALPHA;
                    *glType_ = GL_UNSIGNED_BYTE;
                    return;
                case UNCOMPRESSED_R5G6B5:
                    *glInternalFormat_ = GL_RGB;
                    *glFormat_ = GL_RGB;
                    *glType_ = GL_UNSIGNED_SHORT_5_6_5;
                    return;
                case UNCOMPRESSED_R8G8B8:
                    *glInternalFormat_ = GL_RGB;
                    *glFormat_ = GL_RGB;
                    *glType_ = GL_UNSIGNED_BYTE;
                    return;
                case UNCOMPRESSED_R5G5B5A1:
                    *glInternalFormat_ = GL_RGBA;
                    *glFormat_ = GL_RGBA;
                    *glType_ = GL_UNSIGNED_SHORT_5_5_5_1;
                    return;
                case UNCOMPRESSED_R4G4B4A4:
                    *glInternalFormat_ = GL_RGBA;
                    *glFormat_ = GL_RGBA;
                    *glType_ = GL_UNSIGNED_SHORT_4_4_4_4;
                    return;
                case UNCOMPRESSED_R8G8B8A8:
                    *glInternalFormat_ = GL_RGBA;
                    *glFormat_ = GL_RGBA;
                    *glType_ = GL_UNSIGNED_BYTE;
                    return;
                case UNCOMPRESSED_R32:
                    // Requires OES_texture_float
                    if (m_featureFlags[EXT_TEX_FLOAT]) {
                        *glInternalFormat_ = GL_LUMINANCE;
                        *glFormat_ = GL_LUMINANCE;
                        *glType_ = GL_FLOAT;
                    } else Console::Fail("PlatformGLAPI", "Format not supported.");
                    return;
                case UNCOMPRESSED_R32G32B32:
                    // Requires OES_texture_float
                    if (m_featureFlags[EXT_TEX_FLOAT]) {
                        *glInternalFormat_ = GL_RGB;
                        *glFormat_ = GL_RGB;
                        *glType_ = GL_FLOAT;
                    }
                    return;
                case UNCOMPRESSED_R32G32B32A32:
                    // Requires OES_texture_float
                    if (m_featureFlags[EXT_TEX_FLOAT]) {
                        *glInternalFormat_ = GL_RGBA;
                        *glFormat_ = GL_RGBA;
                        *glType_ = GL_FLOAT;
                    } else Console::Fail("PlatformGLAPI", "Format not supported.");
                    return;
            }
        }
#endif

#if defined(API_OPENGL_ENABLED)
        // GL specific
        if (!m_GLES2 && !m_GLES3) {
            switch (format_) {
                case UNCOMPRESSED_GRAYSCALE:
                    *glInternalFormat_ = GL_R8;
                    *glFormat_ = GL_RED;
                    *glType_ = GL_UNSIGNED_BYTE;
                    return;
                case UNCOMPRESSED_GRAY_ALPHA:
                    *glInternalFormat_ = GL_RG8;
                    *glFormat_ = GL_RG;
                    *glType_ = GL_UNSIGNED_BYTE;
                    return;
                case UNCOMPRESSED_R5G6B5:
                    *glInternalFormat_ = GL_RGB565;
                    *glFormat_ = GL_RGB;
                    *glType_ = GL_UNSIGNED_SHORT_5_6_5;
                    return;
                case UNCOMPRESSED_R8G8B8:
                    *glInternalFormat_ = GL_RGB8;
                    *glFormat_ = GL_RGB;
                    *glType_ = GL_UNSIGNED_BYTE;
                    return;
                case UNCOMPRESSED_R5G5B5A1:
                    *glInternalFormat_ = GL_RGB5_A1;
                    *glFormat_ = GL_RGBA;
                    *glType_ = GL_UNSIGNED_SHORT_5_5_5_1;
                    return;
                case UNCOMPRESSED_R4G4B4A4:
                    *glInternalFormat_ = GL_RGBA4;
                    *glFormat_ = GL_RGBA;
                    *glType_ = GL_UNSIGNED_SHORT_4_4_4_4;
                    return;
                case UNCOMPRESSED_R8G8B8A8:
                    *glInternalFormat_ = GL_RGBA8;
                    *glFormat_ = GL_RGBA;
                    *glType_ = GL_UNSIGNED_BYTE;
                    return;
                case UNCOMPRESSED_R32:
                    // Requires OES_texture_float
                    if (m_featureFlags[EXT_TEX_FLOAT]) {
                        *glInternalFormat_ = GL_R32F;
                        *glFormat_ = GL_RED;
                        *glType_ = GL_FLOAT;
                    } else Console::Fail("PlatformGLAPI", "Format not supported.");
                    return;
                case UNCOMPRESSED_R32G32B32:
                    // Requires OES_texture_float
                    if (m_featureFlags[EXT_TEX_FLOAT]) {
                        *glInternalFormat_ = GL_RGB32F;
                        *glFormat_ = GL_RGB;
                        *glType_ = GL_FLOAT;
                    } else Console::Fail("PlatformGLAPI", "Format not supported.");
                    return;
                case UNCOMPRESSED_R32G32B32A32:
                    // Requires OES_texture_float
                    if (m_featureFlags[EXT_TEX_FLOAT]) {
                        *glInternalFormat_ = GL_RGBA32F;
                        *glFormat_ = GL_RGBA;
                        *glType_ = GL_FLOAT;
                    } else Console::Fail("PlatformGLAPI", "Format not supported.");
                    return;
            }
        }
#endif

        // General/Common
        switch (format_) {
            case COMPRESSED_DXT1_RGB:
                if (m_featureFlags[EXT_COMP_DXT])
                    *glInternalFormat_ = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return;
            case COMPRESSED_DXT1_RGBA:
                if (m_featureFlags[EXT_COMP_DXT])
                    *glInternalFormat_ = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return;
            case COMPRESSED_DXT3_RGBA:
                if (m_featureFlags[EXT_COMP_DXT])
                    *glInternalFormat_ = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return;
            case COMPRESSED_DXT5_RGBA:
                if (m_featureFlags[EXT_COMP_DXT])
                    *glInternalFormat_ = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return;
            case COMPRESSED_ETC1_RGB:
                if (m_featureFlags[EXT_COMP_ETC1])
                    *glInternalFormat_ = GL_ETC1_RGB8_OES;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return; // NOTE: Requires OpenGL ES 2.0 or OpenGL 4.3
            case COMPRESSED_ETC2_RGB:
                if (m_featureFlags[EXT_COMP_ETC2])
                    *glInternalFormat_ = GL_COMPRESSED_RGB8_ETC2;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return; // NOTE: Requires OpenGL ES 3.0 or OpenGL 4.3
            case COMPRESSED_ETC2_EAC_RGBA:
                if (m_featureFlags[EXT_COMP_ETC2])
                    *glInternalFormat_ = GL_COMPRESSED_RGBA8_ETC2_EAC;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return; // NOTE: Requires OpenGL ES 3.0 or OpenGL 4.3
            case COMPRESSED_PVRT_RGB:
                if (m_featureFlags[EXT_COMP_PVRT])
                    *glInternalFormat_ = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return; // NOTE: Requires PowerVR GPU
            case COMPRESSED_PVRT_RGBA:
                if (m_featureFlags[EXT_COMP_PVRT])
                    *glInternalFormat_ = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return; // NOTE: Requires PowerVR GPU
            case COMPRESSED_ASTC_4x4_RGBA:
                if (m_featureFlags[EXT_COMP_ASTC])
                    *glInternalFormat_ = GL_COMPRESSED_RGBA_ASTC_4x4_KHR;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return; // NOTE: Requires OpenGL ES 3.1 or OpenGL 4.3
            case COMPRESSED_ASTC_8x8_RGBA:
                if (m_featureFlags[EXT_COMP_ASTC])
                    *glInternalFormat_ = GL_COMPRESSED_RGBA_ASTC_8x8_KHR;
                else Console::Fail("PlatformGLAPI", "Format not supported.");
                return; // NOTE: Requires OpenGL ES 3.1 or OpenGL 4.3
            default:
                Console::Fail("PlatformGLAPI", "Format not supported.");
                return;
        }
    }

    int PlatformGLAPI::_calculatePixelDataSize(int width_, int height_, PixelFormat format_) {
        auto bpp = 0;

        switch (format_) {
            case UNCOMPRESSED_GRAYSCALE:
                bpp = 8;
                break;
            case UNCOMPRESSED_GRAY_ALPHA:
            case UNCOMPRESSED_R5G6B5:
            case UNCOMPRESSED_R5G5B5A1:
            case UNCOMPRESSED_R4G4B4A4:
                bpp = 16;
                break;
            case UNCOMPRESSED_R8G8B8A8:
                bpp = 32;
                break;
            case UNCOMPRESSED_R8G8B8:
                bpp = 24;
                break;
            case UNCOMPRESSED_R32:
                bpp = 32;
                break;
            case UNCOMPRESSED_R32G32B32:
                bpp = 32 * 3;
                break;
            case UNCOMPRESSED_R32G32B32A32:
                bpp = 32 * 4;
                break;
            case COMPRESSED_DXT1_RGB:
            case COMPRESSED_DXT1_RGBA:
            case COMPRESSED_ETC1_RGB:
            case COMPRESSED_ETC2_RGB:
            case COMPRESSED_PVRT_RGB:
            case COMPRESSED_PVRT_RGBA:
                bpp = 4;
                break;
            case COMPRESSED_DXT3_RGBA:
            case COMPRESSED_DXT5_RGBA:
            case COMPRESSED_ETC2_EAC_RGBA:
            case COMPRESSED_ASTC_4x4_RGBA:
                bpp = 8;
                break;
            case COMPRESSED_ASTC_8x8_RGBA:
                bpp = 2;
                break;
            default:
                break;
        }

        auto dataSize = width_ * height_ * bpp / 8;  // Total data size in bytes

        // Most compressed formats works on 4x4 blocks,
        // if texture is smaller, minimum dataSize is 8 or 16
        if ((width_ < 4) && (height_ < 4)) {
            if ((format_ >= COMPRESSED_DXT1_RGB) && (format_ < COMPRESSED_DXT3_RGBA)) dataSize = 8;
            else if ((format_ >= COMPRESSED_DXT3_RGBA) && (format_ < COMPRESSED_ASTC_8x8_RGBA)) dataSize = 16;
        }

        return dataSize;
    }

    void PlatformGLAPI::_useVertexLayout(VertexLayout *layout_) {
        // Bind VAO if enabled
        if (m_featureFlags[FEATURE_VAO]) {
            if (m_GLES2)
                glBindVertexArrayOES(layout_->VAO);
            else
                glBindVertexArray(layout_->VAO);
        } else {
            // Bind buffers
            auto vBuf = layout_->getVertexBuffer();
            auto iBuf = layout_->getIndexBuffer();
            bindBuffer(vBuf);
            if (iBuf != nullptr)
                bindBuffer(iBuf);

            // Setup layout
            auto elements = layout_->getElements();
            for (const auto &elm : elements) {
                GLenum type;
                switch (elm.Type) {
                    case VertexLayout::VertexElementType::Float:
                        type = GL_FLOAT;
                        break;
                }

                glEnableVertexAttribArray(elm.ElementIndex);
                glVertexAttribPointer(elm.ElementIndex, elm.Count, type, elm.Normalized ? GL_TRUE : GL_FALSE,
                                      elm.Stride, (GLvoid *) elm.Offset);
            }
        }
    }

    void PlatformGLAPI::_stopVertexLayout(VertexLayout *layout_) {
        if (m_featureFlags[FEATURE_VAO]) {
            // Unbind VAO
            if (m_GLES2)
                glBindVertexArrayOES(0);
            else
                glBindVertexArray(0);
        } else {
            // Unbind buffers
            auto vBuf = layout_->getVertexBuffer();
            auto iBuf = layout_->getIndexBuffer();
            unbindBuffer(vBuf);
            if (iBuf != nullptr)
                unbindBuffer(iBuf);

            // Unset layout
            auto elements = layout_->getElements();
            for (const auto &elm : elements) {
                glDisableVertexAttribArray(elm.ElementIndex);
            }
        }
    }

    void PlatformGLAPI::_setUniform(const ShaderProgram *program_, std::string name_, ShaderDataStructure structure_, const void *data_) {
        // Complete actions based on type.
        switch (structure_.Type) {
            case ShaderDataType::Int:
            case ShaderDataType::UnsignedInt:
            case ShaderDataType::Float:
            case ShaderDataType::Matrix:
                // This could be separated again, but this looks better.
                _writeSimpleUniform(program_, name_.c_str(), structure_.Type, structure_.Count, (char *)data_);
                break;
            case ShaderDataType::Struct: {
                // Write each member
                int offset = 0;
                for (const auto& s : structure_.Members) {
                    _setUniform(program_, name_ + "." + s.Name, s, (char *)data_ + offset);
                    offset += s.getSize();
                }
                break;
            }
            case ShaderDataType::Array: {
                // Write simple data for each bit
                auto sizePerEntry = structure_.Members[0].getSize();
                for (auto i = 0; i < structure_.Count; i++) {
                    _setUniform(program_, name_ + "[" + std::to_string(i) + "]", structure_.Members[0], (char*)data_ + sizePerEntry * i);
                }
                break;
            }
        }
    }

    void PlatformGLAPI::_writeSimpleUniform(const ShaderProgram *program_, const char *name_, ShaderDataType type_, int count_, const void *data_) {
        auto loc = glGetUniformLocation(program_->ID, name_);
        switch (type_) {
            case ShaderDataType::Int: {
                auto ints = (int *)data_;
                switch (count_) {
                    case 1:
                        glUniform1i(loc, *ints);
                        break;
                    case 2:
                        glUniform2i(loc, *ints, *(ints + 1));
                        break;
                    case 3:
                        glUniform3i(loc, *ints, *(ints + 1), *(ints + 2));
                        break;
                    case 4:
                        glUniform4i(loc, *ints, *(ints + 1), *(ints + 2), *(ints + 3));
                        break;
                    default:
                        Console::Fail("PlatformGLAPI", "Invalid count for int type.");
                }
                break;
            }
            case ShaderDataType::UnsignedInt: {
                auto uints = (unsigned int *)data_;
                switch (count_) {
                    case 1:
                        glUniform1ui(loc, *uints);
                        break;
                    case 2:
                        glUniform2ui(loc, *uints, *(uints + 1));
                        break;
                    case 3:
                        glUniform3ui(loc, *uints, *(uints + 1), *(uints + 2));
                        break;
                    case 4:
                        glUniform4ui(loc, *uints, *(uints + 1), *(uints + 2), *(uints + 3));
                        break;
                    default:
                        Console::Fail("PlatformGLAPI", "Invalid count for uint type.");
                }
                break;
            }
            case ShaderDataType::Float: {
                auto floats = (float *)data_;
                switch (count_) {
                    case 1:
                        glUniform1f(loc, *floats);
                        break;
                    case 2:
                        glUniform2f(loc, *floats, *(floats + 1));
                        break;
                    case 3:
                        glUniform3f(loc, *floats, *(floats + 1), *(floats + 2));
                        break;
                    case 4: {
                        auto a = *floats;
                        auto b = *(floats + 1);
                        auto c = *(floats + 2);
                        auto d = *(floats + 3);
                        glUniform4f(loc, *floats, *(floats + 1), *(floats + 2), *(floats + 3));
                        break;
                    }
                    default:
                        Console::Fail("PlatformGLAPI", "Invalid count for float type.");
                }
                break;
            }
            case ShaderDataType::Matrix:
                glUniformMatrix4fv(loc, count_, GL_FALSE, (float*)data_);
                break;
            default: Console::Fail("PlatformGLAPI", "Non simple passed to simple write.");
        }
    }

    PlatformGLAPI::PlatformGLAPI(GraphicsDevice *graphicsDevice_)
            : PlatformGraphicsAPI(graphicsDevice_) {
        // Clear feature flags
        for (auto i = 0; i < EXT_TEX_NPOT; i++) m_featureFlags[i] = false;

#if defined(EGL)
        // Init EGL
        EGLint samples = 0;
        EGLint sampleBuffer = 0;

        // TODO: Get window config.
        //if (config_.MSAA_4X) {
#if !defined(PLATFORM_UWP) // Seems to crash UWP, so don't
            //samples = 4;
            //sampleBuffer = 1;
#endif
        //}

        const EGLint framebufferAttribs[] = {
            // Type of context support -> Required on RPI?
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,

            // RED color bit depth (alternative: 5)
            EGL_RED_SIZE, 8,

            // GREEN color bit depth (alternative: 6)
            EGL_GREEN_SIZE, 8,

            // BLUE color bit depth (alternative: 5)
            EGL_BLUE_SIZE, 8,

            // ALPHA bit depth (required for transparent framebuffer)
            EGL_ALPHA_SIZE, 8,

            // Depth buffer size (Required to use Depth testing!)
            EGL_DEPTH_SIZE, 16,

            // Stencil buffer size
            EGL_STENCIL_SIZE, 8,

            // Activate MSAA
            EGL_SAMPLE_BUFFERS, sampleBuffer,

            // 4x Antialiasing if activated (Free on MALI GPUs)
            EGL_SAMPLES, samples,

            // No idea what this does
            EGL_NONE
        };

        auto major = GraphicsDevice::GetTargetAPIMajorVersion();
        auto minor = GraphicsDevice::GetTargetAPIMinorVersion();

        const EGLint contextAttribs[] = {
            //EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_CONTEXT_MAJOR_VERSION, major,
            EGL_CONTEXT_MINOR_VERSION, minor,
            EGL_NONE
        };

#if defined(PLATFORM_UWP)
        const EGLint surfaceAttributes[] = {
            // EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER is part of the same optimization as EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER (see above).
            // If you have compilation issues with it then please update your Visual Studio templates.
            EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_NONE
        };

        const EGLint defaultDisplayAttributes[] = {
            // These are the default display attributes, used to request ANGLE's D3D11 renderer.
            // eglInitialize will only succeed with these attributes if the hardware supports D3D11 Feature Level 10_0+.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,

            // EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER is an optimization that can have large performance benefits on mobile devices.
            // Its syntax is subject to change, though. Please update your Visual Studio templates if you experience compilation issues with it.
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,

            // EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE is an option that enables ANGLE to automatically call
            // the IDXGIDevice3::Trim method on behalf of the application when it gets suspended.
            // Calling IDXGIDevice3::Trim when an application is suspended is a Windows Store application certification requirement.
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        const EGLint fl9_3DisplayAttributes[] = {
            // These can be used to request ANGLE's D3D11 renderer, with D3D11 Feature Level 9_3.
            // These attributes are used if the call to eglInitialize fails with the default display attributes.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE, 9,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE, 3,
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        const EGLint warpDisplayAttributes[] = {
            // These attributes can be used to request D3D11 WARP.
            // They are used if eglInitialize fails with both the default display attributes and the 9_3 display attributes.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_DEVICE_TYPE_WARP_ANGLE,
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        EGLConfig config = nullptr;

        // eglGetPlatformDisplayEXT is an alternative to eglGetDisplay. It allows us to pass in Display attributes, used to configure D3D11.
        PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)(eglGetProcAddress("eglGetPlatformDisplayEXT"));
        if (!eglGetPlatformDisplayEXT) {
            Console::Fail("PlatformGLAPI", "Failed to get function eglGetPlatformDisplayEXT");
        }

        // To initialize the Display, we make three sets of calls to eglGetPlatformDisplayEXT and eglInitialize, with varying
        // parameters passed to eglGetPlatformDisplayEXT:
        // 1) The first calls uses "defaultDisplayAttributes" as a parameter. This corresponds to D3D11 Feature Level 10_0+.
        // 2) If eglInitialize fails for step 1 (e.g. because 10_0+ isn't supported by the default GPU), then we try again
        //    using "fl9_3DisplayAttributes". This corresponds to D3D11 Feature Level 9_3.
        // 3) If eglInitialize fails for step 2 (e.g. because 9_3+ isn't supported by the default GPU), then we try again
        //    using "warpDisplayAttributes".  This corresponds to D3D11 Feature Level 11_0 on WARP, a D3D11 software rasterizer.

        // This tries to initialize EGL to D3D11 Feature Level 10_0+. See above comment for details.
        m_display = (void*)eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, defaultDisplayAttributes);
        if (m_display == EGL_NO_DISPLAY) {
            Console::Fail("PlatformGLAPI", "Failed to initialize EGL Display");
        }

        if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
            // This tries to initialize EGL to D3D11 Feature Level 9_3, if 10_0+ is unavailable (e.g. on some mobile devices).
            m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, fl9_3DisplayAttributes);
            if (m_display == EGL_NO_DISPLAY) {
                Console::Fail("PlatformGLAPI", "Failed to initialize EGL Display");
            }

            if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
                // This initializes EGL to D3D11 Feature Level 11_0 on WARP, if 9_3+ is unavailable on the default GPU.
                m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, warpDisplayAttributes);
                if (m_display == EGL_NO_DISPLAY) {
                    Console::Fail("PlatformGLAPI", "Failed to initialize EGL Display");
                }

                if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
                    // If all of the calls to eglInitialize returned EGL_FALSE then an error has occurred.
                    Console::Fail("PlatformGLAPI", "Failed to initialize EGL");
                }
            }
        }

        EGLint numConfigs = 0;
        if ((eglChooseConfig(m_display, framebufferAttribs, &config, 1, &numConfigs) == EGL_FALSE) || (numConfigs == 0)) {
            Console::Fail("PlatformGLAPI", "Failed to choose first EGLConfig");
        }

        // Create a PropertySet and initialize with the EGLNativeWindowType.
        PropertySet^ surfaceCreationProperties = ref new PropertySet();
        surfaceCreationProperties->Insert(ref new String(EGLNativeWindowTypeProperty), CoreWindow::GetForCurrentThread());     // CoreWindow^ window

        // Create surface
        m_surface = eglCreateWindowSurface(m_display, config, (EGLNativeWindowType)CoreWindow::GetForCurrentThread(), surfaceAttributes);

        if (m_surface == EGL_NO_SURFACE) {
            Console::Fail("PlatformGLAPI", "Failed to create EGL fullscreen surface");
        }

        m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
        if (m_context == EGL_NO_CONTEXT) {
            Console::Fail("PlatformGLAPI", "Failed to create EGL context.");
        }
#else
        // For future GLES2 platforms
        EGLint numConfigs;

        // Get an EGL display connection
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_display == EGL_NO_DISPLAY) {
            throw std::runtime_error("Failed to initialize EGL display");
        }

        // Initialize the EGL display connection
        if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
            // If all of the calls to eglInitialize returned EGL_FALSE then an error has occurred.
            throw std::runtime_error("Failed to initialize EGL");
        }

        // Get an appropriate EGL framebuffer configuration
        eglChooseConfig(m_display, framebufferAttribs, &config, 1, &numConfigs);

        // Set rendering API
        eglBindAPI(EGL_OPENGL_ES_API);

        // Create an EGL rendering context
        m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
        if (m_context == EGL_NO_CONTEXT) {
            throw std::runtime_error("Failed to create EGL context");
        }

        // TODO: For other EGL platforms, initialize window surface
#endif
#endif

        // Make the attached window current so our GL context is active
#if !defined(EGL) // EGL uses this API, which is still being created. TODO: Make this into an Init function instead to tidy this
        m_graphicsDevice->getWindow()->makeCurrent();
#else
        MakeEGLCurrent();
#endif

#if defined(GLAD)
        // Init GLAD loader
        auto status = false;

        if (GraphicsDevice::GetTargetAPI() == GraphicsAPI::OpenGLES) {
#if defined(GLFW)
            status = gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);
#elif defined(EGL)
            status = gladLoadGLES2Loader((GLADloadproc) eglGetProcAddress);
#endif
        } else {
#if defined(GLFW)
            status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
#endif
        }

        if (!status) {
            // Terminate, we can't load
#if defined(GLFW)
            glfwTerminate();
#endif
            Console::Fail("PlatformGLAPI", "Failed to init GLAD.");
        }
        Console::Notice("PlatformGLAPI", "Successfully initialized GLAD.");
#endif

        // Determine if we're running GLES
#if !defined(GLAD)
        if (GraphicsDevice::GetTargetAPI() == GraphicsAPI::OpenGLES) {
            auto major = GraphicsDevice::GetTargetAPIMajorVersion();
            m_GLES2 = major == 2;
            m_GLES3 = major == 3;
        }
#else
        m_GLES2 =
                GLAD_GL_ES_VERSION_2_0 && !GLAD_GL_ES_VERSION_3_0 && !GLAD_GL_ES_VERSION_3_1 && !GLAD_GL_ES_VERSION_3_2;
        m_GLES3 = GLAD_GL_ES_VERSION_3_0;
#endif

        // Setup default extensions
#if defined(GLAD)
        if (GLAD_GL_VERSION_3_0) {
            m_featureFlags[FEATURE_VAO] = true;
            m_featureFlags[EXT_TEX_NPOT] = true;
            m_featureFlags[EXT_TEX_FLOAT] = true;
            m_featureFlags[EXT_TEX_DEPTH] = true;
        }
#endif
        // GLES3 defaults
        if (m_GLES3) {
            m_featureFlags[FEATURE_VAO] = true;
            // TODO: Add any others.
        }

        // Build extension list
        int numExt = 0;
        const char **extList = nullptr;
        char *toDelete = nullptr;
        if (m_GLES2) {
            // Create array
            extList = new const char *[512];

            const char *extensions = (const char *) glGetString(GL_EXTENSIONS);

            int len = strlen(extensions) + 1;
            toDelete = (char *) new char[len];
            strcpy(toDelete, extensions);

            extList[numExt] = toDelete;

            for (int i = 0; i < len; i++) {
                if (toDelete[i] == ' ') {
                    toDelete[i] = '\0';
                    numExt++;
                    extList[numExt] = &toDelete[i + 1];
                }
            }
        } else {
#if defined(GLAD)
            // Get extension count
            glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

            // Get extensions
            extList = new const char *[numExt];
            for (auto i = 0; i < numExt; i++) extList[i] = (char *) glGetStringi(GL_EXTENSIONS, i);
#endif
        }

        // Process extensions
        for (auto i = 0; i < numExt; i++) {
            // GLES 2 extensions
            if (m_GLES2) {
                // Check for VAO support
                if (strcmp(extList[i], "GL_OES_vertex_array_object") == 0) {
#if defined(GLAD)
#if defined(GLFW)
                    // GLFW does not provide the OES methods, try to find them.
                    glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSPROC) glfwGetProcAddress("glGenVertexArraysOES");
                    glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYPROC) glfwGetProcAddress("glBindVertexArrayOES");
                    glDeleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSPROC) glfwGetProcAddress(
                            "glDeleteVertexArraysOES");
#endif
                    if ((glGenVertexArraysOES != nullptr) && (glBindVertexArrayOES != nullptr) &&
                        (glDeleteVertexArraysOES != nullptr))
                        m_featureFlags[FEATURE_VAO] = true;
#else
                    // gl2ext.h provides the functions already.
                    m_featureFlags[FEATURE_VAO] = true;
#endif
                }
            }

            // GLES 2/3 Specific extensions
            if (m_GLES2 || m_GLES3) {
                // Check NPOT textures support
                if (strcmp(extList[i], "GL_OES_texture_npot") == 0) m_featureFlags[EXT_TEX_NPOT] = true;

                // Check texture float support
                if (strcmp(extList[i], "GL_OES_texture_float") == 0) m_featureFlags[EXT_TEX_FLOAT] = true;

                // Check depth texture support
                if ((strcmp(extList[i], "GL_OES_depth_texture") == 0) ||
                    (strcmp(extList[i], "GL_WEBGL_depth_texture") == 0))
                    m_featureFlags[EXT_TEX_DEPTH] = true;

                if (strcmp(extList[i], "GL_OES_depth24") == 0) m_maxDepthBits = 24;
                if (strcmp(extList[i], "GL_OES_depth32") == 0) m_maxDepthBits = 32;
            }

            // DDS texture compression support
            if ((strcmp(extList[i], "GL_EXT_texture_compression_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBKIT_WEBGL_compressed_texture_s3tc") == 0))
                m_featureFlags[EXT_COMP_DXT] = true;

            // ETC1 texture compression support
            if ((strcmp(extList[i], "GL_OES_compressed_ETC1_RGB8_texture") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_etc1") == 0))
                m_featureFlags[EXT_COMP_ETC1] = true;

            // ETC2/EAC texture compression support
            if (strcmp(extList[i], "GL_ARB_ES3_compatibility") == 0) m_featureFlags[EXT_COMP_ETC2] = true;

            // PVR texture compression support
            if (strcmp(extList[i], "GL_IMG_texture_compression_pvrtc") == 0) m_featureFlags[EXT_COMP_PVRT] = true;

            // ASTC texture compression support
            if (strcmp(extList[i], "GL_KHR_texture_compression_astc_hdr") == 0) m_featureFlags[EXT_COMP_ASTC] = true;

            // Anisotropic texture filter
            if (strcmp(extList[i], (const char *) "GL_EXT_texture_filter_anisotropic") == 0) {
                m_featureFlags[EXT_ANISOTROPIC_TEXTURE_FILTER] = true;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropicLevel);
            }

            // Clamp mirror wrap mode supported
            if (strcmp(extList[i], (const char *) "GL_EXT_texture_mirror_clamp") == 0)
                m_featureFlags[EXT_TEX_MIRROR_CLAMP] = true;

            // TODO: Determine m_maxDepthBits for OpenGL
        }

        // Delete if needed
        delete[] toDelete;
    }

    PlatformGLAPI::~PlatformGLAPI() {
#if defined(EGL)
        // Close surface, context and Display
        if (m_display != EGL_NO_DISPLAY) {
            eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

            if (m_surface != EGL_NO_SURFACE) {
                eglDestroySurface(m_display, m_surface);
                m_surface = EGL_NO_SURFACE;
            }

            if (m_context != EGL_NO_CONTEXT) {
                eglDestroyContext(m_display, m_context);
                m_context = EGL_NO_CONTEXT;
            }

            eglTerminate(m_display);
            m_display = EGL_NO_DISPLAY;
        }
#endif
    }

    void PlatformGLAPI::configureViewport(int x_, int y_, int width_, int height_) {
        glViewport(x_, y_, width_, height_);
    }

    void PlatformGLAPI::clear(const Color &color_) {
        glClearColor(color_.R, color_.G, color_.B, color_.A);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void PlatformGLAPI::createTexture(Texture2D *texture_, unsigned char *data_) {
        // Create texture
        texture_->ID = 0;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &texture_->ID);

        // Bind
        glBindTexture(GL_TEXTURE_2D, texture_->ID);

        // Generate mipmaps TODO: Just generate mipmaps through OGL instead?
        int mipWidth = texture_->Width;
        int mipHeight = texture_->Height;
        int mipOffset = 0;

        auto format = texture_->getFormat();
        unsigned int glInternalFormat = 0, glFormat = 0, glType = 0;
        _getTextureFormats(format, &glInternalFormat, &glFormat, &glType);

        for (int i = 0; i < texture_->getMipmapCount(); i++) {
            unsigned int mipSize = _calculatePixelDataSize(mipWidth, mipHeight, format);

            if (glInternalFormat != -1) {
                if (format < COMPRESSED_DXT1_RGB)
                    glTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, glFormat, glType,
                                 (unsigned char *) data_ + mipOffset);
                else
                    glCompressedTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, mipSize,
                                           (unsigned char *) data_ + mipOffset);

#if defined(API_OPENGL_ENABLED)
                if (!m_GLES2 && !m_GLES3) {
                    if (format == UNCOMPRESSED_GRAYSCALE) {
                        GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
                        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
                    } else if (format == UNCOMPRESSED_GRAY_ALPHA) {
#if defined(GLAD)
                        if (GLAD_GL_VERSION_3_0) {
                            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
                            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
                        } else if (GLAD_GL_VERSION_2_0) {
                            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ALPHA};
                            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
                        }
#endif
                    }
                }
#endif
            }

            // Set default filters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
    }

    void PlatformGLAPI::deleteTexture(Texture2D *texture_) {
        glDeleteTextures(1, &texture_->ID);
        texture_->ID = 0;
    }

    void PlatformGLAPI::bindTexture(Texture2D *texture_) {
        if (texture_ != m_currentTexture) {
            glBindTexture(GL_TEXTURE_2D, texture_ != nullptr ? texture_->ID : 0);
            m_currentTexture = texture_;
        }
    }

    void PlatformGLAPI::setTextureFilterMode(Texture2D *texture_, TextureFilterMode mode_) {
        // Bind
        bindTexture(texture_);

        // Get variables
        auto mipmapCount = texture_->getMipmapCount();

        // Set filter mode
        switch (mode_) {
            case TextureFilterMode::Point:
                if (mipmapCount > 1) {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                } else {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }
                break;
            case TextureFilterMode::Bilinear:
                if (mipmapCount > 1) {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                } else {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }
                break;
            case TextureFilterMode::Trilinear:
                if (mipmapCount > 1) {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                } else {
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }
                break;
            case TextureFilterMode::Anisotropic_4X:
                if (4 < m_maxAnisotropicLevel) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
                else
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropicLevel);
                break;
            case TextureFilterMode::Anisotropic_8X:
                if (8 < m_maxAnisotropicLevel) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
                else
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropicLevel);
                break;
            case TextureFilterMode::Anisotropic_16X:
                if (16 < m_maxAnisotropicLevel) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
                else
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropicLevel);
                break;
        }
    }

    void PlatformGLAPI::setTextureWrapMode(Texture2D *texture_, TextureWrapMode mode_) {
        // Bind
        bindTexture(texture_);

        // Set wrap mode
        switch (mode_) {
            case TextureWrapMode::Repeat:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;
            case TextureWrapMode::Clamp:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                break;
            case TextureWrapMode::MirrorRepeat:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                break;
        }
    }

    bool PlatformGLAPI::isTextureValid(const Texture2D *texture_) {
        return texture_->ID > 0;
    }

    bool PlatformGLAPI::compareTextures(const Texture2D *a_, const Texture2D *b_) {
        return a_->ID == b_->ID;
    }

    bool PlatformGLAPI::createRenderTarget(RenderTarget *renderTarget_) {
        // Set initial IDs
        renderTarget_->ID[0] = 0;
        renderTarget_->ID[1] = 0;

        // Depth buffer
        glGenRenderbuffers(1, &renderTarget_->ID[1]);
        glBindRenderbuffer(GL_RENDERBUFFER, renderTarget_->ID[1]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, renderTarget_->Width, renderTarget_->Height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Create FBO
        glGenFramebuffers(1, &renderTarget_->ID[0]);

        // Bind
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget_->ID[0]);

        // Set depth and color attachment
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderTarget_->ID[1]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTarget_->getTexture()->ID, 0);

        // Check framebuffer status
        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            Console::Error("PlatformGLAPI", "Failed to create framebuffer.");

            // Unbind
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Delete depth buffer
            glDeleteRenderbuffers(1, &renderTarget_->ID[1]);
            renderTarget_->ID[1] = 0;

            // Delete framebuffer
            glDeleteFramebuffers(1, &renderTarget_->ID[0]);
            renderTarget_->ID[0] = 0;

            return false;
        }

        Console::Notice("PlatformGLAPI", "Successfully created framebuffer with ID %i", renderTarget_->ID[0]);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return true;
    }

    void PlatformGLAPI::deleteRenderTarget(RenderTarget *renderTarget_) {
        // Delete depth buffer
        if (renderTarget_->ID[1] > 0) {
            glDeleteRenderbuffers(1, &renderTarget_->ID[1]);
            renderTarget_->ID[1] = 0;
        }

        // Delete framebuffer
        if (renderTarget_->ID[0] > 0) {
            glDeleteFramebuffers(1, &renderTarget_->ID[0]);
            renderTarget_->ID[0] = 0;
        }
    }

    void PlatformGLAPI::bindRenderTarget(RenderTarget *renderTarget_) {
        if (renderTarget_ != m_currentRenderTarget) {
            glBindFramebuffer(GL_FRAMEBUFFER, renderTarget_ != nullptr ? renderTarget_->ID[0] : 0);
            m_currentRenderTarget = renderTarget_;
        }
    }

    bool PlatformGLAPI::isRenderTargetValid(const RenderTarget *renderTarget_) {
        return renderTarget_->ID[0] > 0;
    }

    bool PlatformGLAPI::compareRenderTargets(const RenderTarget *a_, const RenderTarget *b_) {
        return a_->ID[0] == b_->ID[0] && a_->ID[1] == b_->ID[1];
    }

    void PlatformGLAPI::createShader(Shader *shader_, void *sourceData_) {
        // Get shader type
        GLenum type;
        switch (shader_->Type) {
            case ShaderType::Vertex:
                type = GL_VERTEX_SHADER;
                break;
            case ShaderType::Fragment:
                type = GL_FRAGMENT_SHADER;
                break;
        }

        // Create shader object
        shader_->ID = glCreateShader(type);

        // Set source
        const char *src = (const char *) sourceData_;
        glShaderSource(shader_->ID, 1, &src, nullptr);

        // Compile
        glCompileShader(shader_->ID);

        // Test compile status
        GLint compiled;
        glGetShaderiv(shader_->ID, GL_COMPILE_STATUS, &compiled);

        if (compiled != GL_TRUE) // error in compilation occurred
        {
            glDeleteShader(shader_->ID);
            Console::Fail("PlatformGLAPI", "Failed to compile shader.");
        }
    }

    void PlatformGLAPI::deleteShader(Shader *shader_) {
        // Delete
        glDeleteShader(shader_->ID);
        shader_->ID = 0;
    }

    bool PlatformGLAPI::isShaderValid(const Shader *shader_) {
        return shader_->ID > 0;
    }

    void PlatformGLAPI::createShaderProgram(ShaderProgram *program_) {
        // Create
        program_->ID = glCreateProgram();

        // Attach shaders
        glAttachShader(program_->ID, program_->VertexShader->ID);
        glAttachShader(program_->ID, program_->FragmentShader->ID);

        // TODO: Develop a better system for this. (One that is universal across shader languages)
        //  Maybe do this as part of VertexLayout, whenever a shader or a vertex layout is bound, link these attrib locations up?
        // Bind attrib locations
        glBindAttribLocation(program_->ID, 0, "NG_VertexPos");
        glBindAttribLocation(program_->ID, 1, "NG_VertexColor");
        glBindAttribLocation(program_->ID, 2, "NG_VertexTexCoord");

        // Link
        glLinkProgram(program_->ID);

        // Get link status
        int linked = GL_TRUE;
        glGetProgramiv(program_->ID, GL_LINK_STATUS, &linked);

        if (linked != GL_TRUE) {
            glDeleteProgram(program_->ID);
            Console::Fail("PlatformGLAPI", "Failed to link shader program.");
            return;
        }
    }

    void PlatformGLAPI::deleteShaderProgram(ShaderProgram *program_) {
        glDeleteProgram(program_->ID);
        program_->ID = 0;
    }

    void PlatformGLAPI::bindShaderProgram(const ShaderProgram *program_) {
        glUseProgram(program_ != nullptr ? program_->ID : 0);
    }

    void PlatformGLAPI::bindShaderProgramState(ShaderProgramState *state_) {
        // Bind the program
        bindShaderProgram(state_->AttachedProgram);

        // Apply each uniform
        for (const auto &uniform : state_->AttachedProgram->getUniforms()) {
            _setUniform(state_->AttachedProgram, uniform.Name, uniform, state_->getUniform(uniform.Name));
        }
    }

    bool PlatformGLAPI::isShaderProgramValid(const ShaderProgram *program_) {
        return program_->ID > 0 && isShaderValid(program_->VertexShader) && isShaderValid(program_->FragmentShader);
    }

    void PlatformGLAPI::bindBuffer(Buffer *buffer_) {
        // Check not null
        if (buffer_ == nullptr)
            Console::Fail("PlatformGLAPI", "Cannot bind null buffer.");

        // Bind buffer
        switch (buffer_->Type) {
            case BufferType::Vertex:
                glBindBuffer(GL_ARRAY_BUFFER, buffer_->ID);
                break;
            case BufferType::Index:
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_->ID);
                break;
        }
    }

    void PlatformGLAPI::unbindBuffer(Buffer *buffer_) {
        // Check not null
        if (buffer_ == nullptr)
            Console::Fail("PlatformGLAPI", "Cannot unbind null buffer.");

        // Bind buffer
        switch (buffer_->Type) {
            case BufferType::Vertex:
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                break;
            case BufferType::Index:
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                break;
        }
    }

    void PlatformGLAPI::createBuffer(Buffer *buffer_) {
        // Create buffer
        buffer_->ID = 0;
        glGenBuffers(1, &buffer_->ID);

        // Bind
        bindBuffer(buffer_);
    }

    void PlatformGLAPI::deleteBuffer(Buffer *buffer_) {
        // Unbind buffer
        unbindBuffer(buffer_);

        // Delete
        glDeleteBuffers(1, &buffer_->ID);
        buffer_->ID = 0;
    }

    void PlatformGLAPI::writeBuffer(Buffer *buffer_, void *data_, int count_, int size_, bool update_) {
        // Bind
        bindBuffer(buffer_);

        // Get type
        GLenum bufType = 0;
        switch (buffer_->Type) {
            case BufferType::Vertex:
                bufType = GL_ARRAY_BUFFER;
                break;
            case BufferType::Index:
                bufType = GL_ELEMENT_ARRAY_BUFFER;
                break;
        }

        // Get usage
        GLenum bufUsage = 0;
        switch (buffer_->Usage) {
            case BufferUsage::Static:
                bufUsage = GL_STATIC_DRAW;
                break;
            case BufferUsage::Dynamic:
                bufUsage = GL_DYNAMIC_DRAW;
                break;
        }

        // Write
        if (update_) {
            glBufferSubData(bufType, 0, size_ * count_, data_);
        } else {
            glBufferData(bufType, size_ * count_, data_, bufUsage);
        }
    }

    void PlatformGLAPI::createVertexLayout(VertexLayout *layout_) {
        // Create VAO if enabled
        if (m_featureFlags[FEATURE_VAO]) {
            layout_->VAO = 0;
            if (m_GLES2)
                glGenVertexArraysOES(1, &layout_->VAO);
            else
                glGenVertexArrays(1, &layout_->VAO);
        }
    }

    void PlatformGLAPI::deleteVertexLayout(VertexLayout *layout_) {
        // Delete VAO if enabled
        if (m_featureFlags[FEATURE_VAO]) {
            if (m_GLES2)
                glDeleteVertexArraysOES(1, &layout_->VAO);
            else
                glDeleteVertexArrays(1, &layout_->VAO);
            layout_->VAO = 0;
        }
    }

    void PlatformGLAPI::configureVertexLayout(VertexLayout *layout_) {
        // Get buffers
        auto vBuf = layout_->getVertexBuffer();
        auto iBuf = layout_->getIndexBuffer();

        if (m_featureFlags[FEATURE_VAO]) {
            // Unbind any existing buffers
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // Bind VAO
            if (m_GLES2)
                glBindVertexArrayOES(layout_->VAO);
            else
                glBindVertexArray(layout_->VAO);

            // Bind buffers
            bindBuffer(vBuf);
            if (iBuf != nullptr)
                bindBuffer(iBuf);
        }

        auto elements = layout_->getElements();
        for (const auto &elm : elements) {
            GLenum type;
            switch (elm.Type) {
                case VertexLayout::VertexElementType::Float:
                    type = GL_FLOAT;
                    break;
            }

            glEnableVertexAttribArray(elm.ElementIndex);
            glVertexAttribPointer(elm.ElementIndex, elm.Count, type, elm.Normalized ? GL_TRUE : GL_FALSE, elm.Stride,
                                  (GLvoid *) elm.Offset);
        }

        // Unbind VAO then buffers (if present)
        if (m_featureFlags[FEATURE_VAO]) {
            if (m_GLES2)
                glBindVertexArrayOES(0);
            else
                glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    void PlatformGLAPI::prepareFor2D() {
        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void PlatformGLAPI::draw(int count_, int start_) {
        glDrawArrays(GL_TRIANGLES, start_, count_);
    }

    void PlatformGLAPI::drawIndexed(int count_, int start_) {
        glDrawElements(GL_TRIANGLES, count_, GL_UNSIGNED_SHORT, (GLvoid *) start_);
    }

#if defined(EGL)
    // TODO: Add an abstraction kind of thing for Window so this kind of context management can be handled externally?
    EGLContext PlatformGLAPI::GetEGLContext() {
        return m_context;
    }

    EGLDisplay PlatformGLAPI::GetEGLDisplay() {
        return m_display;
    }

    EGLSurface PlatformGLAPI::GetEGLSurface() {
        return m_surface;
    }

    void PlatformGLAPI::QueryEGLSurfaceSize(int *width_, int *height_) {
        eglQuerySurface(m_display, m_surface, EGL_WIDTH, width_);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT, height_);
    }

    void PlatformGLAPI::MakeEGLCurrent() {
        if (eglMakeCurrent(m_display, m_surface, m_surface, m_context) == EGL_FALSE)
            Console::Fail("PlatformGLAPI", "Failed to make EGL context current.");
    }

    void PlatformGLAPI::SwapEGLBuffers() {
        eglSwapBuffers(m_display, m_surface);
    }

    void PlatformGLAPI::SetEGLSwapInterval(int interval_) {
        eglSwapInterval(m_display, interval_);
    }
#endif
}

#endif