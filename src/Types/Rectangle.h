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

#ifndef RECTANGLE_H
#define RECTANGLE_H

// These will be enabled and a source file added once we add collisions? Maybe? IDK.
// #define TYPE_DECL_HEADER
// #include "../ngine.h"
// #undef TYPE_DECL_HEADER

#include "Vector2.h"

namespace NerdThings::Ngine {
    /*
     * A rectangle
     */
    struct TRectangle {
        // Public Fields

        /*
         * Rectangle X position
         */
        float X;

        /*
         * Rectangle Y position
         */
        float Y;

        /*
         * Rectangle Width
         */
        float Width;

        /*
         * Rectangle Height
         */
        float Height;

        // Public Constructor(s)

        TRectangle()
            : X(0), Y(0), Width(0), Height(0) {}

        TRectangle(float x_, float y_, float width_, float height_)
            : X(x_), Y(y_), Width(width_), Height(height_) {}

        TRectangle(TVector2 position_, float width_, float height_)
            : X(position_.X), Y(position_.Y), Width(width_), Height(height_) {}

        TRectangle(TVector2 position_, TVector2 size_)
            : X(position_.X), Y(position_.Y), Width(size_.X), Height(size_.Y) {}

        // Public Methods
        #ifdef INCLUDE_RAYLIB

        /*
         * Convert to Raylib Rectangle
         */
        Rectangle ToRaylibRect() const {
            return {X, Y, Width, Height};
        }

        /*
         * Convert from Raylib rectangle
         */
        static TRectangle FromRaylibRect(const Rectangle &rect) {
            return {rect.x, rect.y, rect.width, rect.height};
        }

        #endif
    };
}

#endif //RECTANGLE_H
