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

    TTexture2D::TTexture2D(TTexture2D &&tex_)
    noexcept {
        ID = tex_.ID;
        Width = tex_.Width;
        Height = tex_.Height;
        Mipmaps = tex_.Mipmaps;
        Format = tex_.Format;

        tex_.ID = 0;
        tex_.Width = 0;
        tex_.Height = 0;
        tex_.Mipmaps = 0;
        tex_.Format = 0;
    }

    // Destructor

    TTexture2D::~TTexture2D() {
        if (ID > 0) {
            UnloadTexture((*this).ToRaylibTex());
            ID = 0;
            Width = 0;
            Height = 0;
            Mipmaps = 0;
            Format = 0;
        }
    }

    // Public Methods

    #ifdef INCLUDE_RAYLIB

    Texture2D TTexture2D::ToRaylibTex() const {
        return {
            ID,
            Width,
            Height,
            Mipmaps,
            Format
        };
    }

    TTexture2D TTexture2D::FromRaylibTex(const Texture2D tex_) {
        return {
            tex_.id,
            tex_.width,
            tex_.height,
            tex_.mipmaps,
            tex_.format
        };
    }

    #endif

    void TTexture2D::GenerateMipmaps() const {
        auto tex = (*this).ToRaylibTex();
        GenTextureMipmaps(&tex);
    }

    TTexture2D *TTexture2D::LoadTexture(const std::string &filename_) {
        auto tex = FromRaylibTex(::LoadTexture(filename_.c_str()));
        auto ret = new TTexture2D(tex.ID, tex.Width, tex.Height, tex.Mipmaps, tex.Format);
        tex.ID = 0;
        tex.Width = 0;
        tex.Height = 0;
        tex.Mipmaps = 0;
        tex.Format = 0;
        return ret;
    }

    void TTexture2D::SetTextureFilter(const ETextureFilterMode filterMode_) const {
        ::SetTextureFilter(ToRaylibTex(), static_cast<int>(filterMode_));
    }

    void TTexture2D::SetTextureWrap(const ETextureWrapMode wrapMode_) const {
        ::SetTextureWrap(ToRaylibTex(), static_cast<int>(wrapMode_));
    }

}
