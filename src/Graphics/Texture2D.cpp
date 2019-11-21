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

#include "Texture2D.h"

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifdef USE_EXPERIMENTAL_RENDERER

#include "Rewrite/OpenGLDefines.h"

#endif
#endif

#include "Rewrite/GraphicsDevice.h"

namespace NerdThings::Ngine::Graphics {
    void Texture2D::__CreateTexture(Rewrite::GraphicsDevice *graphicsDevice_, unsigned char *data_) {
        // Unbind any bound textures
        glBindTexture(GL_TEXTURE_2D, 0);

        // Check format support
        if ((!graphicsDevice_->GetGLSupportFlag(Rewrite::GraphicsDevice::GL_COMP_DXT))
            && ((_Format == COMPRESSED_DXT1_RGB)
                || (_Format == COMPRESSED_DXT1_RGBA)
                || (_Format == COMPRESSED_DXT3_RGBA)
                || (_Format == COMPRESSED_DXT5_RGBA))) {
            throw std::runtime_error("DXT compressed texture format not supported");
        }

        if ((!graphicsDevice_->GetGLSupportFlag(Rewrite::GraphicsDevice::GL_COMP_ETC1))
            && (_Format == COMPRESSED_ETC1_RGB)) {
            throw std::runtime_error("ETC1 compressed texture format not supported");
        }

        if ((!graphicsDevice_->GetGLSupportFlag(Rewrite::GraphicsDevice::GL_COMP_ETC2)) &&
            ((_Format == COMPRESSED_ETC2_RGB) || (_Format == COMPRESSED_ETC2_EAC_RGBA))) {
            throw std::runtime_error("ETC2 compressed texture format not supported");
        }

        if ((!graphicsDevice_->GetGLSupportFlag(Rewrite::GraphicsDevice::GL_COMP_PVRT)) &&
            ((_Format == COMPRESSED_PVRT_RGB) || (_Format == COMPRESSED_PVRT_RGBA))) {
            throw std::runtime_error("PVRT compressed texture format not supported");
        }

        if ((!graphicsDevice_->GetGLSupportFlag(Rewrite::GraphicsDevice::GL_COMP_ASTC)) &&
            ((_Format == COMPRESSED_ASTC_4x4_RGBA) || (_Format == COMPRESSED_ASTC_8x8_RGBA))) {
            throw std::runtime_error("ASTC compressed texture format not supported");
        }

        // Create texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &_ID);

        // Bind
        glBindTexture(GL_TEXTURE_2D, _ID);

        // Generate mipmaps
        int mipWidth = Width;
        int mipHeight = Height;
        int mipOffset = 0;

        unsigned int glInternalFormat, glFormat, glType;
        graphicsDevice_->GetGLTextureFormats(_Format, &glInternalFormat, &glFormat, &glType);

        for (int i = 0; i < _MipmapCount; i++) {
            unsigned int mipSize = __CalculatePixelDataSize();

            if (glInternalFormat != -1) {
                if (_Format < COMPRESSED_DXT1_RGB) glTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, glFormat, glType, (unsigned char *)data_ + mipOffset);
                else glCompressedTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, mipSize, (unsigned char *)data_ + mipOffset);

#if defined(GRAPHICS_OPENGL33)
                if (_Format == UNCOMPRESSED_GRAYSCALE)
                {
                    GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
                    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
                }
                else if (_Format == UNCOMPRESSED_GRAY_ALPHA)
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

        // TEMP: Needs proper implementation
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    int Texture2D::__CalculatePixelDataSize() {
        auto bpp = 0;

        switch (_Format)
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

        auto dataSize = Width*Height*bpp/8;  // Total data size in bytes

        // Most compressed formats works on 4x4 blocks,
        // if texture is smaller, minimum dataSize is 8 or 16
        if ((Width < 4) && (Height < 4))
        {
            if ((_Format >= COMPRESSED_DXT1_RGB) && (_Format < COMPRESSED_DXT3_RGBA)) dataSize = 8;
            else if ((_Format >= COMPRESSED_DXT3_RGBA) && (_Format < COMPRESSED_ASTC_8x8_RGBA)) dataSize = 16;
        }

        return dataSize;
    }

    Texture2D::Texture2D() {
        Unload();
    }

    Texture2D::Texture2D(Rewrite::GraphicsDevice *graphicsDevice_, unsigned char *data_, unsigned int width_,
                         unsigned height_, PixelFormat format_, int mipmapCount_) {
        // Check dimensions
        if (width_ <= 0 || height_ <= 0) {
            ConsoleMessage("Texture was given invalid dimensions of " + std::to_string(width_) + ", " +
                           std::to_string(height_) + ".", "ERROR", "Texture2D");
            return;
        }

        Width = width_;
        Height = height_;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifdef USE_EXPERIMENTAL_RENDERER
        _MipmapCount = mipmapCount_;
        _Format = format_;

        __CreateTexture(graphicsDevice_, data_);
#else
        // Get format
        OpenGL::GLPixelFormat frmt = OpenGL::UNCOMPRESSED_GRAYSCALE;
        if (format_ == UNCOMPRESSED_GRAY_ALPHA) frmt = OpenGL::UNCOMPRESSED_GRAY_ALPHA;
        else if (format_ == UNCOMPRESSED_R8G8B8) frmt = OpenGL::UNCOMPRESSED_R8G8B8;
        else if (format_ == UNCOMPRESSED_R8G8B8A8) frmt = OpenGL::UNCOMPRESSED_R8G8B8A8;
        else throw std::runtime_error("Incompatible format.");

        InternalTexture = std::make_shared<OpenGL::GLTexture>(width_, height_, data_, mipmapCount_, frmt);
#endif
#endif
    }

    Texture2D::Texture2D(Rewrite::GraphicsDevice *graphicsDevice_, const Filesystem::Path &path_) {
        // Create
        Image img(path_);
        *this = Texture2D(graphicsDevice_, &img);
    }

    Texture2D::Texture2D(Rewrite::GraphicsDevice *graphicsDevice_, const Image *img_) {
        // Create
        *this = Texture2D(graphicsDevice_, img_->PixelData, img_->Width, img_->Height, img_->Format, img_->MipmapCount);
    }

    int Texture2D::GetMipmapCount() const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifndef USE_EXPERIMENTAL_RENDERER
        return InternalTexture->MipmapCount;
#endif
#endif
        return 0;
    }

