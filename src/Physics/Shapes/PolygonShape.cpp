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

#include "Physics/Shapes/PolygonShape.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/ShapeRenderer.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics::shapes {
    // Public Constructors

    PolygonShape::PolygonShape() : PhysicsShape() {
        _Shape = new b2PolygonShape();
    }

    PolygonShape::PolygonShape(b2PolygonShape *polygonShape_) : PhysicsShape(polygonShape_) {}

    PolygonShape::PolygonShape(PolygonShape &&old) : PhysicsShape(std::move(old)) {}

    PolygonShape::PolygonShape(const PolygonShape &polygon_)
            : PhysicsShape(polygon_) {
        // Create shape
        _Shape = new b2PolygonShape();

        // Copy data
        setCentroid(polygon_.getCentroid());
        setVertices(polygon_.getVertices());
        setNormals(polygon_.getNormals());
        setRadius(polygon_.getRadius());
    }

    // Public Methods

    PolygonShape PolygonShape::CreateAsBox(float hx, float hy) {
        PolygonShape s;
        s.setAsBox(hx, hy);
        return std::move(s);
    }

    void PolygonShape::debugDraw(graphics::Renderer *renderer_, float ppm_, Vector2 pos_, float angle_) const {
        auto vts = getVertices();
        auto c = getCentroid() + pos_;
        for (auto i = 0; i < vts.size(); i++) {
            Vector2 v1 = vts[i];
            Vector2 v2 = (i == vts.size() - 1) ? vts[0] : vts[i + 1];

            // PPM scale
            v1 *= ppm_;
            v2 *= ppm_;

            // Apply scale
            v1 += pos_;
            v2 += pos_;

            // Apply rotation
            v1 = v1.transform(c, angle_);
            v2 = v2.transform(c, angle_);

            graphics::ShapeRenderer::DrawLine(renderer_, v1, v2, graphics::Color::Red);
        }
    }

    Vector2 PolygonShape::getCentroid() const {
        auto c = ((b2PolygonShape *) _Shape)->m_centroid;
        return {c.x, c.y};
    }

    std::vector<Vector2> PolygonShape::getNormals() const {
        // Get shape
        auto p = ((b2PolygonShape *) _Shape);

        // Init vector
        std::vector<Vector2> norms(p->m_count);

        // Fill
        for (auto i = 0; i < p->m_count; i++) norms.emplace_back(p->m_normals[i].x, p->m_normals[i].y);

        return norms;
    }

    int PolygonShape::getVertexCount() const {
        return ((b2PolygonShape *) _Shape)->m_count;
    }

    std::vector<Vector2> PolygonShape::getVertices() const {
        // Get shape
        auto p = ((b2PolygonShape *) _Shape);

        // Init vector
        std::vector<Vector2> verts(p->m_count);

        // Fill
        for (auto i = 0; i < p->m_count; i++) verts[i] = {p->m_vertices[i].x, p->m_vertices[i].y};

        return verts;
    }

    void PolygonShape::setAsBox(float hx, float hy) {
        ((b2PolygonShape *) _Shape)->SetAsBox(hx, hy);
    }

    void PolygonShape::setAsBox(float hx, float hy, const Vector2 &center_, float rotation_) {
        ((b2PolygonShape *) _Shape)->SetAsBox(hx, hy, {center_.X, center_.Y}, rotation_);
    }

    void PolygonShape::setCentroid(Vector2 centroid_) {
        ((b2PolygonShape *) _Shape)->m_centroid = {centroid_.X, centroid_.Y};
    }

    void PolygonShape::setNormals(std::vector<Vector2> verts_) {
        for (auto i = 0; i < b2_maxPolygonVertices; i++) {
            if (i < verts_.size()) {
                ((b2PolygonShape *) _Shape)->m_normals[i] = {verts_[i].X, verts_[i].Y};
            } else ((b2PolygonShape *) _Shape)->m_normals[i] = {0, 0};
        }
    }

    void PolygonShape::setVertices(std::vector<Vector2> verts_) {
        // Create array
        auto arr = new b2Vec2[verts_.size()];
        for (auto i = 0; i < verts_.size(); i++) arr[i] = {verts_[i].X, verts_[i].Y};

        // Set
        ((b2PolygonShape *) _Shape)->Set(arr, verts_.size());

        // Delete
        delete[] arr;
    }
}