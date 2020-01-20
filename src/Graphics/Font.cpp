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

#include "Graphics/Font.hpp"

#include <numeric>
#include <utility>
#include <stb_truetype.h>

#include "Graphics/Renderer.hpp"
#include "Math.hpp"
#include "Console.hpp"
#include "Rectangle.hpp"

namespace NerdThings::Ngine::Graphics {
    Font *Font::DefaultFont;

    Font::Font() {}

    Font::~Font() {
        Console::Notice("Font", "Unloading and deleting font.");
        Unload();
    }

    Font *Font::GetDefaultFont() {
        return DefaultFont;
    }

    int Font::GetGlyphIndex(int char_) const {
        for (auto i = 0; i < CharacterCount; i++) {
            if (Characters[i].Character == char_) return i;
        }

        // Fail.
        return -1;
    }

    Texture2D *Font::GetTexture() const {
        return Texture.get();
    }

    bool Font::IsValid() const {
        return Texture->IsValid();
    }

    Font *Font::LoadTTFFont(GraphicsDevice *graphicsDevice_, const Filesystem::Path &path_, int baseSize_,
                            std::vector<int> fontChars_) {
        // Check size
        if (baseSize_ < 1) throw std::runtime_error("Invalid base size.");

        // Initialize font
        auto font = new Font();

        // Set default info
        font->BaseSize = baseSize_;

        // Load font info
        font->_loadFontInfo(path_, std::move(fontChars_));

        if (!font->Characters.empty()) {
            // Generate font atals
            font->_generateAtlas(graphicsDevice_);

            // Unload individual character images
            for (auto i = 0; i < font->CharacterCount; i++) {
                font->Characters[i].Image->Unload();
            }
        } else {
            font = GetDefaultFont();
        }

        return font;
    }

    Vector2 Font::MeasureString(const std::string &string_, const float fontSize_, const float spacing_) const {
        int tempLen = 0;                // Used to count longer text line num chars
        int lenCounter = 0;

        float textWidth = 0.0f;
        float tempTextWidth = 0.0f;     // Used to count longer text line width

        float textHeight = (float) BaseSize;
        float scaleFactor = fontSize_ / (float) BaseSize;

        int letter = 0;                 // Current character
        int index = 0;                  // Index position in sprite font

        for (auto i = 0; i < string_.length(); i++) {
            lenCounter++;

            letter = string_[i];
            index = GetGlyphIndex(letter);

            if (letter != '\n') {
                if (Characters[index].AdvanceX == 0) textWidth += Characters[index].Rectangle.Width + spacing_;
                else textWidth += Characters[index].AdvanceX;
            } else {
                if (tempTextWidth < textWidth) tempTextWidth = textWidth;
                lenCounter = 0;
                textWidth = 0;
                textHeight += ((float) BaseSize * 1.5f);
            }

            if (tempLen < lenCounter) tempLen = lenCounter;
        }

        if (tempTextWidth < textWidth) tempTextWidth = textWidth;

        Vector2 vec;
        vec.X = tempTextWidth * scaleFactor + (float) ((tempLen - 1) * spacing_);
        vec.Y = textHeight * scaleFactor;

        return vec;
    }

    void Font::SetDefaultFont(Font *font_) {
        DefaultFont = font_;
    }

    void Font::Unload() {
        // Unload atlas texture.
        Texture->Unload();
        Texture = nullptr;

        // Unset stuff
        BaseSize = 0;
        CharacterCount = 0;
        Characters.clear();
    }


