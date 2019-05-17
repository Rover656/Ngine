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

#include "Font.h"

namespace Ngine::Types {
    //----------------------------------------------------------------------------------
    // TCharInfo
    //----------------------------------------------------------------------------------

    // Public Methods

    #ifdef INCLUDE_RAYLIB

    CharInfo TCharInfo::ToRaylibInfo() const {
        CharInfo inf;

        inf.value = Character;
        inf.rec = Rectangle.ToRaylibRect();
        inf.offsetX = OffsetX;
        inf.offsetY = OffsetY;
        inf.advanceX = AdvanceX;
        inf.data = Data;

        return inf;
    }

    TCharInfo TCharInfo::FromRaylibInfo(const CharInfo &info_) {
        TCharInfo inf;

        inf.Character = info_.value;
        inf.Rectangle = TRectangle::FromRaylibRect(info_.rec);
        inf.OffsetX = info_.offsetX;
        inf.OffsetY = info_.offsetY;
        inf.AdvanceX = info_.advanceX;
        inf.Data = info_.data;

        return inf;
    }

    #endif

    //----------------------------------------------------------------------------------
    // TFont
    //----------------------------------------------------------------------------------

    // Public Methods

    #ifdef INCLUDE_RAYLIB

    Font TFont::ToRaylibFont() const {
        Font fnt;

        fnt.texture = Texture.ToRaylibTex();
        fnt.baseSize = BaseSize;
        fnt.charsCount = CharacterCount;

        // This works because all of the structs used follow the same structure
        fnt.chars = reinterpret_cast<CharInfo*>(Characters);

        return fnt;
    }

    TFont TFont::FromRaylibFont(const Font &font_) {
        TFont fnt;

        fnt.Texture = TTexture2D::FromRaylibTex(font_.texture);
        fnt.BaseSize = font_.baseSize;
        fnt.CharacterCount = font_.charsCount;

        // This works because all of the structs used follow the same structure
        fnt.Characters = reinterpret_cast<TCharInfo*>(font_.chars);

        return fnt;
    }

    #endif

    struct TFont TFont::GetDefaultFont() {
        return FromRaylibFont(GetFontDefault());
    }

    TFont TFont::LoadFont(const std::string &filename_) {
        return TFont::FromRaylibFont(::LoadFont(filename_.c_str()));
    }

    TVector2 TFont::MeasureString(const std::string &string_, const float fontSize_, const float spacing_) const {
        return TVector2::FromRaylibVec(MeasureTextEx((*this).ToRaylibFont(), string_.c_str(), fontSize_, spacing_));
    }
}
