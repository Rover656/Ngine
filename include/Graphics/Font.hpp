/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#ifndef FONT_HPP
#define FONT_HPP

#include "../Config.hpp"

#include "../Filesystem/Filesystem.hpp"
#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "../Resource.hpp"
#include "Image.hpp"
#include "Texture2D.hpp"

namespace NerdThings::Ngine::Graphics {
    /**
     * The text alignment
     *
     * @todo Need to use enum classes more...
     */
    enum class TextAlignment {
        Left,
        Center,
        Right
    };

    /**
     * Text wrapping mode.
     */
    enum class TextWrap {
        None,
        BreakLine,
        BreakWord
    };

    /**
     * Character information.
     */
    struct NEAPI CharInfo {
        /**
         * Character value.
         */
        int Character;

        /**
         * Sprite font source rectangle.
         */
        Ngine::Rectangle Rectangle;

        /**
         * Character X offset for drawing.
         */
        int OffsetX;

        /**
         * Character Y offset for drawing.
         */
        int OffsetY;

        /**
         * Character advance X position.
         */
        int AdvanceX;

        /**
         * Greyscale pixel data.
         */
        std::shared_ptr<Graphics::Image> Image;

        /**
         * Create an empty character info structure.
         */
        CharInfo()
                : Character(0),
                  OffsetX(0),
                  OffsetY(0),
                  AdvanceX(0) {}
    };

    /**
     * A font.
     */
    struct NEAPI Font : public IResource {
        /**
         * The default font.
         */
        static Font *DefaultFont;

        /**
         * Font texture.
         */
        std::shared_ptr<Texture2D> Texture;

        /**
         * Base size (default char height).
         */
        int BaseSize = 0;

        /**
         * Character count.
         */
        int CharacterCount = 0;

        /**
         * Character data.
         */
        std::vector<CharInfo> Characters;

        /**
         * Create an empty font.
         */
        Font();

        virtual ~Font();

        /**
         * Get the default font.
         *
         * @return The default font (set by you).
         */
        static Font *GetDefaultFont();

        /**
         * Set the default font.
         *
         * @param font_ The default font.
         */
        static void SetDefaultFont(Font *font_);

        /**
         * Get a character glyph index.
         *
         * @param char_ The character to get.
         * @return The glyph index within the character info.
         */
        int GetGlyphIndex(int char_) const;

        /**
         * Get the font texture.
         *
         * @return The font atlas (for rendering).
         */
        Texture2D *GetTexture() const;

        /**
         * Whether or not the font is valid.
         *
         * @return Whether or not the font is valid.
         */
        bool IsValid() const override;

        /**
         * Load a true type font with specified characters.
         *
         * @warning The higher the base size is set, the more memory each loaded font will use.
         * @param path_ Path to font file.
         * @param baseSize_ Base font size.
         * @param fontChars_ The list of characters to add into the atlas. Leave empty for standard alpha-numeric characters
         * @return The loaded font.
         */
        static Font *LoadTTFFont(GraphicsDevice *graphicsDevice_, const Filesystem::Path &path_, int baseSize_ = 36,
                                 std::vector<int> fontChars_ = std::vector<int>());

        /**
         * Measure the dimensions of a string (on one line).
         *
         * @param string_ The string to measure
         * @param fontSize_ The size of the font.
         * @param spacing_ Font spacing.
         * @return The size of the string.
         * @todo Make this support wrapping and rectangles etc.
         */
        Vector2 MeasureString(const std::string &string_, float fontSize_, float spacing_) const;

        /**
         * Unload the font.
         */
        void Unload() override;

        //// NEW -- The Rendering API for the new Renderer :)

        /**
         * Draw a string with this font at the given position.
         *
         * @param renderer_ The renderer.
         * @param string_ The string to render.
         * @param position_ The position to render at.
         * @param fontSize_ The font size to use.
         * @param spacing_ Letter spacing.
         */
        void DrawString(Renderer *renderer_, const std::string &string_, const Vector2 &position_, float fontSize_,
                        float spacing_, const Angle &rotation_ = 0, const Vector2 &origin_ = Vector2::Zero);

        /**
         * Draw a string within given bounds with options for alignment and wrapping.
         */
        void DrawString(Renderer *renderer_, const std::string &string_, const Rectangle &bounds_, float fontSize_,
                        float spacing_, TextAlignment alignment_ = TextAlignment::Left, TextWrap wrap_ = TextWrap::None,
                        const Angle &rotation_ = 0, const Vector2 &origin_ = Vector2::Zero);

        /**
         * Measure the size of a string if it was rendered without constraints.
         */
        Vector2 NEW_MeasureString(const std::string &string_, float fontSize_, float spacing_);

        /**
         * Measure the size of a string if it was rendered with a maximum width.
         */
        Vector2
        NEW_MeasureStringRestrictedW(const std::string &string_, float fontSize_, float spacing_, float maxWidth_,
                                     TextAlignment alignment_ = TextAlignment::Left, TextWrap wrap_ = TextWrap::None);

        /**
         * Measure the size of a string if it was rendered with a maximum height.
         */
        Vector2
        NEW_MeasureStringRestrictedH(const std::string &string_, float fontSize_, float spacing_, float maxHeight_,
                                     TextAlignment alignment_ = TextAlignment::Left, TextWrap wrap_ = TextWrap::None);

    private:
        void _generateAtlas(GraphicsDevice *graphicsDevice_);

        void _loadFontInfo(const Filesystem::Path &path_, std::vector<int> chars_);
    };
}

#endif //FONT_HPP
