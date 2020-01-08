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

#include "Rectangle.hpp"

namespace NerdThings::Ngine {
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
        const auto tl = Vector2(X, Y).Transform(origin_, rotation_);
        const auto tr = Vector2(X + Width, Y).Transform(origin_, rotation_);
        const auto bl = Vector2(X, Y + Height).Transform(origin_, rotation_);
        const auto br = Vector2(X + Width, Y + Height).Transform(origin_, rotation_);

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
            vertex = vertex.Transform(origin_, rotation_);
        }
        return Physics::Polygon(vertices);
    }

    Physics::Polygon *Rectangle::ToPolygonPtr(const float rotation_, Vector2 origin_) const {
        // Fix origin
        origin_ += {X, Y};

        // Make polygon
        std::vector<Vector2> vertices = {{X, Y}, {X + Width, Y}, {X + Width, Y + Height}, {X, Y + Height}};
        for (auto &vertex : vertices) {
            vertex = vertex.Transform(origin_, rotation_);
        }
        return new Physics::Polygon(vertices);
    }
}
