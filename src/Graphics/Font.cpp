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

    std::shared_ptr<TFont> TFont::DefaultFont;

    // Public Constructor(s)

    TFont::TFont() {}

    // Destructor

    TFont::~TFont() {
        ConsoleMessage("Unloading and deleting font.", "NOTICE", "FONT");
    }

    // Public Methods

    std::shared_ptr<TFont> TFont::GetDefaultFont() {
        if (DefaultFont == nullptr || !DefaultFont->IsValid()) {
            //_DefaultFont = std::shared_ptr<TFont>(FromRaylibFont(GetFontDefault()));
        }

        return DefaultFont;
    }

    std::shared_ptr<TFont> TFont::LoadTTFont(const Filesystem::TPath &path_, int baseSize_, std::vector<int> fontChars_) {
        // Initialize font
        auto font = std::make_shared<TFont>();

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
        //return TVector2::FromRaylibVec(
        //    MeasureTextEx((*this).ToRaylibFont(), string_.c_str(), fontSize_, spacing_));
        return {0, 0};
    }

    bool TFont::IsValid() const {
        return false;
    }

    void TFont::Unload() {

    }

    // Private Methods

    // SDF default values
#define SDF_CHAR_PADDING            4
#define SDF_ON_EDGE_VALUE         128
#define SDF_PIXEL_DIST_SCALE     64.0f

#define BITMAP_ALPHA_THRESHOLD     80

    void TFont::__GenerateAtlas() {

    }

    void TFont::__LoadFontInfo(const Filesystem::TPath &path_, std::vector<int> chars_) {
        // Characters
        std::vector<TCharInfo> characters;

        // Load font file
        auto fontFile = Filesystem::TFile(path_);
        if (fontFile.Open(Filesystem::MODE_READ, true)) {
            // Get font data
            auto size = fontFile.GetSize();

            auto fontDataPtr = fontFile.ReadBytes();
            auto fontData = fontDataPtr.get();
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
                Characters[i].Image = TImage::LoadPixels(pixels, chw, chh, UNCOMPRESSED_GRAYSCALE);

                // TODO: Option to remove anti-aliasing

                // Set offset
                Characters[i].OffsetY += (int)((float)ascent*scaleFactor);

                // Get advance
                stbtt_GetCodepointHMetrics(&fontInfo, ch, &Characters[i].AdvanceX, nullptr);
                Characters[i].AdvanceX *= scaleFactor;
            }

            //delete fontData;
        } else {
            throw std::runtime_error("Unable to open font file.");
        }
    }
}
