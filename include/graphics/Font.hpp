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

#include "../filesystem/Filesystem.hpp"
#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "../Resource.hpp"
#include "Image.hpp"
#include "Texture2D.hpp"

namespace ngine::graphics {
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
     * Multiplier for the space between lines
     */
    enum class LineSpacing {
        One,
        OnePointFive,
        Two
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
        ngine::Rectangle Rectangle;

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
        std::shared_ptr<graphics::Image> Image;

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
    class NEAPI Font : public IResource {
        /**
         * The default font.
         */
        static Font *m_defaultFont;

        /**
         * Font texture.
         */
        Texture2D *m_texture;

        /**
         * Base size (default char height).
         */
        int m_baseSize = 0;

        /**
         * Character count.
         */
        int m_characterCount = 0;

        /**
         * Character data.
         */
        std::vector<CharInfo> m_characters;

    public:
        /**
         * Create an empty font.
         */
        Font();

        ~Font();

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
        int getGlyphIndex(int char_) const;

        /**
         * Get the font texture.
         *
         * @return The font atlas (for rendering).
         */
        Texture2D *getTexture() const;

        /**
         * Whether or not the font is valid.
         *
         * @return Whether or not the font is valid.
         */
        bool isValid() const override;

        /**
         * Load a true type font with specified characters.
         *
         * @warning The higher the base size is set, the more memory each loaded font will use.
         * @param path_ Path to font file.
         * @param baseSize_ Base font size.
         * @param fontChars_ The list of characters to add into the atlas. Leave empty for standard alpha-numeric characters
         * @return The loaded font.
         */
        static Font *LoadTTFFont(GraphicsDevice *graphicsDevice_, const filesystem::Path &path_, int baseSize_ = 36,
                                 std::vector<int> fontChars_ = std::vector<int>());

        /**
         * Free the font.
         */
        void free() override;

        /**
         * Draw a string with this font at the given position.
         *
         * @param renderer_ The renderer.
         * @param string_ The string to render.
         * @param position_ The position to render at.
         * @param fontSize_ The font size to use.
         * @param spacing_ Letter spacing.
         */
        void drawString(Renderer *renderer_, const std::string &string_, const Vector2 &position_, float fontSize_,
                        float spacing_, const Color &color_, LineSpacing lineSpacing_ = LineSpacing::OnePointFive,
                        const Angle &rotation_ = 0, const Vector2 &origin_ = Vector2::Zero);

        /**
         * Draw a string within given bounds with options for alignment and wrapping.
         *
         * @warning Not implemented.
         */
        void drawString(Renderer *renderer_, const std::string &string_, const Rectangle &bounds_, float fontSize_,
                        float spacing_, const Color &color_, LineSpacing lineSpacing_ = LineSpacing::OnePointFive,
                        TextAlignment alignment_ = TextAlignment::Left, TextWrap wrap_ = TextWrap::None,
                        const Angle &rotation_ = 0, const Vector2 &origin_ = Vector2::Zero);

        /**
         * Measure the size of a string if it was rendered without constraints.
         */
        Vector2 measureString(const std::string &string_, float fontSize_, float spacing_,
                              LineSpacing lineSpacing_ = LineSpacing::OnePointFive);

        /**
         * Measure the size of a string if it was rendered with a maximum width.
         *
         * @warning Not implemented.
         */
        Vector2 measureStringRestrictedW(const std::string &string_, float fontSize_, float spacing_, float maxWidth_,
                                         TextAlignment alignment_ = TextAlignment::Left,
                                         TextWrap wrap_ = TextWrap::None);

        /**
         * Measure the size of a string if it was rendered with a maximum height.
         *
         * @warning Not implemented.
         */
        Vector2 measureStringRestrictedH(const std::string &string_, float fontSize_, float spacing_, float maxHeight_,
                                         TextAlignment alignment_ = TextAlignment::Left,
                                         TextWrap wrap_ = TextWrap::None);

    private:
        void _generateAtlas(GraphicsDevice *graphicsDevice_);

        void _loadFontInfo(const filesystem::Path &path_, std::vector<int> chars_);
    };
}

#endif //FONT_HPP
