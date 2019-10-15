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

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Ngine.h"

#include "Physics/BoundingBox.h"
#include "Physics/Polygon.h"
#include "Vector2.h"

namespace NerdThings::Ngine {
    /*
     * A rectangle
     */
    struct NEAPI Rectangle {
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

        Rectangle()
            : X(0), Y(0), Width(0), Height(0) {}

        Rectangle(float x_, float y_, float width_, float height_)
            : X(x_), Y(y_), Width(width_), Height(height_) {}

        Rectangle(Vector2 position_, float width_, float height_)
            : X(position_.X), Y(position_.Y), Width(width_), Height(height_) {}

        Rectangle(Vector2 position_, Vector2 size_)
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
        static Rectangle FromRaylibRect(const Rectangle &rect_);

        #endif

        /*
         * Whether or not the rectangle contains the point
         */
        bool Contains(Vector2 point_) const;

        /*
         * Convert rectangle to bounding box
         */
        Physics::BoundingBox ToBoundingBox(float rotation_ = 0, Vector2 origin_ = Vector2::Zero) const;

        /*
         * Convert rectangle to bounding box pointer.
         * Must be deleted
         */
        Physics::BoundingBox *ToBoundingBoxPtr(float rotation_ = 0, Vector2 origin_ = Vector2::Zero) const;

        /*
         * Convert rectangle to polygon
         */
        Physics::Polygon ToPolygon(float rotation_ = 0, Vector2 origin_ = Vector2::Zero) const;

        /*
         * Convert rectangle to polygon pointer.
         * Must be deleted.
         */
        Physics::Polygon *ToPolygonPtr(float rotation_ = 0, Vector2 origin_ = Vector2::Zero) const;
    };
}

#endif //RECTANGLE_H
