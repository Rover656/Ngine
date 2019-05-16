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

#include <algorithm>

namespace Ngine {
    namespace Types {
        /*
         * A 32-bit packed color
         */
        struct TColor {
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
            explicit TColor(unsigned int packedValue_) : PackedValue(packedValue_) {}

            /*
             * Create a color using float values between 0 and 1
             */
            TColor(float r_, float g_, float b_, float a_)
                : TColor(static_cast<int>(r_ * 255), static_cast<int>(g_ * 255), static_cast<int>(b_ * 255),
                         static_cast<int>(a_ * 255)) {}

            /*
             * Create a color using int values between 0 and 255
             */
            TColor(int r_, int g_, int b_, int a_) {
                if (((r_ | g_ | b_ | a_) & 0xFFFFFFFF00) != 0) {
                    const auto clamped_r = static_cast<const unsigned int>(std::clamp(r_, 0, 255));
                    const auto clamped_g = static_cast<const unsigned int>(std::clamp(g_, 0, 255));
                    const auto clamped_b = static_cast<const unsigned int>(std::clamp(b_, 0, 255));
                    const auto clamped_a = static_cast<const unsigned int>(std::clamp(a_, 0, 255));

                    this->PackedValue = (clamped_a << 24) | (clamped_b << 16) | (clamped_g << 8) | (clamped_r);
                }
                else {
                    this->PackedValue = ((unsigned int)(a_) << 24) | ((unsigned int)(b_) << 16) | ((unsigned int)(g_) <<
                        8) | ((unsigned int)(r_));
                }
            }

            //Methods

            /*
             * Get the red value as a float
             */
            float FR() const {
                return static_cast<unsigned char>(PackedValue) / 255.0f;
            }

            /*
             * Get the green value as a float
             */
            float FG() const {
                return static_cast<unsigned char>(PackedValue >> 8) / 255.0f;
            }

            /*
             * Get the blue value as a float
             */
            float FB() const {
                return static_cast<unsigned char>(PackedValue >> 16) / 255.0f;
            }

            /*
             * Get the alpha value as a float
             */
            float FA() const {
                return static_cast<unsigned char>(PackedValue >> 24) / 255.0f;
            }

            /*
             * Get the red value as an int
             */
            int IR() const {
                return static_cast<unsigned char>(PackedValue);
            }

            /*
             * Get the green value as an int
             */
            int IG() const {
                return static_cast<unsigned char>(PackedValue >> 8);
            }

            /*
             * Get the blue value as an int
             */
            int IB() const {
                return static_cast<unsigned char>(PackedValue >> 16);
            }

            /*
             * Get the alpha value as an int
             */
            int IA() const {
                return static_cast<unsigned char>(PackedValue >> 24);
            }

            #ifdef INCLUDE_RAYLIB
            /*
             * Convert to a raylib color
             */
            Color ToRaylibColor() const {
                return {
                    static_cast<unsigned char>(IR()), 
                    static_cast<unsigned char>(IG()),
                    static_cast<unsigned char>(IB()), 
                    static_cast<unsigned char>(IA())
                };
            }
            #endif

            //Operators

            bool operator==(TColor b);
            bool operator!=(TColor b);
        };

        //Define colors

        const TColor Beige = TColor(0xffdcf5f5);
        const TColor Black = TColor(0xff000000);
        const TColor Blue = TColor(0xffff0000);
        const TColor Brown = TColor(0xff2a2aa5);
        const TColor DarkBlue = TColor(0xff8b0000);
        const TColor DarkBrown = TColor(76, 63, 47, 255);
        const TColor DarkGray = TColor(0xffa9a9a9);
        const TColor DarkGreen = TColor(0xff006400);
        const TColor DarkPurple = TColor(112, 31, 126, 255);
        const TColor Gold = TColor(0xff00d7ff);
        const TColor Gray = TColor(0xff808080);
        const TColor Green = TColor(0xff008000);
        const TColor LightGray = TColor(0xffd3d3d3);
        const TColor Lime = TColor(0xff00ff00);
        const TColor Magenta = TColor(0xffff00ff);
        const TColor Orange = TColor(0xff00a5ff);
        const TColor Pink = TColor(0xffcbc0ff);
        const TColor Purple = TColor(0xff800080);
        const TColor Red = TColor(0xff0000ff);
        const TColor SkyBlue = TColor(0xffebce87);
        const TColor Transparent = TColor(0);
        const TColor Violet = TColor(0xffee82ee);
        const TColor White = TColor(UINT_MAX);
        const TColor Yellow = TColor(0xff00ffff);
    }
}

#endif //COLOR_H
