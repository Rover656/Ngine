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

#include "Rectangle.h"
#include "Vector2.h"

namespace NerdThings::Ngine::Graphics {
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

    // Private Fields

    std::shared_ptr<TFont> TFont::_DefaultFont;

    // Public Constructor(s)

    TFont::TFont(TFont &&font_)
    noexcept {
        Texture = font_.Texture;
        BaseSize = font_.BaseSize;
        CharacterCount = font_.CharacterCount;
        Characters = font_.Characters;

        font_.Texture = std::shared_ptr<TTexture2D>(nullptr);
        font_.BaseSize = 0;
        font_.CharacterCount = 0;
        font_.Characters = nullptr;
    }

    // Destructor

    TFont::~TFont() {
        ConsoleMessage("Unloading and deleting font.", "NOTICE", "FONT");
        UnloadFont(ToRaylibFont());
    }

    // Public Methods

    #ifdef INCLUDE_RAYLIB

    Font TFont::ToRaylibFont() const {
        Font fnt;

        fnt.texture = Texture->ToRaylibTex();
        fnt.baseSize = BaseSize;
        fnt.charsCount = CharacterCount;

        // This works because all of the structs used follow the same structure
        fnt.chars = reinterpret_cast<CharInfo*>(Characters);

        return fnt;
    }

    std::shared_ptr<TFont> TFont::FromRaylibFont(const Font &font_) {
        return std::shared_ptr<TFont>(new TFont(
            TTexture2D::FromRaylibTex(font_.texture),
            font_.baseSize,
            font_.charsCount,
            reinterpret_cast<TCharInfo*>(font_.chars)
        ));
    }

    #endif

    std::shared_ptr<TFont> TFont::GetDefaultFont() {
        if (_DefaultFont == nullptr) {
            _DefaultFont = std::shared_ptr<TFont>(FromRaylibFont(GetFontDefault()));
        }

        return _DefaultFont;
    }

    std::shared_ptr<TFont> TFont::LoadFont(const std::string &filename_) {
        return FromRaylibFont(::LoadFont(filename_.c_str()));
    }

    TVector2 TFont::MeasureString(const std::string &string_, const float fontSize_, const float spacing_) const {
        return TVector2::FromRaylibVec(
            MeasureTextEx((*this).ToRaylibFont(), string_.c_str(), fontSize_, spacing_));
    }
}
