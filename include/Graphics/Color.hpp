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

#ifndef COLOR_HPP
#define COLOR_HPP

#include "../Config.hpp"

namespace NerdThings::Ngine::Graphics {
    /**
     * A 32-bit packed color.
     */
    struct NEAPI Color {
        /**
         * The red component.
         */
        float R;

        /**
         * The green component.
         */
        float G;

        /**
         * The blue component.
         */
        float B;

        /**
         * The alpha (transparency) component.
         */
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

#endif //COLOR_HPP
