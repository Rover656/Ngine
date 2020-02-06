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

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Config.hpp"

#include "Physics/BoundingBox.hpp"
#include "Physics/Polygon.hpp"
#include "Math.hpp"

namespace ngine {
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
        bool contains(Vector2 point_) const;

        /*
         * Convert rectangle to bounding box
         */
        physics::BoundingBox toBoundingBox(float rotation_ = 0, Vector2 origin_ = Vector2::Zero) const;

        /*
         * Convert rectangle to bounding box pointer.
         * Must be deleted
         */
        physics::BoundingBox *toBoundingBoxPtr(float rotation_ = 0, Vector2 origin_ = Vector2::Zero) const;

        /*
         * Convert rectangle to polygon
         */
        physics::Polygon toPolygon(float rotation_ = 0, Vector2 origin_ = Vector2::Zero) const;

        /*
         * Convert rectangle to polygon pointer.
         * Must be deleted.
         */
        physics::Polygon *toPolygonPtr(float rotation_ = 0, Vector2 origin_ = Vector2::Zero) const;
    };
}

#endif //RECTANGLE_HPP
