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

#include "ngine/graphics/Font.hpp"

#include "ngine/graphics/Renderer.hpp"
#include "ngine/Console.hpp"
#include "ngine/Rectangle.hpp"

#include <cmath>
#include <numeric>
#include <utility>
#include <stb_truetype.h>

namespace ngine::graphics {
    Font *Font::m_defaultFont;

    Font::Font() {}

    Font::~Font() {
        Console::notice("Font", "Unloading and deleting font.");
        free();
    }

    Font *Font::getDefaultFont() {
        return m_defaultFont;
    }

    int Font::getGlyphIndex(int character) const {
        for (auto i = 0; i < m_characterCount; i++) {
            if (m_characters[i].Character == character) return i;
        }

        // Fail.
        return -1;
    }

    Texture2D *Font::getTexture() const {
        return m_texture;
    }

    bool Font::isValid() const {
        return m_texture->isValid();
    }

    Font *Font::loadTTFFont(GraphicsDevice *graphicsDevice, const filesystem::Path &path, int baseSize,
                            std::vector<int> fontChars) {
        // Check size
        if (baseSize < 1) throw std::runtime_error("Invalid base size.");

        // Initialize font
        auto font = new Font();

        // Set default info
        font->m_baseSize = baseSize;

        // Load font info
        font->_loadFontInfo(path, std::move(fontChars));

        if (!font->m_characters.empty()) {
            // Generate font atals
            font->_generateAtlas(graphicsDevice);

            // Free individual character images
            for (auto i = 0; i < font->m_characterCount; i++) {
                font->m_characters[i].Image->free();
            }
        } else {
            font = getDefaultFont();
        }

        return font;
    }

    void Font::setDefaultFont(Font * font) {
        m_defaultFont = font;
    }

    void Font::free() {
        // Delete atlas texture.
        delete m_texture;
        m_texture = nullptr;

        // Unset stuff
        m_baseSize = 0;
        m_characterCount = 0;
        m_characters.clear();
    }

    void Font::drawString(Renderer *renderer, const std::string &string, const Vector2 &position, float fontSize,
                          float spacing, const Color &color, LineSpacing lineSpacing, const Angle &rotation,
                          const Vector2 &origin) {
        // Check if a single batch will fit:
        auto willFit = renderer->willFit(PrimitiveType::Quads, string.length());
        if (!willFit) // TODO: Automatically batch text for the developer
            Console::fail("Font", "Too much text for a single buffer, please split your draw calls up.");

        // We need to know how big this will be
        auto size = measureString(string, fontSize, spacing, lineSpacing);

        // Determine scaling
        float scaleFactor = fontSize / m_baseSize;
        float lineHeight = (float) m_baseSize * scaleFactor;

        // Transform to get it in the right place and orientation
        renderer->pushMatrix();
        if (renderer->getCoordinateSystem() == CoordinateSystem::GUI)
            renderer->translate({position.X, position.Y, 0});
        else renderer->translate({position.X, position.Y - lineHeight, 0});

        // Apply rotation (this fixes issues with different Y stuffs)
        if (renderer->getCoordinateSystem() == CoordinateSystem::GUI)
            renderer->rotate(rotation, {0, 0, 1});
        else renderer->rotate(rotation, {0, 0, -1});

        renderer->translate({-origin.X, -origin.Y, 0});

        // Ready our texture
        renderer->setTexture(m_texture);
        renderer->beginVertices(PrimitiveType::Quads);

        // Get line spacing as float
        float lineSpacingf = 1.5f;
        switch (lineSpacing) {
            case LineSpacing::One:
                lineSpacingf = 1.0f;
                break;
            case LineSpacing::OnePointFive:
                lineSpacingf = 1.5f;
                break;
            case LineSpacing::Two:
                lineSpacingf = 2.0f;
                break;
        }

        // Loop variables
        float textX = 0.0f, textY = 0.0f;

        // Cycle over each character
        for (auto i = 0; i < string.length(); i++) {
            auto letter = string[i]; // TODO: Unicode support
            auto index = getGlyphIndex(letter);

            if (letter == '\n') {
                // New line
                if (renderer->getCoordinateSystem() == CoordinateSystem::GUI)
                    textY += lineHeight * lineSpacingf;
                else textY -= lineHeight * lineSpacingf;
                textX = 0;
            } else {
                if (letter != ' ') {
                    // Translate to correct position
                    renderer->pushMatrix();

                    if (renderer->getCoordinateSystem() == CoordinateSystem::GUI)
                        renderer->translate({textX + m_characters[index].OffsetX * scaleFactor,
                                             textY + m_characters[index].OffsetY * scaleFactor, 0});
                    else renderer->translate({textX + m_characters[index].OffsetX * scaleFactor,
                                             textY, 0});

                    // Push vertices
                    auto srcRect = m_characters[index].Rectangle;
                    auto width = m_characters[index].Rectangle.Width * scaleFactor;
                    auto height = m_characters[index].Rectangle.Height * scaleFactor;
                    auto atlasWidth = m_texture->Width;
                    auto atlasHeight = m_texture->Height;

                    // Get correct dimensions
                    float x1,y1,x2,y2,srcX1,srcY1,srcX2,srcY2;

                    // Set common values
                    x1 = 0;
                    x2 = width;
                    srcX1 = (srcRect.X) / (float) atlasWidth;
                    srcY1 = (srcRect.Y) / (float) atlasHeight;
                    srcX2 = (srcRect.Width + srcRect.X) / (float) atlasWidth;
                    srcY2 = (srcRect.Height + srcRect.Y) / (float) atlasHeight;

                    if (renderer->getCoordinateSystem() != CoordinateSystem::GUI) {
                        // Start in top-left
                        y1 = height;
                        y2 = 0;
                    } else {
                        y1 = 0;
                        y2 = height;
                    }

                    renderer->pushVertex({
                                                 {x1, y1, 0},
                                                 {
                                                          srcX1,
                                                          srcY1
                                                  }, color});
                    renderer->pushVertex({
                                                 {x1, y2, 0},
                                                 {
                                                          srcX1,
                                                          srcY2
                                                  }, color});
                    renderer->pushVertex({
                                                 {x2, y2, 0},
                                                 {
                                                          srcX2,
                                                          srcY2
                                                  }, color});
                    renderer->pushVertex({
                                                 {x2, y1, 0},
                                                 {
                                                          srcX2,
                                                          srcY1
                                                  }, color});

                    // Pop matrix
                    renderer->popMatrix();
                }

                if (m_characters[index].AdvanceX == 0)
                    textX += ((float) m_characters[index].Rectangle.Width * scaleFactor + spacing);
                else textX += ((float) m_characters[index].AdvanceX * scaleFactor + spacing);
            }
        }

        // End renderer
        renderer->endVertices();
        renderer->popMatrix();
    }

