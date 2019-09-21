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

#include "../Filesystem/Filesystem.h"
#include "../Rectangle.h"
#include "../Resource.h"
#include "../Vector2.h"
#include "Image.h"
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
        std::shared_ptr<TImage> Image;

        // Public Constructor

        TCharInfo()
            : Character(0),
              OffsetX(0),
              OffsetY(0),
              AdvanceX(0) {}
    };

    /*
     * A font
     */
    struct NEAPI TFont : public TResource {
        // Public Fields

        /*
         * The default font
         */
        static std::unique_ptr<TFont> DefaultFont;

        /*
         * Font texture
         */
        std::shared_ptr<TTexture2D> Texture;

        /*
         * Base size (default char height)
         */
        int BaseSize = 0;

        /*
         * Character count
         */
        int CharacterCount = 0;

        /*
         * Character data
         */
        std::vector<TCharInfo> Characters;

        // Public Constructor(s)

        /*
         * Create a null font
         */
        TFont();

        // Destructor

        virtual ~TFont();

        // Public Methods

        /*
         * Get the default font
         */
        static TFont *GetDefaultFont();

        /*
         * Get a character glyph index
         */
        int GetGlyphIndex(int char_) const;

        /*
         * Get the font texture
         */
        TTexture2D *GetTexture() const;

        /*
         * Load a true type font with specified characters
         */
        static TFont *LoadTTFFont(const Filesystem::TPath &path_, int baseSize_ = 12, std::vector<int> fontChars_ = std::vector<int>());

        /*
         * Measure the dimensions of a string
         */
        TVector2 MeasureString(const std::string &string_, float fontSize_, float spacing_) const;

        /*
         * Whether or not the font is valid.
         */
        bool IsValid() const override;

        /*
         * Unload the font
         */
        void Unload() override;
    private:
        // Private Methods

        void __GenerateAtlas();

        void __LoadFontInfo(const Filesystem::TPath &path_, std::vector<int> chars_);
    };
}

#endif //FONT_H
