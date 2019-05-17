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

#include "Rectangle.h"
#include "Texture2D.h"

namespace Ngine::Types {
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
        TRectangle Rectangle;

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
              Rectangle(TRectangle()),
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

        TFont()
            : Texture({}), BaseSize(0), CharacterCount(0), Characters(nullptr) {}

        // Public Methods

        #ifdef INCLUDE_RAYLIB

        /*
         * Convert to raylib font
         */
        Font ToRaylibFont() const;

        /*
         * Convert from raylib font
         */
        static TFont FromRaylibFont(const Font& font_);

        #endif

        /*
         * Get the default font (Supplied by raylib)
         */
        static TFont GetDefaultFont();

        /*
         * Load a font
         */
        static TFont LoadFont(const std::string& filename_);

        /*
         * Measure the dimensions of a string
         */
        [[nodiscard]] TVector2 MeasureString(const std::string& string_, float fontSize_, float spacing_) const;
    };
}

#endif //FONT_H
