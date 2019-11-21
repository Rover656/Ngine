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
    /**
     * A 32-bit packed color.
     */
    struct NEAPI Color {
        float R;
        float G;
        float B;
        float A;

        /**
         * Create the default color (White).
         */
        Color();

        /**
         * Create a color using float values between 0 and 1.
         *
         * @param r_ The red value.
         * @param g_ The green value.
         * @param b_ The blue value.
         * @param a_ The alpha (transparency) value.
         */
        Color(float r_, float g_, float b_, float a_ = 1);

        /**
         * Create a color using int values between 0 and 255.
         *
         * @param r_ The red value.
         * @param g_ The green value.
         * @param b_ The blue value.
         * @param a_ The alpha (transparency) value.
         */
        Color(int r_, int g_, int b_, int a_ = 255);

        bool operator==(const Color &b_) const;
        bool operator!=(const Color &b_) const;

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
