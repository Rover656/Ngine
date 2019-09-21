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

#include <numeric>
#include <utility>
#include <stb_truetype.h>

#include "../Rectangle.h"
#include "../Vector2.h"

namespace NerdThings::Ngine::Graphics {
    // Public Fields

    std::unique_ptr<TFont> TFont::DefaultFont;

    // Public Constructor(s)

    TFont::TFont() {}

    // Destructor

    TFont::~TFont() {
        ConsoleMessage("Unloading and deleting font.", "NOTICE", "FONT");
    }

    // Public Methods

    TFont *TFont::GetDefaultFont() {
        if (DefaultFont == nullptr || !DefaultFont->IsValid()) {
            //_DefaultFont = std::unique_ptr<TFont>(FromRaylibFont(GetFontDefault()));
        }

        return DefaultFont.get();
    }

    int TFont::GetGlyphIndex(int char_) const {
        for (auto i = 0; i < CharacterCount; i++) {
            if (Characters[i].Character == char_) return i;
        }

        // Fail.
        return -1;
    }

    TTexture2D *TFont::GetTexture() const {
        return Texture.get();
    }

    TFont *TFont::LoadTTFFont(const Filesystem::TPath &path_, int baseSize_, std::vector<int> fontChars_) {
        // Initialize font
        auto font = new TFont();

        // Set default info
        font->BaseSize = baseSize_;

        // Load font info
        font->__LoadFontInfo(path_, std::move(fontChars_));

        if (!font->Characters.empty()) {
            // Generate font atals
            font->__GenerateAtlas();

            // Unload individual character images
            for (auto i = 0; i < font->CharacterCount; i++) {
                font->Characters[i].Image->Unload();
            }
        } else {
            font = GetDefaultFont();
        }

        return font;
    }

    TVector2 TFont::MeasureString(const std::string &string_, const float fontSize_, const float spacing_) const {
        int tempLen = 0;                // Used to count longer text line num chars
        int lenCounter = 0;

        float textWidth = 0.0f;
        float tempTextWidth = 0.0f;     // Used to count longer text line width

        float textHeight = (float)BaseSize;
        float scaleFactor = fontSize_/(float)BaseSize;

        int letter = 0;                 // Current character
        int index = 0;                  // Index position in sprite font

        scaleFactor = fontSize_/BaseSize;

        for (int i = 0; i < string_.length(); i += 1) {
            int next = 0;
            letter = string_[i];
            index = GetGlyphIndex(letter);

            if (letter != '\n') {
                if (Characters[index].AdvanceX == 0) textWidth  += ((float)Characters[index].Rectangle.Width*scaleFactor + spacing_);
                else textWidth  += ((float)Characters[index].AdvanceX*scaleFactor + spacing_);
            } else {
                if (tempTextWidth < textWidth) tempTextWidth = textWidth;
                lenCounter = 0;
                textWidth = 0;
                textHeight += ((float)BaseSize*1.5f);
            }
        }

        if (tempTextWidth < textWidth) tempTextWidth = textWidth;

        TVector2 vec;
        vec.X = tempTextWidth*scaleFactor + (float)((tempLen - 1)*spacing_);
        vec.Y = textHeight*scaleFactor;

        return vec;
    }

    bool TFont::IsValid() const {
        return false;
    }

    void TFont::Unload() {
        // Unload atlas texture.
        Texture->Unload();
        Texture = nullptr;

        // Unset stuff
        BaseSize = 0;
        CharacterCount = 0;
        Characters.clear();
    }

    // Private Methods

    // SDF default values
#define SDF_CHAR_PADDING            4
#define SDF_ON_EDGE_VALUE         128
#define SDF_PIXEL_DIST_SCALE     64.0f

#define BITMAP_ALPHA_THRESHOLD     80

    // All from raylib.

    void TFont::__GenerateAtlas() {
        // Generate atlas texture.
        auto atlas = std::make_shared<TImage>();

        // Undetermined constants
        int padding = 0;

        // By this point, no validation is required.

        // Load rectangles
        std::vector<TRectangle> recs(CharacterCount);

        // Calculate image size
        float requiredArea = 0;
        for (int i = 0; i < CharacterCount; i++) requiredArea += ((Characters[i].Image->Width + 2*padding)*(Characters[i].Image->Height + 2*padding));
        float guessSize = sqrtf(requiredArea)*1.3f;
        int imageSize = (int)powf(2, ceilf(logf((float)guessSize)/logf(2)));  // Calculate next POT

        // Set image parameters
        atlas->Width = imageSize;
        atlas->Height = imageSize;
        atlas->PixelData = new unsigned char[atlas->Width * atlas->Height];
        atlas->Format = UNCOMPRESSED_GRAYSCALE;
        atlas->Mipmaps = 1;

        // Default packing method (not adding skyline for now.)
        int offsetX = padding;
        int offsetY = padding;

        // NOTE: Using simple packaging, one char after another
        for (int i = 0; i < CharacterCount; i++)
        {
            // Copy pixel data from fc.data to atlas
            for (int y = 0; y < Characters[i].Image->Height; y++)
            {
                for (int x = 0; x < Characters[i].Image->Width; x++)
                {
                    ((unsigned char *)atlas->PixelData)[(offsetY + y)*atlas->Width + (offsetX + x)] = ((unsigned char *)Characters[i].Image->PixelData)[y*Characters[i].Image->Width + x];
                }
            }

            // Fill chars rectangles in atlas info
            recs[i].X = (float)offsetX;
            recs[i].Y = (float)offsetY;
            recs[i].Width = (float)Characters[i].Image->Width;
            recs[i].Height = (float)Characters[i].Image->Height;

            // Save rectangle
            Characters[i].Rectangle = recs[i];

            // Move atlas position X for next character drawing
            offsetX += (Characters[i].Image->Width + 2*padding);

            if (offsetX >= (atlas->Width - Characters[i].Image->Width - padding))
            {
                offsetX = padding;

                // NOTE: Be careful on offsetY for SDF fonts, by default SDF
                // use an internal padding of 4 pixels, it means char rectangle
                // height is bigger than fontSize, it could be up to (fontSize + 8)
                offsetY += (BaseSize + 2*padding);

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
        Texture = std::make_shared<TTexture2D>(atlas);

        // Delete atlas
        atlas->Unload();
    }

    void TFont::__LoadFontInfo(const Filesystem::TPath &path_, std::vector<int> chars_) {
        // Characters
        std::vector<TCharInfo> characters;

        // Load font file
        auto fontFile = Filesystem::TFile(path_);
        if (fontFile.Open(Filesystem::MODE_READ, true)) {
            // Get font data
            auto size = fontFile.GetSize();

            auto fontData = fontFile.ReadBytes();
            fontFile.Close();

            // Get ready to read data
            stbtt_fontinfo fontInfo;
            if (!stbtt_InitFont(&fontInfo, fontData, 0)) throw std::runtime_error("Failed to init font.");

            //Get font scale factor
            auto scaleFactor = stbtt_ScaleForPixelHeight(&fontInfo, (float)BaseSize);

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
                auto pixels = stbtt_GetCodepointBitmap(&fontInfo, scaleFactor, scaleFactor, ch, &chw, &chh, &Characters[i].OffsetX, &Characters[i].OffsetY);

                // Load image
                Characters[i].Image = std::make_shared<TImage>(pixels, chw, chh, UNCOMPRESSED_GRAYSCALE);

                // TODO: Option to remove anti-aliasing

                // Set offset
                Characters[i].OffsetY += (int)((float)ascent*scaleFactor);

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
