/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef COLOR_H
#define COLOR_H

#include "../Ngine.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * A 32-bit packed color
     */
    struct NEAPI Color {
        // Public Fields

        /*
         * The unsigned int value of the color
         */
        unsigned int PackedValue;

        // Public Constructor(s)

        /*
         * Create the default color (White)
         */
        Color() : PackedValue(0xFFFFFFFF) {}

        /*
         * Create a color using an existing packed value
         */
        explicit Color(const unsigned int packedValue_) : PackedValue(packedValue_) {}

        /*
         * Create a color using float values between 0 and 1
         */
        Color(const float r_, const float g_, float b_, const float a_)
            : Color(static_cast<int>(r_ * 255),
                    static_cast<int>(g_ * 255),
                    static_cast<int>(b_ * 255),
                    static_cast<int>(a_ * 255)) {}

        /*
         * Create a color using int values between 0 and 255
         */
        Color(int r_, int g_, int b_, int a_);

        // Public Methods

        /*
         * Get the red value as a float
         */
        [[nodiscard]] float RedF() const;

        /*
         * Get the green value as a float
         */
        [[nodiscard]] float GreenF() const;

        /*
         * Get the blue value as a float
         */
        [[nodiscard]] float BlueF() const;

        /*
         * Get the alpha value as a float
         */
        [[nodiscard]] float AlphaF() const;

        /*
         * Get the red value as an int
         */
        [[nodiscard]] int RedI() const;

        /*
         * Get the green value as an int
         */
        [[nodiscard]] int GreenI() const;

        /*
         * Get the blue value as an int
         */
        [[nodiscard]] int BlueI() const;

        /*
         * Get the alpha value as an int
         */
        [[nodiscard]] int AlphaI() const;

        #ifdef INCLUDE_RAYLIB
        /*
         * Convert to a raylib color
         */
        [[nodiscard]] Color ToRaylibColor() const;

        /*
         * Convert from a raylib color
         */
        static Color FromRaylibColor(const Color &col_);

        #endif

        // Operators

        bool operator==(const Color &b_) const;
        bool operator!=(const Color &b_) const;

        // Predefined Colors

        static const Color Beige;
        static const Color Black;
        static const Color Blue;
        static const Color Brown;
        static const Color DarkBlue;
        static const Color DarkBrown;
        static const Color DarkGray;
        static const Color DarkGreen;
        static const Color DarkPurple;
        static const Color Gold;
        static const Color Gray;
        static const Color Green;
        static const Color LightGray;
        static const Color Lime;
        static const Color Magenta;
        static const Color Orange;
        static const Color Pink;
        static const Color Purple;
        static const Color Red;
        static const Color SkyBlue;
        static const Color Transparent;
        static const Color Violet;
        static const Color White;
        static const Color Yellow;
    };
}

#endif //COLOR_H
