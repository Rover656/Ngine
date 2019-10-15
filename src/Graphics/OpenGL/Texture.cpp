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

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#include "Texture.h"

// Platform specifics
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
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

#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT       0x84FE
#endif

#include "OpenGL.h"

namespace NerdThings::Ngine::Graphics::OpenGL {
    GLTexture::GLTexture() {}

    GLTexture::GLTexture(unsigned int width_, unsigned int height_, void *data_, int mipmapCount_,
                         GLPixelFormat format_) {
        // Check dimensions
        if (width_ <= 0 || height_ <= 0) {
            ConsoleMessage("Texture was given invalid dimensions of " + std::to_string(width_) + ", " + std::to_string(height_) + ".", "ERROR", "GLTexture");
            return;
        }

        // Unbind any bound textures
        glBindTexture(GL_TEXTURE_2D, 0);

        // Set mipmap count and format
        MipmapCount = mipmapCount_;
        _Format = format_;

        // Check format support
        if ((!GL::TexCompDXTSupported) && ((format_ == COMPRESSED_DXT1_RGB) || (format_ == COMPRESSED_DXT1_RGBA) ||
                                       (format_ == COMPRESSED_DXT3_RGBA) || (format_ == COMPRESSED_DXT5_RGBA)))
        {
            throw std::runtime_error("DXT compressed texture format not supported");
        }
        if ((!GL::TexCompETC1Supported) && (format_ == COMPRESSED_ETC1_RGB))
        {
            throw std::runtime_error("ETC1 compressed texture format not supported");
        }

        if ((!GL::TexCompETC2Supported) && ((format_ == COMPRESSED_ETC2_RGB) || (format_ == COMPRESSED_ETC2_EAC_RGBA)))
        {
            throw std::runtime_error("ETC2 compressed texture format not supported");
        }

        if ((!GL::TexCompPVRTSupported) && ((format_ == COMPRESSED_PVRT_RGB) || (format_ == COMPRESSED_PVRT_RGBA)))
        {
            throw std::runtime_error("PVRT compressed texture format not supported");
        }

        if ((!GL::TexCompASTCSupported) && ((format_ == COMPRESSED_ASTC_4x4_RGBA) || (format_ == COMPRESSED_ASTC_8x8_RGBA)))
        {
            throw std::runtime_error("ASTC compressed texture format not supported");
        }

        // Create texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &ID);
        ConsoleMessage("Texture with ID " + std::to_string(ID) + " is being created with width and height: " + std::to_string(width_) + ", " + std::to_string(height_), "NOTICE", "GLTexture");

        // Bind
        Bind();

        // Generate mipmaps
        int mipWidth = width_;
        int mipHeight = height_;
        int mipOffset = 0;

        unsigned int glInternalFormat, glFormat, glType;
        GL::GetGLTextureFormats(format_, &glInternalFormat, &glFormat, &glType);

        for (int i = 0; i < mipmapCount_; i++) {
            unsigned int mipSize = GetPixelDataSize(mipWidth, mipHeight, format_);

            if (glInternalFormat != -1) {
                if (format_ < COMPRESSED_DXT1_RGB) glTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, glFormat, glType, (unsigned char *)data_ + mipOffset);
                else glCompressedTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, mipSize, (unsigned char *)data_ + mipOffset);

#if defined(GRAPHICS_OPENGL33)
                if (format_ == UNCOMPRESSED_GRAYSCALE)
                {
                    GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
                    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
                }
                else if (format_ == UNCOMPRESSED_GRAY_ALPHA)
                {
#if defined(GRAPHICS_OPENGL21)
                    GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ALPHA };
#elif defined(GRAPHICS_OPENGL33)
                    GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
#endif
                    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
                }
#endif
            }

            mipWidth /= 2;
            mipHeight /= 2;
            mipOffset += mipSize;

            // Security check for NPOT textures
            if (mipWidth < 1) mipWidth = 1;
            if (mipHeight < 1) mipHeight = 1;
        }

        // Init parameters
