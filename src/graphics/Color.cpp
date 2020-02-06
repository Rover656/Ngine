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

#include "graphics/Color.hpp"

namespace ngine::graphics {
    Color::Color() : R(1), G(1), B(1), A(1) {}

    Color::Color(float r_, float g_, float b_, float a_)
            : R(r_), G(g_), B(b_), A(a_) {}

    Color::Color(const int r_, const int g_, const int b_, const int a_)
    : Color(r_ / 255.0f, g_ / 255.0f, b_ / 255.0f, a_ / 255.0f) {}

    bool Color::operator==(const Color &b_) const {
        return R == b_.R && G == b_.G && B == b_.B && A == b_.A;
    }

    bool Color::operator!=(const Color &b_) const {
        return R != b_.R && G != b_.G && B != b_.B && A != b_.A;
    }

    const Color Color::Beige = Color(245, 245, 220);
    const Color Color::Black = Color(0, 0, 0);
    const Color Color::Blue = Color(0, 0, 255);
    const Color Color::Brown = Color(165, 42, 42);
    const Color Color::DarkBlue = Color(0, 0, 139);
    const Color Color::DarkBrown = Color(76, 63, 47);
    const Color Color::DarkGray = Color(169, 169, 169);
    const Color Color::DarkGreen = Color(0, 100, 0);
    const Color Color::DarkPurple = Color(112, 31, 126);
    const Color Color::Gold = Color(255, 217, 0);
    const Color Color::Gray = Color(128, 128, 128);
    const Color Color::Green = Color(0, 128, 0);
    const Color Color::LightGray = Color(211, 211, 211);
    const Color Color::Lime = Color(0, 255, 0);
    const Color Color::Magenta = Color(255, 0, 255);
    const Color Color::Orange = Color(255, 166, 0);
    const Color Color::Pink = Color(255, 192, 203);
    const Color Color::Purple = Color(128, 0, 128);
    const Color Color::Red = Color(255, 0, 0);
    const Color Color::SkyBlue = Color(135, 207, 235);
    const Color Color::Transparent = Color(0, 0, 0, 0);
    const Color Color::Violet = Color(238, 130, 238);
    const Color Color::White = Color(255, 255, 255);
    const Color Color::Yellow = Color(255, 255, 0);
}
