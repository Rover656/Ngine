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

#include "BoundingBox.h"
#include "Polygon.h"
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

        TBoundingBox ToBoundingBox(float rotation_ = 0, TVector2 origin = TVector2::Zero) const {
            // Fix origin
            origin += {X, Y};

            // Get rectangle coords rotated
            auto tl = TVector2(X, Y).Rotate(origin, rotation_);
            auto tr = TVector2(X + Width, Y).Rotate(origin, rotation_);
            auto bl = TVector2(X, Y + Height).Rotate(origin, rotation_);
            auto br = TVector2(X + Width, Y + Height).Rotate(origin, rotation_);

            // Find min and max
            auto minX = std::min(tl.X, std::min(tr.X, std::min(bl.X, br.X)));
            auto maxX = std::max(tl.X, std::max(tr.X, std::max(bl.X, br.X)));
            auto minY = std::min(tl.Y, std::min(tr.Y, std::min(bl.Y, br.Y)));
            auto maxY = std::max(tl.Y, std::max(tr.Y, std::max(bl.Y, br.Y)));

            // Create bounding box
            TBoundingBox box;
            box.Min = {minX, minY};
            box.Max = {maxX, maxY};
            return box;
        }

        TPolygon ToPolygon(float rotation_ = 0, TVector2 origin = TVector2::Zero) {
            // Fix origin
            origin += {X, Y};

            // Make polygon
            std::vector<TVector2> vertices = {{X, Y}, {X + Width, Y}, {X + Width, Y + Height}, {X, Y + Height}};
            for (auto i = 0; i < vertices.size(); i++) {
                vertices[i] = vertices[i].Rotate(origin, rotation_);
            }
            return TPolygon(vertices);
        }
    };
}

#endif //RECTANGLE_H
