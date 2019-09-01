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

#include "../Rectangle.h"
#include "../Vector2.h"
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
        unsigned char *Data;

        // Public Constructor

        TCharInfo()
            : Character(0),
              OffsetX(0),
              OffsetY(0),
              AdvanceX(0),
              Data(nullptr) {}
    };

    /*
     * A font
     */
    struct NEAPI TFont {
    private:
        /*
         * The default font
         */
        static std::shared_ptr<TFont> _DefaultFont;
    public:

        // Public Fields

        /*
         * Font texture
         */
        std::shared_ptr<TTexture2D> Texture;

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
        std::vector<TCharInfo> Characters;

        // Public Constructor(s)

        /*
         * Create a null font
         */
        TFont()
            : BaseSize(0), CharacterCount(0), Characters() {}

        /*
         * Move a font
         */
        TFont(TFont &&font_);

        /*
         * Copy a font (Reference, if one is deleted, both will stop working correctly.)
         * Use with caution.
         */
        TFont(const TFont &font_) = default;

        // Destructor

        virtual ~TFont();

        // Public Methods

        /*
         * Get the default font (Supplied by raylib)
         */
        static std::shared_ptr<TFont> GetDefaultFont();

        /*
         * Load a font
         */
        static std::shared_ptr<TFont> LoadFont(const std::string &filename_);

        /*
         * Measure the dimensions of a string
         */
        [[nodiscard]] TVector2 MeasureString(const std::string &string_, float fontSize_, float spacing_) const;

        // Operators

        /*
         * Move a font
         */
        TFont &operator=(TFont &&font_) noexcept {
            Texture = std::move(font_.Texture);
            BaseSize = font_.BaseSize;
            CharacterCount = font_.CharacterCount;
            Characters = font_.Characters;

            font_.Texture = std::shared_ptr<TTexture2D>(nullptr);
            font_.BaseSize = 0;
            font_.CharacterCount = 0;
            font_.Characters.clear();

            return *this;
        }

        /*
         * Copy a font (Reference, if one is deleted, both will stop working correctly.)
         * Use with caution.
         */
        TFont &operator=(const TFont &font_) = default;

    private:
        // Private Constructor(s)

        TFont(std::shared_ptr<TTexture2D> tex_, int baseSize_, int charCount_, std::vector<TCharInfo> chars_)
            : BaseSize(baseSize_), CharacterCount(charCount_), Characters(chars_) {
            Texture = tex_;
        }
    };
}

#endif //FONT_H
