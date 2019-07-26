/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Texture2D.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    TTexture2D::TTexture2D(TTexture2D &&tex_) noexcept {
        InternalTexture = tex_.InternalTexture;
        Width = tex_.Width;
        Height = tex_.Height;

        tex_.InternalTexture = nullptr;
        tex_.Width = 0;
        tex_.Height = 0;
    }

    // Destructor

    TTexture2D::~TTexture2D() {
        if (InternalTexture != nullptr) {
            // Unassign texture. Smart pointers do the rest
            InternalTexture = nullptr;
        }
    }

    // Public Methods

    std::shared_ptr<TTexture2D> TTexture2D::LoadTexture(const std::string &filename_) {
        //return FromRaylibTex(::LoadTexture(filename_.c_str()));
        // TODO: Texture loading
        return nullptr;
    }

    void TTexture2D::SetTextureFilter(const ETextureFilterMode filterMode_) const {
        switch(filterMode_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
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

    void TTexture2D::SetTextureWrap(const ETextureWrapMode wrapMode_) const {
        switch (wrapMode_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
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
}
