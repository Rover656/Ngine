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

#include "Color.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    Color::Color(const int r_, const int g_, const int b_, const int a_) {
        if (((r_ | g_ | b_ | a_) & 0xFFFFFFFF00) != 0) {
            const auto clampedR = static_cast<const unsigned int>(std::clamp(r_, 0, 255));
            const auto clampedG = static_cast<const unsigned int>(std::clamp(g_, 0, 255));
            const auto clampedB = static_cast<const unsigned int>(std::clamp(b_, 0, 255));
            const auto clampedA = static_cast<const unsigned int>(std::clamp(a_, 0, 255));

            this->PackedValue = clampedA << 24
                | clampedB << 16
                | clampedG << 8
                | clampedR;
        } else {
            this->PackedValue = static_cast<unsigned int>(a_) << 24
                | static_cast<unsigned int>(b_) << 16
                | static_cast<unsigned int>(g_) << 8
                | static_cast<unsigned int>(r_);
        }
    }

    // Public Methods

    float Color::RedF() const {
        return static_cast<unsigned char>(PackedValue) / 255.0f;
    }

    float Color::GreenF() const {
        return static_cast<unsigned char>(PackedValue >> 8) / 255.0f;
    }

    float Color::BlueF() const {
        return static_cast<unsigned char>(PackedValue >> 16) / 255.0f;
    }

    float Color::AlphaF() const {
        return static_cast<unsigned char>(PackedValue >> 24) / 255.0f;
    }

    int Color::RedI() const {
        return static_cast<unsigned char>(PackedValue);
    }

    int Color::GreenI() const {
        return static_cast<unsigned char>(PackedValue >> 8);
    }

    int Color::BlueI() const {
        return static_cast<unsigned char>(PackedValue >> 16);
    }

    int Color::AlphaI() const {
        return static_cast<unsigned char>(PackedValue >> 24);
    }

    #ifdef INCLUDE_RAYLIB // We don't really need this, but just to save any issues

    Color Color::ToRaylibColor() const {
        return {
            static_cast<unsigned char>(RedInt()),
            static_cast<unsigned char>(GreenInt()),
            static_cast<unsigned char>(BlueInt()),
            static_cast<unsigned char>(AlphaInt())
        };
    }

    Color Color::FromRaylibColor(const Color &col_) {
        return Color(col_.r, col_.g, col_.b, col_.a);
    }

    #endif

    // Operators

    bool Color::operator==(const Color &b_) const {
        return PackedValue == b_.PackedValue;
    }

    bool Color::operator!=(const Color &b_) const {
        return PackedValue != b_.PackedValue;
    }

    // Predefined Colors

    const Color Color::Beige = Color(0xffdcf5f5);
    const Color Color::Black = Color(0xff000000);
    const Color Color::Blue = Color(0xffff0000);
    const Color Color::Brown = Color(0xff2a2aa5);
    const Color Color::DarkBlue = Color(0xff8b0000);
    const Color Color::DarkBrown = Color(76, 63, 47, 255);
    const Color Color::DarkGray = Color(0xffa9a9a9);
    const Color Color::DarkGreen = Color(0xff006400);
    const Color Color::DarkPurple = Color(112, 31, 126, 255);
    const Color Color::Gold = Color(0xff00d7ff);
    const Color Color::Gray = Color(0xff808080);
    const Color Color::Green = Color(0xff008000);
    const Color Color::LightGray = Color(0xffd3d3d3);
    const Color Color::Lime = Color(0xff00ff00);
    const Color Color::Magenta = Color(0xffff00ff);
    const Color Color::Orange = Color(0xff00a5ff);
    const Color Color::Pink = Color(0xffcbc0ff);
    const Color Color::Purple = Color(0xff800080);
    const Color Color::Red = Color(0xff0000ff);
    const Color Color::SkyBlue = Color(0xffebce87);
    const Color Color::Transparent = Color(0);
    const Color Color::Violet = Color(0xffee82ee);
    const Color Color::White = Color(0xffffffff);
    const Color Color::Yellow = Color(0xff00ffff);
}