    void Font::drawString(Renderer *renderer, const std::string &string, const Rectangle &bounds, float fontSize,
                          float spacing, const Color &color, LineSpacing lineSpacing, TextAlignment alignment,
                          TextWrap wrap, const Angle &rotation, const Vector2 &origin) {
        Console::fail("Font", "This version of DrawString is not yet implemented.");
    }

    Vector2 Font::measureString(const std::string &string, float fontSize, float spacing, LineSpacing lineSpacing) {
        // Get ready to process text
        float textX = 0.0f, textY = 0.0f, scaleFactor = fontSize / (float) m_baseSize, maxX = 0.0f;

        // We are starting off by measuring height.
        textY = (float) m_baseSize * scaleFactor;

        // Get line spacing as float
        float lineSpacingf = 1.5f;
        switch (lineSpacing) {
            case LineSpacing::One:
                lineSpacingf = 1.0f;
                break;
            case LineSpacing::OnePointFive:
                lineSpacingf = 1.5f;
                break;
            case LineSpacing::Two:
                lineSpacingf = 2.0f;
                break;
        }

        // Cycle over each character
        for (auto i = 0; i < string.length(); i++) {
            auto letter = string[i]; // TODO: Unicode support
            auto index = getGlyphIndex(letter);

            if (letter == '\n') {
                // Save max X before continuing
                if (textX > maxX) maxX = textX;

                // New line
                textY += ((float) m_baseSize * lineSpacingf) * scaleFactor;
                textX = 0;
            } else {
                if (m_characters[index].AdvanceX == 0)
                    textX += ((float) m_characters[index].Rectangle.Width * scaleFactor + spacing);
                else textX += ((float) m_characters[index].AdvanceX * scaleFactor + spacing);
            }
        }
        if (textX > maxX) maxX = textX;

        return {maxX, textY};
    }

    Vector2
    Font::measureStringRestrictedW(const std::string &string, float fontSize, float spacing, float maxWidth,
                                   TextAlignment alignment, TextWrap wrap) {
        Console::fail("Font", "MeasureStringRestrictedW is not yet implemented.");
        return {};
    }

    Vector2
    Font::measureStringRestrictedH(const std::string &string, float fontSize, float spacing, float maxHeight,
                                   TextAlignment alignment, TextWrap wrap) {
        Console::fail("Font", "MeasureStringRestrictedH is not yet implemented.");
        return {};
    }


