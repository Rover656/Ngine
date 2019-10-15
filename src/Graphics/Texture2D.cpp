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

#include "Image.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    Texture2D::Texture2D() {}

    Texture2D::Texture2D(unsigned char *data_, unsigned int width_, unsigned height_, PixelFormat format_, int mipmapCount_) {
        Width = width_;
        Height = height_;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        // Get format
        OpenGL::GLPixelFormat frmt = OpenGL::UNCOMPRESSED_GRAYSCALE;
        if (format_ == UNCOMPRESSED_GRAY_ALPHA) frmt = OpenGL::UNCOMPRESSED_GRAY_ALPHA;
        else if (format_ == UNCOMPRESSED_R8G8B8) frmt = OpenGL::UNCOMPRESSED_R8G8B8;
        else if (format_ == UNCOMPRESSED_R8G8B8A8) frmt = OpenGL::UNCOMPRESSED_R8G8B8A8;
        else throw std::runtime_error("Incompatible format.");

        InternalTexture = std::make_shared<OpenGL::GLTexture>(width_, height_, data_, mipmapCount_, frmt);
#endif
    }

    Texture2D::Texture2D(const Filesystem::Path &path_) {
        // Create
        *this = Texture2D(std::make_shared<Image>(path_));
    }

    Texture2D::Texture2D(const std::shared_ptr<Image> &img_) {
        // Create
        *this = Texture2D(img_->PixelData, img_->Width, img_->Height, img_->Format, img_->Mipmaps);
    }

    // Public Methods

    std::shared_ptr<Texture2D> Texture2D::FromImage(const std::shared_ptr<Image> &img_) {
        return std::make_shared<Texture2D>(img_);
    }

    int Texture2D::GetMipmapCount() const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        return InternalTexture->MipmapCount;
#endif
        return 0;
    }

    bool Texture2D::IsValid() const {
        if (InternalTexture != nullptr)
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
            if (InternalTexture->ID > 0)
                return true;
#endif
        return false;
    }

    Texture2D *Texture2D::LoadPixels(unsigned char *data_, unsigned int width_, unsigned height_, PixelFormat format_, int mipmapCount_) {
        return new Texture2D(data_, width_, height_, format_, mipmapCount_);
    }

    Texture2D *Texture2D::LoadTexture(const Filesystem::Path &path_) {
        return new Texture2D(path_);
    }

    void Texture2D::SetTextureFilter(const TextureFilterMode filterMode_) const {
        switch(filterMode_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
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
            default: break;
        }
    }

    void Texture2D::SetTextureWrap(const TextureWrapMode wrapMode_) const {
        switch (wrapMode_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
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
        }
    }

    void Texture2D::Unload() {
        // Delete texture data
        Width = 0;
        Height = 0;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        InternalTexture = nullptr;
#endif
    }

    // Operators

    bool Texture2D::operator==(const Texture2D &tex_) const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        return InternalTexture->ID == tex_.InternalTexture->ID;
#endif
        return false;
    }

    bool Texture2D::operator!=(const Texture2D &tex_) const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        return InternalTexture->ID != tex_.InternalTexture->ID;
#endif
        return true;
    }
}
