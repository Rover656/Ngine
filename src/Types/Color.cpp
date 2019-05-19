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

#include "Color.h"

// Public Constructor(s)

TColor::TColor(const int r_, const int g_, const int b_, const int a_) {
    if (((r_ | g_ | b_ | a_) & 0xFFFFFFFF00) != 0) {
        const auto clampedR = static_cast<const unsigned int>(std::clamp(r_, 0, 255));
        const auto clampedG = static_cast<const unsigned int>(std::clamp(g_, 0, 255));
        const auto clampedB = static_cast<const unsigned int>(std::clamp(b_, 0, 255));
        const auto clampedA = static_cast<const unsigned int>(std::clamp(a_, 0, 255));

        this->PackedValue = clampedA << 24
            | clampedB << 16
            | clampedG << 8
            | clampedR;
    }
    else {
        this->PackedValue = static_cast<unsigned int>(a_) << 24
            | static_cast<unsigned int>(b_) << 16
            | static_cast<unsigned int>(g_) << 8
            | static_cast<unsigned int>(r_);
    }
}

// Public Methods

float TColor::RedFloat() const {
    return static_cast<unsigned char>(PackedValue) / 255.0f;
}

float TColor::GreenFloat() const {
    return static_cast<unsigned char>(PackedValue >> 8) / 255.0f;
}

float TColor::BlueFloat() const {
    return static_cast<unsigned char>(PackedValue >> 16) / 255.0f;
}

float TColor::AlphaFloat() const {
    return static_cast<unsigned char>(PackedValue >> 24) / 255.0f;
}

int TColor::RedInt() const {
    return static_cast<unsigned char>(PackedValue);
}

int TColor::GreenInt() const {
    return static_cast<unsigned char>(PackedValue >> 8);
}

int TColor::BlueInt() const {
    return static_cast<unsigned char>(PackedValue >> 16);
}

int TColor::AlphaInt() const {
    return static_cast<unsigned char>(PackedValue >> 24);
}

#ifdef INCLUDE_RAYLIB // We don't really need this, but just to save any issues

Color TColor::ToRaylibColor() const {
    return {
        static_cast<unsigned char>(RedInt()),
        static_cast<unsigned char>(GreenInt()),
        static_cast<unsigned char>(BlueInt()),
        static_cast<unsigned char>(AlphaInt())
    };
}

TColor TColor::FromRaylibColor(const Color &col_) {
    return TColor(col_.r, col_.g, col_.b, col_.a);
}

#endif

// Operators

bool TColor::operator==(const TColor &b_) const {
    return PackedValue == b_.PackedValue;
}

bool TColor::operator!=(const TColor &b_) const {
    return PackedValue != b_.PackedValue;
}

// Predefined Colors

const TColor TColor::Beige = TColor(0xffdcf5f5);
const TColor TColor::Black = TColor(0xff000000);
const TColor TColor::Blue = TColor(0xffff0000);
const TColor TColor::Brown = TColor(0xff2a2aa5);
const TColor TColor::DarkBlue = TColor(0xff8b0000);
const TColor TColor::DarkBrown = TColor(76, 63, 47, 255);
const TColor TColor::DarkGray = TColor(0xffa9a9a9);
const TColor TColor::DarkGreen = TColor(0xff006400);
const TColor TColor::DarkPurple = TColor(112, 31, 126, 255);
const TColor TColor::Gold = TColor(0xff00d7ff);
const TColor TColor::Gray = TColor(0xff808080);
const TColor TColor::Green = TColor(0xff008000);
const TColor TColor::LightGray = TColor(0xffd3d3d3);
const TColor TColor::Lime = TColor(0xff00ff00);
const TColor TColor::Magenta = TColor(0xffff00ff);
const TColor TColor::Orange = TColor(0xff00a5ff);
const TColor TColor::Pink = TColor(0xffcbc0ff);
const TColor TColor::Purple = TColor(0xff800080);
const TColor TColor::Red = TColor(0xff0000ff);
const TColor TColor::SkyBlue = TColor(0xffebce87);
const TColor TColor::Transparent = TColor(0);
const TColor TColor::Violet = TColor(0xffee82ee);
const TColor TColor::White = TColor(0xffffffff);
const TColor TColor::Yellow = TColor(0xff00ffff);
