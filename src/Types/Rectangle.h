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

#include "BoundingBox2D.h"
#include "Matrix.h"
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

        TBoundingBox2D ToBoundingBox(float rotation_) {
            // Create translation matrix
            auto mat = TMatrix::RotateZ(rotation_);

            // Transform all coordinates
            TVector2 topleft = {X, Y};
            TVector2 topright = {X + Width, Y};
            TVector2 bottomleft = {X, Y + Height};
            TVector2 bottomright = {X + Width, Y + Height};

            topleft = topleft.Transform(mat);
            topright = topright.Transform(mat);
            bottomleft = bottomleft.Transform(mat);
            bottomright = bottomright.Transform(mat);

            // Find min x
            auto min_x = topleft.X;

            if (min_x > topright.X) {
                min_x = topright.X;
            }

            if (min_x > bottomleft.X) {
                min_x = bottomleft.X;
            }

            if (min_x > bottomright.X) {
                min_x = bottomright.X;
            }

            // Find max x
            auto max_x = topleft.X;

            if (max_x < topright.X) {
                max_x = topright.X;
            }

            if (max_x < bottomleft.X) {
                max_x = bottomleft.X;
            }

            if (max_x < bottomright.X) {
                max_x = bottomright.X;
            }

            // Find min y
            auto min_y = topleft.Y;

            if (min_y > topright.Y) {
                min_y = topright.Y;
            }

            if (min_y > bottomleft.Y) {
                min_y = bottomleft.Y;
            }

            if (min_y > bottomright.Y) {
                min_y = bottomright.Y;
            }

            // Find max y
            auto max_y = topleft.Y;

            if (max_y < topright.Y) {
                max_y = topright.Y;
            }

            if (max_y < bottomleft.Y) {
                max_y = bottomleft.Y;
            }

            if (max_y < bottomright.Y) {
                max_y = bottomright.Y;
            }

            // Create bounding box
            TBoundingBox2D box;
            box.Min = {min_x, min_y};
            box.Max = {max_x, max_y};
            return box;
        }
    };
}

#endif //RECTANGLE_H
