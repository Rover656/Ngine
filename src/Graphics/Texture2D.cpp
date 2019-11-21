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

#include "Image.h"

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
    }

    Texture2D::Texture2D() {}

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
#ifndef USE_EXPERIMENTAL_RENDERER
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