    void Font::DrawString(Renderer *renderer_, const std::string &string_, const Vector2 &position_, float fontSize_,
                          float spacing_, const Color &color_, const Angle &rotation_, const Vector2 &origin_) {
#ifdef USE_EXPERIMENTAL_RENDERER
//        // Transform to get it in the right place and orientation
//        renderer_->PushMatrix();
//        renderer_->Translate({position_.X, position_.Y, 0});
//        renderer_->Rotate(rotation_, {0, 0, 1});
//        renderer_->Translate({-origin_.X, -origin_.Y, 0});
//
//        // Ready our texture
//        renderer_->Begin(PrimitiveType::Quads, Texture.get());

        // TODO: Use the renderer in full

        // Get ready to render text
        float textX = 0.0f, textY = 0.0f, scaleFactor = fontSize_ / BaseSize;

        // Cycle over each character TODO: Check if there are too many characters and split batches.
        for (auto i = 0; i < string_.length(); i++) {
            auto letter = string_[i]; // TODO: Unicode support
            auto index = GetGlyphIndex(letter);

            if (letter == '\n') {
                // New line
                textY += ((float) BaseSize * 1.5f) * scaleFactor; // TODO: Line spacing option/enum?
                textX = 0;
            } else {
                if (letter != ' ') {
                    Texture->Draw(renderer_,
                                  {position_.X + textX + Characters[index].OffsetX * scaleFactor,
                                   position_.Y + textY + Characters[index].OffsetY * scaleFactor,
                                   Characters[index].Rectangle.Width * scaleFactor,
                                   Characters[index].Rectangle.Height * scaleFactor},
                                  Characters[index].Rectangle, color_, {0, 0}, 0.0f);
                }

                if (Characters[index].AdvanceX == 0)
                    textX += ((float) Characters[index].Rectangle.Width * scaleFactor + spacing_);
                else textX += ((float) Characters[index].AdvanceX * scaleFactor + spacing_);
            }
        }
#endif
    }

    void Font::DrawString(Renderer *renderer_, const std::string &string_, const Rectangle &bounds_, float fontSize_,
                          float spacing_, const Color &color_, TextAlignment alignment_, TextWrap wrap_,
                          const Angle &rotation_, const Vector2 &origin_) {

    }

    Vector2 Font::NEW_MeasureString(const std::string &string_, float fontSize_, float spacing_) {
        // Get ready to process text
        float textX = 0.0f, textY = 0.0f, scaleFactor = fontSize_ / (float) BaseSize, maxX = 0.0f;

        // We are starting off by measuring height.
        textY = (float)BaseSize * scaleFactor;

        // Cycle over each character
        for (auto i = 0; i < string_.length(); i++) {
            auto letter = string_[i]; // TODO: Unicode support
            auto index = GetGlyphIndex(letter);

            if (letter == '\n') {
                // Save max X before continuing
                if (textX > maxX) maxX = textX;

                // New line
                textY += ((float) BaseSize * 1.5f) * scaleFactor; // TODO: Line spacing option/enum?
                textX = 0;
            } else {
                if (Characters[index].AdvanceX == 0)
                    textX += ((float) Characters[index].Rectangle.Width * scaleFactor + spacing_);
                else textX += ((float) Characters[index].AdvanceX * scaleFactor + spacing_);
            }
        }
        if (textX > maxX) maxX = textX;

        return {maxX, textY};
    }

    Vector2
    Font::NEW_MeasureStringRestrictedW(const std::string &string_, float fontSize_, float spacing_, float maxWidth_,
                                       TextAlignment alignment_, TextWrap wrap_) {
        return Vector2();
    }

    Vector2
    Font::NEW_MeasureStringRestrictedH(const std::string &string_, float fontSize_, float spacing_, float maxHeight_,
                                       TextAlignment alignment_, TextWrap wrap_) {
        return Vector2();
    }


