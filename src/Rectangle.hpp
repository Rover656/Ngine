/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Config.hpp"

#include "Physics/BoundingBox.hpp"
#include "Physics/Polygon.hpp"
#include "Math.hpp"

namespace NerdThings::Ngine {
    /*
     * A rectangle
     */
    struct NEAPI Rectangle {
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

        Rectangle()
            : X(0), Y(0), Width(0), Height(0) {}

        Rectangle(float x_, float y_, float width_, float height_)
            : X(x_), Y(y_), Width(width_), Height(height_) {}

        Rectangle(Vector2 position_, float width_, float height_)
            : X(position_.X), Y(position_.Y), Width(width_), Height(height_) {}

        Rectangle(Vector2 position_, Vector2 size_)
            : X(position_.X), Y(position_.Y), Width(size_.X), Height(size_.Y) {}

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

#endif //RECTANGLE_HPP
