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

#include "../ngine.h"

#include "../Physics/BoundingBox.h"
#include "../Physics/Polygon.h"
#include "Vector2.h"

namespace NerdThings::Ngine::Math {
    /*
     * A rectangle
     */
    struct NEAPI TRectangle {
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
        Rectangle ToRaylibRect() const;

        /*
         * Convert from Raylib rectangle
         */
        static TRectangle FromRaylibRect(const Rectangle &rect_);

        #endif

        /*
         * Convert rectangle to bounding box
         */
        Physics::TBoundingBox ToBoundingBox(float rotation_ = 0, TVector2 origin_ = TVector2::Zero) const;

        /*
         * Convert rectangle to polygon
         */
        Physics::TPolygon ToPolygon(float rotation_ = 0, TVector2 origin_ = TVector2::Zero);
    };
}

#endif //RECTANGLE_H
