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

#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "Shape.hpp"

#define MAX_POLY_VERTS 8

struct b2Shape;
struct b2PolygonShape;

namespace NerdThings::Ngine::Physics {
    /**
     * A Polygon shape.
     *
     * Powered by Box2D Math.
     */
    struct NEAPI Polygon : public Shape {
        static b2PolygonShape createPolygonShape(std::vector<Vector2> verts_);
        static b2PolygonShape createPolygonShape(std::vector<Vector2> verts_, std::vector<Vector2> normals_);
    public:
        Vector2 Vertices[MAX_POLY_VERTS];
        Vector2 Normals[MAX_POLY_VERTS];
        Vector2 Centroid;
        int Count;

        Polygon(std::vector<Vector2> verts_);
        Polygon(const b2PolygonShape &shape_);

        void Set(std::vector<Vector2> verts_);
        void SetAsBox(float hx, float hy);
    };
}

#endif //POLYGON_HPP
