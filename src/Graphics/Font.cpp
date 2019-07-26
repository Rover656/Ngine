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

#include "../Rectangle.h"
#include "../Vector2.h"

namespace NerdThings::Ngine::Graphics {
    // Private Fields

    std::shared_ptr<TFont> TFont::_DefaultFont;

    // Public Constructor(s)

    TFont::TFont(TFont &&font_) {
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
        //UnloadFont(ToRaylibFont());
    }

    // Public Methods

    std::shared_ptr<TFont> TFont::GetDefaultFont() {
        if (_DefaultFont == nullptr) {
            //_DefaultFont = std::shared_ptr<TFont>(FromRaylibFont(GetFontDefault()));
        }

        return _DefaultFont;
    }

    std::shared_ptr<TFont> TFont::LoadFont(const std::string &filename_) {
        //return FromRaylibFont(::LoadFont(filename_.c_str()));
        return nullptr;
    }

    TVector2 TFont::MeasureString(const std::string &string_, const float fontSize_, const float spacing_) const {
        //return TVector2::FromRaylibVec(
        //    MeasureTextEx((*this).ToRaylibFont(), string_.c_str(), fontSize_, spacing_));
        return {0, 0};
    }
}