    void Font::_generateAtlas(GraphicsDevice *graphicsDevice) {
        // Generate atlas texture.
        auto atlas = new Image();

        // Undetermined constants
        int padding = 0;

        // Load rectangles
        std::vector<Rectangle> recs(m_characterCount);

        // Calculate image size
        float requiredArea = 0;
        for (int i = 0; i < m_characterCount; i++)
            requiredArea += ((m_characters[i].Image->Width + 2 * padding) *
                             (m_characters[i].Image->Height + 2 * padding));
        float guessSize = sqrtf(requiredArea) * 1.3f;
        int imageSize = (int) powf(2, ceilf(logf((float) guessSize) / logf(2)));  // Calculate next POT

        // Set image parameters
        atlas->Width = imageSize;
        atlas->Height = imageSize;
        atlas->PixelData = new unsigned char[atlas->Width * atlas->Height];
        atlas->Format = UNCOMPRESSED_GRAYSCALE;
        atlas->MipmapCount = 1;

        // Default packing method (not adding skyline for now.)
        int offsetX = padding;
        int offsetY = padding;

        // NOTE: Using simple packaging, one char after another
        auto maxH = 0;
        for (int i = 0; i < m_characterCount; i++) {
            // Copy pixel data from fc.data to atlas
            for (int y = 0; y < m_characters[i].Image->Height; y++) {
                for (int x = 0; x < m_characters[i].Image->Width; x++) {
                    ((unsigned char *) atlas->PixelData)[(offsetY + y) * atlas->Width + (offsetX +
                                                                                         x)] = ((unsigned char *) m_characters[i].Image->PixelData)[
                            y * m_characters[i].Image->Width + x];
                }
            }

            // Fill chars rectangles in atlas info
            recs[i].X = (float) offsetX;
            recs[i].Y = (float) offsetY;
            recs[i].Width = (float) m_characters[i].Image->Width;
            recs[i].Height = (float) m_characters[i].Image->Height;

            if (maxH < recs[i].Height) maxH = recs[i].Height;

            // Save rectangle
            m_characters[i].Rectangle = recs[i];

            // Move atlas position X for next character drawing
            offsetX += (m_characters[i].Image->Width + 2 * padding);

            if (offsetX >= (atlas->Width - m_characters[i].Image->Width - padding)) {
                offsetX = padding;

                // NOTE: Be careful on offsetY for SDF fonts, by default SDF
                // use an internal padding of 4 pixels, it means char rectangle
                // height is bigger than fontSize, it could be up to (fontSize + 8)
                offsetY += (maxH + 2 * padding);

                maxH = 0;

                if (offsetY > (atlas->Height - m_baseSize - padding)) break;
            }
        }

        // Convert to gray alpha
        unsigned char *dataGrayAlpha = new unsigned char[atlas->Width * atlas->Height * 2];

        for (auto i = 0, k = 0; i < atlas->Width * atlas->Height; i++, k += 2) {
            dataGrayAlpha[k] = 255;
            dataGrayAlpha[k + 1] = atlas->PixelData[i];
        }

        // Complete atlas
        delete atlas->PixelData;
        atlas->PixelData = dataGrayAlpha;
        atlas->Format = UNCOMPRESSED_GRAY_ALPHA;

        // Create texture
        m_texture = new Texture2D(graphicsDevice, atlas);

        // Delete atlas
        delete atlas;
    }

    void Font::_loadFontInfo(const filesystem::Path &path, std::vector<int> characters) {
        // Characters
        std::vector<CharInfo> chars;

        // TODO: Support for bitmap fonts something to consider?

        // Load font file
        auto fontFile = filesystem::File(path);
        if (fontFile.open(filesystem::FileOpenMode::Read, true)) {
            // Get font data
            auto size = fontFile.getSize();

            auto fontData = fontFile.readBytes();
            fontFile.close();

            // Get ready to read data
            stbtt_fontinfo fontInfo;
            if (!stbtt_InitFont(&fontInfo, fontData, 0)) throw std::runtime_error("Failed to init font.");

            //Get font scale factor
            auto scaleFactor = stbtt_ScaleForPixelHeight(&fontInfo, (float) m_baseSize);

            // Calculate metrics
            int ascent, descent, lineGap;
            stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);

            // Verify character count
            m_characterCount = (m_characterCount > 0) ? m_characterCount : 95;

            // Set characters if empty
            if (characters.empty()) {
                // Set size
                characters.resize(m_characterCount);

                // Fill
                std::iota(characters.begin(), characters.end(), 32); //32 = space
            }

            // Reserve space for character info
            m_characters.resize(m_characterCount);

            // Fetch character data
            for (auto i = 0; i < m_characterCount; i++) {
                int chw = 0, chh = 0; // Character w and h.
                int ch = characters[i]; // Character value
                m_characters[i].Character = ch;

                // Get image data
                auto pixels = stbtt_GetCodepointBitmap(&fontInfo, scaleFactor, scaleFactor, ch, &chw, &chh,
                                                       &m_characters[i].OffsetX, &m_characters[i].OffsetY);

                // Load image
                m_characters[i].Image = std::make_shared<Image>(pixels, chw, chh, UNCOMPRESSED_GRAYSCALE);

                // Set offset
                m_characters[i].OffsetY += (int) ((float) ascent * scaleFactor);

                // Get advance
                stbtt_GetCodepointHMetrics(&fontInfo, ch, &m_characters[i].AdvanceX, nullptr);
                m_characters[i].AdvanceX *= scaleFactor;
            }

            // Delete font data
            delete fontData;
        } else {
            throw std::runtime_error("Unable to open font file.");
        }
    }
}
