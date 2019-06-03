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

#ifndef FONT_H
#define FONT_H

#include "../ngine.h"

#include "../Math/Rectangle.h"
#include "../Math/Vector2.h"
#include "Texture2D.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * Character information
     */
    struct NEAPI TCharInfo {
        // Public Fields

        /*
         * Character (Unicode)
         */
        int Character;

        /*
         * Sprite font source rectangle
         */
        Math::TRectangle Rectangle;

        /*
         * Character X offset for drawing
         */
        int OffsetX;

        /*
         * Character Y offset for drawing
         */
        int OffsetY;

        /*
         * Character advance X position
         */
        int AdvanceX;

        /*
         * Greyscale pixel data
         */
        unsigned char *Data;

        // Public Constructor

        TCharInfo()
            : Character(0),
              OffsetX(0),
              OffsetY(0),
              AdvanceX(0),
              Data(nullptr) {}

        // Public Methods
        #ifdef INCLUDE_RAYLIB

        /*
         * Convert to raylib charinfo
         */
        CharInfo ToRaylibInfo() const;

        /*
         * Convert from raylib charinfo
         */
        static TCharInfo FromRaylibInfo(const CharInfo &info_);

        #endif
    };

    /*
     * A font
     */
    struct NEAPI TFont {
    private:
        /*
         * The default font
         */
        static TFont* _DefaultFont;
    public:

        // Public Fields

        /*
         * Font texture
         */
        TTexture2D Texture;

        /*
         * Base size (default char height)
         */
        int BaseSize;

        /*
         * Character count
         */
        int CharacterCount;

        /*
         * Character data
         */
        TCharInfo *Characters;

        // Public Constructor(s)

        /*
         * Create a null font
         */
        TFont()
            : BaseSize(0), CharacterCount(0), Characters(nullptr) {}

        /*
         * Move a font
         */
        TFont(TFont &&font_) noexcept;

        /*
         * Copy a font (Reference, if one is deleted, both will stop working correctly.)
         * Use with caution.
         */
        TFont(const TFont &font_) = default;

        // Public Methods

        #ifdef INCLUDE_RAYLIB

        /*
         * Convert to raylib font
         */
        Font ToRaylibFont() const;

        /*
         * Convert from raylib font
         */
        static TFont *FromRaylibFont(const Font &font_);

        #endif

        /*
         * Get the default font (Supplied by raylib)
         */
        static TFont *GetDefaultFont();

        /*
         * Load a font
         */
        static TFont *LoadFont(const std::string &filename_);

        /*
         * Measure the dimensions of a string
         */
        [[nodiscard]] Math::TVector2 MeasureString(const std::string &string_, float fontSize_, float spacing_) const;

        // Operators

        /*
         * Move a font
         */
        TFont &operator=(TFont &&font_) noexcept {
            Texture = std::move(font_.Texture);
            BaseSize = font_.BaseSize;
            CharacterCount = font_.CharacterCount;
            Characters = font_.Characters;

            font_.Texture = TTexture2D();
            font_.BaseSize = 0;
            font_.CharacterCount = 0;
            font_.Characters = nullptr;

            return *this;
        }

        /*
         * Copy a font (Reference, if one is deleted, both will stop working correctly.)
         * Use with caution.
         */
        TFont &operator=(const TFont &font_) = default;

    private:
        // Private Constructor(s)

        TFont(TTexture2D tex_, int baseSize_, int charCount_, TCharInfo *chars_)
            : BaseSize(baseSize_), CharacterCount(charCount_), Characters(chars_) {
            Texture = std::move(tex_);
        }
    };
}

#endif //FONT_H
