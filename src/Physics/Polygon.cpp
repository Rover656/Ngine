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

#include "Polygon.hpp"

#include <Box2D/Box2D.h>

#include <utility>

namespace NerdThings::Ngine::Physics {
    b2PolygonShape Polygon::createPolygonShape(std::vector<Vector2> verts_) {
        b2PolygonShape shp;
        shp.Set((b2Vec2 *)verts_.data(), verts_.size());
        return shp;
    }

    b2PolygonShape Polygon::createPolygonShape(std::vector<Vector2> verts_, std::vector<Vector2> normals_) {
        b2PolygonShape shp;
        shp.Set((b2Vec2 *)verts_.data(), verts_.size());
        for (auto i = 0; i < normals_.size(); i++)
            shp.m_normals[i] = {normals_[i].X, normals_[i].Y};
        return shp;
    }

    Polygon::Polygon(std::vector<Vector2> verts_) {
        Set(std::move(verts_));
    }

    Polygon::Polygon(const b2PolygonShape &shape_) {
        // Get count
        Count = shape_.m_count;

        // Get centroid
        Centroid = {shape_.m_centroid.x, shape_.m_centroid.y};

        // Get vertices
        auto verts = shape_.m_vertices;
        for (auto i = 0 ; i < shape_.m_count; i++) {
            Vertices[i] = {verts[i].x, verts[i].y};
        }
        // Get normals
        auto norms = shape_.m_normals;
        for (auto i = 0 ; i < shape_.m_count; i++) {
            Normals[i] = {norms[i].x, norms[i].y};
        }
    }

    void Polygon::Set(std::vector<Vector2> verts_) {
        // Save count
        Count = verts_.size();

        // Get normals from b2d
        auto shp = createPolygonShape(verts_);

        // Get normals
        auto norms = shp.m_normals;
        for (auto i = 0 ; i < shp.m_count; i++) {
            Normals[i] = {norms[i].x, norms[i].y};
        }

        // Get centroid
        Centroid = {shp.m_centroid.x, shp.m_centroid.y};
    }

    void Polygon::SetAsBox(float hx, float hy) {
        Count = 4;
        Vertices[0] = {-hx, -hy};
        Vertices[1] = {hx, -hy};
        Vertices[2] = {hx, hy};
        Vertices[3] = {-hx, hy};
        Normals[0] = {0, -1};
        Normals[1] = {1, 0};
        Normals[2] = {0, 1};
        Normals[3] = {-1, 0};
        Centroid = {0, 0};
    }
}