    void Texture2D::SetTextureFilter(const TextureFilterMode filterMode_) const {
        switch (filterMode_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifndef USE_EXPERIMENTAL_RENDERER
            case FILTER_POINT:
                if (InternalTexture->MipmapCount > 1) {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_MIP_NEAREST);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_NEAREST);
                } else {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_NEAREST);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_NEAREST);
                }
                break;
            case FILTER_BILINEAR:
                if (InternalTexture->MipmapCount > 1) {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_LINEAR_MIP_NEAREST);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_NEAREST);
                } else {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_LINEAR);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_LINEAR);
                }
                break;
            case FILTER_TRILINEAR:
                if (InternalTexture->MipmapCount > 1) {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_MIP_NEAREST);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_LINEAR);
                } else {
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MIN_FILTER, OpenGL::FILTER_FUNC_LINEAR);
                    InternalTexture->SetParameter(OpenGL::TEXPARAM_MAG_FILTER, OpenGL::FILTER_FUNC_LINEAR);
                }
                break;
            case FILTER_ANISOTROPIC_4X:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_ANISOTROPIC_FILTER, 4);
                break;
            case FILTER_ANISOTROPIC_8X:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_ANISOTROPIC_FILTER, 8);
                break;
            case FILTER_ANISOTROPIC_16X:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_ANISOTROPIC_FILTER, 16);
                break;
#endif
#endif
            default:
                break;
        }
    }

    void Texture2D::SetTextureWrap(const TextureWrapMode wrapMode_) const {
        switch (wrapMode_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifndef USE_EXPERIMENTAL_RENDERER
            case WRAP_REPEAT:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_S, OpenGL::WRAP_REPEAT);
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_T, OpenGL::WRAP_REPEAT);
                break;
            case WRAP_CLAMP:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_S, OpenGL::WRAP_CLAMP);
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_T, OpenGL::WRAP_CLAMP);
                break;
            case WRAP_MIRROR_REPEAT:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_S, OpenGL::WRAP_MIRROR_REPEAT);
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_T, OpenGL::WRAP_MIRROR_REPEAT);
                break;
            case WRAP_MIRROR_CLAMP:
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_S, OpenGL::WRAP_MIRROR_CLAMP);
                InternalTexture->SetParameter(OpenGL::TEXPARAM_WRAP_T, OpenGL::WRAP_MIRROR_CLAMP);
                break;
#endif
#endif
        }
    }

    bool Texture2D::IsValid() const {
#ifndef USE_EXPERIMENTAL_RENDERER
        if (InternalTexture != nullptr)
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
            if (InternalTexture->ID > 0)
                return true;
#endif
#endif
        return false;
    }

    void Texture2D::Unload() {
        // Delete texture data
        Width = 0;
        Height = 0;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifdef USE_EXPERIMENTAL_RENDERER
        glDeleteTextures(1, &_ID);
        _ID = 0;
#else
        InternalTexture = nullptr;
#endif
#endif
    }

    // Operators

    bool Texture2D::operator==(const Texture2D &tex_) const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifndef USE_EXPERIMENTAL_RENDERER
        return InternalTexture->ID == tex_.InternalTexture->ID;
#endif
#endif
        return false;
    }

    bool Texture2D::operator!=(const Texture2D &tex_) const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifndef USE_EXPERIMENTAL_RENDERER
        return InternalTexture->ID != tex_.InternalTexture->ID;
#endif
#endif
        return true;
    }
}
