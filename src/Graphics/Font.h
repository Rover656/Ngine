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
    struct NEAPI CharInfo {
        // Public Fields

        /*
         * Character (Unicode)
         */
        int Character;

        /*
         * Sprite font source rectangle
         */
        Rectangle Rectangle;

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
        std::shared_ptr<Image> Image;

        // Public Constructor

        CharInfo()
            : Character(0),
              OffsetX(0),
              OffsetY(0),
              AdvanceX(0) {}
    };

    /*
     * A font
     */
    struct NEAPI Font : public IResource {
        // Public Fields

        /*
         * The default font
         */
        static Font *DefaultFont;

        /*
         * Font texture
         */
        std::shared_ptr<Texture2D> Texture;

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
        std::vector<CharInfo> Characters;

        // Public Constructor(s)

        /*
         * Create a null font
         */
        Font();

        // Destructor

        virtual ~Font();

        // Public Methods

        /*
         * Get the default font
         */
        static Font *GetDefaultFont();

        /*
         * Get a character glyph index
         */
        int GetGlyphIndex(int char_) const;

        /*
         * Get the font texture
         */
        Texture2D *GetTexture() const;

        /*
         * Whether or not the font is valid.
         */
        bool IsValid() const override;

        /*
         * Load a true type font with specified characters
         */
        static Font *LoadTTFFont(const Filesystem::Path &path_, int baseSize_ = 36, std::vector<int> fontChars_ = std::vector<int>());

        /*
         * Measure the dimensions of a string
         */
        Vector2 MeasureString(const std::string &string_, float fontSize_, float spacing_) const;

        /*
         * Set the default font
         */
        static void SetDefaultFont(Font *font_);

        /*
         * Unload the font
         */
        void Unload() override;
    private:
        // Private Methods

        void __GenerateAtlas();

        void __LoadFontInfo(const Filesystem::Path &path_, std::vector<int> chars_);
    };
}

#endif //FONT_H