    void Font::_generateAtlas(GraphicsDevice *graphicsDevice_) {
        // Generate atlas texture.
        auto atlas = std::make_shared<Image>();

        // Undetermined constants
        int padding = 0;

        // Load rectangles
        std::vector<Rectangle> recs(CharacterCount);

        // Calculate image size
        float requiredArea = 0;
        for (int i = 0; i < CharacterCount; i++)
            requiredArea += ((Characters[i].Image->Width + 2 * padding) * (Characters[i].Image->Height + 2 * padding));
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
        for (int i = 0; i < CharacterCount; i++) {
            // Copy pixel data from fc.data to atlas
            for (int y = 0; y < Characters[i].Image->Height; y++) {
                for (int x = 0; x < Characters[i].Image->Width; x++) {
                    ((unsigned char *) atlas->PixelData)[(offsetY + y) * atlas->Width + (offsetX +
                                                                                         x)] = ((unsigned char *) Characters[i].Image->PixelData)[
                            y * Characters[i].Image->Width + x];
                }
            }

            // Fill chars rectangles in atlas info
            recs[i].X = (float) offsetX;
            recs[i].Y = (float) offsetY;
            recs[i].Width = (float) Characters[i].Image->Width;
            recs[i].Height = (float) Characters[i].Image->Height;

            if (maxH < recs[i].Height) maxH = recs[i].Height;

            // Save rectangle
            Characters[i].Rectangle = recs[i];

            // Move atlas position X for next character drawing
            offsetX += (Characters[i].Image->Width + 2 * padding);

            if (offsetX >= (atlas->Width - Characters[i].Image->Width - padding)) {
                offsetX = padding;

                // NOTE: Be careful on offsetY for SDF fonts, by default SDF
                // use an internal padding of 4 pixels, it means char rectangle
                // height is bigger than fontSize, it could be up to (fontSize + 8)
                offsetY += (maxH + 2 * padding);

                maxH = 0;

                if (offsetY > (atlas->Height - BaseSize - padding)) break;
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
        Texture = std::make_shared<Texture2D>(graphicsDevice_, atlas.get());

        // Delete atlas
        atlas->Unload();
    }

    void Font::_loadFontInfo(const Filesystem::Path &path_, std::vector<int> chars_) {
        // Characters
        std::vector<CharInfo> characters;

        // TODO: Support for bitmap fonts something to consider?

        // Load font file
        auto fontFile = Filesystem::File(path_);
        if (fontFile.Open(Filesystem::FileOpenMode::Read, true)) {
            // Get font data
            auto size = fontFile.GetSize();

            auto fontData = fontFile.ReadBytes();
            fontFile.Close();

            // Get ready to read data
            stbtt_fontinfo fontInfo;
            if (!stbtt_InitFont(&fontInfo, fontData, 0)) throw std::runtime_error("Failed to init font.");

            //Get font scale factor
            auto scaleFactor = stbtt_ScaleForPixelHeight(&fontInfo, (float) BaseSize);

            // Calculate metrics
            int ascent, descent, lineGap;
            stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);

            // Verify character count
            CharacterCount = (CharacterCount > 0) ? CharacterCount : 95;

            // Set characters if empty
            if (chars_.empty()) {
                // Set size
                chars_.resize(CharacterCount);

                // Fill
                std::iota(chars_.begin(), chars_.end(), 32); //32 = space
            }

            // Reserve space for character info
            Characters.resize(CharacterCount);

            // Fetch character data
            for (auto i = 0; i < CharacterCount; i++) {
                int chw = 0, chh = 0; // Character w and h.
                int ch = chars_[i]; // Character value
                Characters[i].Character = ch;

                // Get image data
                auto pixels = stbtt_GetCodepointBitmap(&fontInfo, scaleFactor, scaleFactor, ch, &chw, &chh,
                                                       &Characters[i].OffsetX, &Characters[i].OffsetY);

                // Load image
                Characters[i].Image = std::make_shared<Image>(pixels, chw, chh, UNCOMPRESSED_GRAYSCALE);

                // Set offset
                Characters[i].OffsetY += (int) ((float) ascent * scaleFactor);

                // Get advance
                stbtt_GetCodepointHMetrics(&fontInfo, ch, &Characters[i].AdvanceX, nullptr);
                Characters[i].AdvanceX *= scaleFactor;
            }

            // Delete font data
            delete fontData;
        } else {
            throw std::runtime_error("Unable to open font file.");
        }
    }
}
