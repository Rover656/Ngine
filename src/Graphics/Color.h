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

#ifndef COLOR_H
#define COLOR_H

#include "../ngine.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * A 32-bit packed color
     */
    struct NEAPI TColor {
        // Public Fields

        /*
         * The unsigned int value of the color
         */
        unsigned int PackedValue;

        // Public Constructor(s)

        /*
         * Create the default color (White)
         */
        TColor() : PackedValue(0xFFFFFFFF) {}

        /*
         * Create a color using an existing packed value
         */
        explicit TColor(const unsigned int packedValue_) : PackedValue(packedValue_) {}

        /*
         * Create a color using float values between 0 and 1
         */
        TColor(const float r_, const float g_, float b_, const float a_)
            : TColor(static_cast<int>(r_ * 255),
                     static_cast<int>(g_ * 255),
                     static_cast<int>(b_ * 255),
                     static_cast<int>(a_ * 255)) {}

        /*
         * Create a color using int values between 0 and 255
         */
        TColor(int r_, int g_, int b_, int a_);

        // Public Methods

        /*
         * Get the red value as a float
         */
        [[nodiscard]] float RedFloat() const;

        /*
         * Get the green value as a float
         */
        [[nodiscard]] float GreenFloat() const;

        /*
         * Get the blue value as a float
         */
        [[nodiscard]] float BlueFloat() const;

        /*
         * Get the alpha value as a float
         */
        [[nodiscard]] float AlphaFloat() const;

        /*
         * Get the red value as an int
         */
        [[nodiscard]] int RedInt() const;

        /*
         * Get the green value as an int
         */
        [[nodiscard]] int GreenInt() const;

        /*
         * Get the blue value as an int
         */
        [[nodiscard]] int BlueInt() const;

        /*
         * Get the alpha value as an int
         */
        [[nodiscard]] int AlphaInt() const;

        #ifdef INCLUDE_RAYLIB
        /*
         * Convert to a raylib color
         */
        [[nodiscard]] Color ToRaylibColor() const;

        /*
         * Convert from a raylib color
         */
        static TColor FromRaylibColor(const Color &col_);

        #endif

        // Operators

        bool operator==(const TColor &b_) const;
        bool operator!=(const TColor &b_) const;

        // Predefined Colors

        static const TColor Beige;
        static const TColor Black;
        static const TColor Blue;
        static const TColor Brown;
        static const TColor DarkBlue;
        static const TColor DarkBrown;
        static const TColor DarkGray;
        static const TColor DarkGreen;
        static const TColor DarkPurple;
        static const TColor Gold;
        static const TColor Gray;
        static const TColor Green;
        static const TColor LightGray;
        static const TColor Lime;
        static const TColor Magenta;
        static const TColor Orange;
        static const TColor Pink;
        static const TColor Purple;
        static const TColor Red;
        static const TColor SkyBlue;
        static const TColor Transparent;
        static const TColor Violet;
        static const TColor White;
        static const TColor Yellow;
    };
}

#endif //COLOR_H