#if defined(GRAPHICS_OPENGLES2)
    // NOTE: OpenGL ES 2.0 with no GL_OES_texture_npot support (i.e. WebGL) has limited NPOT support, so CLAMP_TO_EDGE must be used
    if (GL::TexNPOTSupported)
    {
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        SetParameter(TEXPARAM_WRAP_S, WRAP_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        SetParameter(TEXPARAM_WRAP_T, WRAP_REPEAT);
    }
    else
    {
        // NOTE: If using negative texture coordinates (LoadOBJ()), it does not work!

        // Clamp X
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        SetParameter(TEXPARAM_WRAP_S, WRAP_CLAMP);

        // Clamp Y
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        SetParameter(TEXPARAM_WRAP_T, WRAP_CLAMP);
    }
#else
        SetParameter(TEXPARAM_WRAP_S, WRAP_REPEAT);
        SetParameter(TEXPARAM_WRAP_T, WRAP_REPEAT);
#endif

#if defined(GRAPHICS_OPENGL33)
        if (mipmapCount_ > 1)
        {
            SetParameter(TEXPARAM_MAG_FILTER, FILTER_FUNC_LINEAR);
            SetParameter(TEXPARAM_MIN_FILTER, FILTER_FUNC_MIP_LINEAR);
        } else
#endif
        {
            // By default we filter by nearest. TODO: Look at pros and cons of this
            SetParameter(TEXPARAM_MAG_FILTER, FILTER_FUNC_NEAREST);
            SetParameter(TEXPARAM_MIN_FILTER, FILTER_FUNC_NEAREST);
        }
    }

    GLTexture::~GLTexture() {
        Delete();
    }

    void GLTexture::Bind() {
        // Use texture unit 0
        glActiveTexture(GL_TEXTURE0);

        // Bind
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    void GLTexture::Delete() {
        if (ID > 0) {
            // Delete texture
            glDeleteTextures(1, &ID);

            // Set ID to 0
            ID = 0;
        }
    }

    void GLTexture::SetParameter(GLTextureParameter param_, int value_) {
        // Bind
        Bind();

        // Set parameter
        switch(param_) {
            case TEXPARAM_WRAP_S:
            case TEXPARAM_WRAP_T:
                if (value_ == WRAP_MIRROR_CLAMP && !GL::TexMirrorClampSupported)
                    ConsoleMessage("Clamp mirror mode not supported.", "WARN", "GLTexture");
                else
                    glTexParameteri(GL_TEXTURE_2D, param_, value_);
                break;

            case TEXPARAM_MAG_FILTER:
            case TEXPARAM_MIN_FILTER:
                glTexParameteri(GL_TEXTURE_2D, param_, value_);
                break;
            case TEXPARAM_ANISOTROPIC_FILTER:
                if (value_ <= GL::MaxAnisotropicLevel) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)value_);
                else if (GL::MaxAnisotropicLevel > 0.0f) {
                    ConsoleMessage("Filter level was higher than max, setting to max.", "WARN", "GLTexture");
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GL::MaxAnisotropicLevel);
                } else
                    ConsoleMessage("Anisotropic filtering is not supported", "WARN", "GLTexture");
                break;
            default: break;
        }
    }

    int GLTexture::GetPixelDataSize(int width_, int height_, GLPixelFormat format_) {
        auto bpp = 0;

        switch (format_)
        {
            case UNCOMPRESSED_GRAYSCALE: bpp = 8; break;
            case UNCOMPRESSED_GRAY_ALPHA:
            case UNCOMPRESSED_R5G6B5:
            case UNCOMPRESSED_R5G5B5A1:
            case UNCOMPRESSED_R4G4B4A4: bpp = 16; break;
            case UNCOMPRESSED_R8G8B8A8: bpp = 32; break;
            case UNCOMPRESSED_R8G8B8: bpp = 24; break;
            case UNCOMPRESSED_R32: bpp = 32; break;
            case UNCOMPRESSED_R32G32B32: bpp = 32*3; break;
            case UNCOMPRESSED_R32G32B32A32: bpp = 32*4; break;
            case COMPRESSED_DXT1_RGB:
            case COMPRESSED_DXT1_RGBA:
            case COMPRESSED_ETC1_RGB:
            case COMPRESSED_ETC2_RGB:
            case COMPRESSED_PVRT_RGB:
            case COMPRESSED_PVRT_RGBA: bpp = 4; break;
            case COMPRESSED_DXT3_RGBA:
            case COMPRESSED_DXT5_RGBA:
            case COMPRESSED_ETC2_EAC_RGBA:
            case COMPRESSED_ASTC_4x4_RGBA: bpp = 8; break;
            case COMPRESSED_ASTC_8x8_RGBA: bpp = 2; break;
            default: break;
        }

        auto dataSize = width_*height_*bpp/8;  // Total data size in bytes

        // Most compressed formats works on 4x4 blocks,
        // if texture is smaller, minimum dataSize is 8 or 16
        if ((width_ < 4) && (height_ < 4))
        {
            if ((format_ >= COMPRESSED_DXT1_RGB) && (format_ < COMPRESSED_DXT3_RGBA)) dataSize = 8;
            else if ((format_ >= COMPRESSED_DXT3_RGBA) && (format_ < COMPRESSED_ASTC_8x8_RGBA)) dataSize = 16;
        }

        return dataSize;
    }
}
#endif
