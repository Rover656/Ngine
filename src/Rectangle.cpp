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

#include "Rectangle.h"

namespace NerdThings::Ngine {
    // Public Methods

    #ifdef INCLUDE_RAYLIB

    Rectangle Rectangle::ToRaylibRect() const {
        return {X, Y, Width, Height};
    }

    Rectangle Rectangle::FromRaylibRect(const Rectangle &rect_) {
        return {rect_.x, rect_.y, rect_.width, rect_.height};
    }

    #endif

    bool Rectangle::Contains(Vector2 point_) const {
        return X <= point_.X
            && point_.X < X + Width
            && Y <= point_.Y
            && point_.Y < Y + Height;
    }

    Physics::BoundingBox Rectangle::ToBoundingBox(const float rotation_, Vector2 origin_) const {
        // Fix origin
        origin_ += {X, Y};

        // Get rectangle coords rotated
        const auto tl = Vector2(X, Y).Rotate(origin_, rotation_);
        const auto tr = Vector2(X + Width, Y).Rotate(origin_, rotation_);
        const auto bl = Vector2(X, Y + Height).Rotate(origin_, rotation_);
        const auto br = Vector2(X + Width, Y + Height).Rotate(origin_, rotation_);

        // Find min and max
        auto minX = std::min(tl.X, std::min(tr.X, std::min(bl.X, br.X)));
        auto maxX = std::max(tl.X, std::max(tr.X, std::max(bl.X, br.X)));
        auto minY = std::min(tl.Y, std::min(tr.Y, std::min(bl.Y, br.Y)));
        auto maxY = std::max(tl.Y, std::max(tr.Y, std::max(bl.Y, br.Y)));

        // Create bounding box
        Physics::BoundingBox box;
        box.Min = {minX, minY};
        box.Max = {maxX, maxY};
        return box;
    }

    Physics::BoundingBox *Rectangle::ToBoundingBoxPtr(float rotation_, Vector2 origin_) const {
        auto valbbox = ToBoundingBox(rotation_, origin_);
        auto bbox = new Physics::BoundingBox();
        bbox->Min = valbbox.Min;
        bbox->Max = valbbox.Max;
        return bbox;
    }

    Physics::Polygon Rectangle::ToPolygon(const float rotation_, Vector2 origin_) const {
        // Fix origin
        origin_ += {X, Y};

        // Make polygon
        std::vector<Vector2> vertices = {{X, Y}, {X + Width, Y}, {X + Width, Y + Height}, {X, Y + Height}};
        for (auto &vertex : vertices) {
            vertex = vertex.Rotate(origin_, rotation_);
        }
        return Physics::Polygon(vertices);
    }

    Physics::Polygon *Rectangle::ToPolygonPtr(const float rotation_, Vector2 origin_) const {
        // Fix origin
        origin_ += {X, Y};

        // Make polygon
        std::vector<Vector2> vertices = {{X, Y}, {X + Width, Y}, {X + Width, Y + Height}, {X, Y + Height}};
        for (auto &vertex : vertices) {
            vertex = vertex.Rotate(origin_, rotation_);
        }
        return new Physics::Polygon(vertices);
    }
}
