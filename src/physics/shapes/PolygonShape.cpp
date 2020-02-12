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

#include "physics/shapes/PolygonShape.hpp"

#include "graphics/ShapeRenderer.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics::shapes {
    // Public Constructors

    PolygonShape::PolygonShape() : PhysicsShape() {
        _Shape = new b2PolygonShape();
    }

    PolygonShape::PolygonShape(b2PolygonShape *polygonShape) : PhysicsShape(polygonShape) {}

    PolygonShape::PolygonShape(PolygonShape &&old) : PhysicsShape(std::move(old)) {}

    PolygonShape::PolygonShape(const PolygonShape &polygon)
            : PhysicsShape(polygon) {
        // Create shape
        _Shape = new b2PolygonShape();

        // Copy data
        setCentroid(polygon.getCentroid());
        setVertices(polygon.getVertices());
        setNormals(polygon.getNormals());
        setRadius(polygon.getRadius());
    }

    // Public Methods

    PolygonShape PolygonShape::CreateAsBox(float hx, float hy) {
        PolygonShape s;
        s.setAsBox(hx, hy);
        return std::move(s);
    }

    void PolygonShape::debugDraw(graphics::Renderer *renderer, float ppm, Vector2 pos, float angle) const {
        auto vts = getVertices();
        auto c = getCentroid() + pos;
        for (auto i = 0; i < vts.size(); i++) {
            Vector2 v1 = vts[i];
            Vector2 v2 = (i == vts.size() - 1) ? vts[0] : vts[i + 1];

            // PPM scale
            v1 *= ppm;
            v2 *= ppm;

            // Apply scale
            v1 += pos;
            v2 += pos;

            // Apply rotation
            v1 = v1.transform(c, angle);
            v2 = v2.transform(c, angle);

            graphics::ShapeRenderer::DrawLine(renderer, v1, v2, graphics::Color::Red);
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

    void PolygonShape::setAsBox(float hx, float hy, const Vector2 &center, float rotation) {
        ((b2PolygonShape *) _Shape)->SetAsBox(hx, hy, {center.X, center.Y}, rotation);
    }

    void PolygonShape::setCentroid(Vector2 centroid) {
        ((b2PolygonShape *) _Shape)->m_centroid = {centroid.X, centroid.Y};
    }

    void PolygonShape::setNormals(std::vector<Vector2> norms) {
        for (auto i = 0; i < b2_maxPolygonVertices; i++) {
            if (i < norms.size()) {
                ((b2PolygonShape *) _Shape)->m_normals[i] = {norms[i].X, norms[i].Y};
            } else ((b2PolygonShape *) _Shape)->m_normals[i] = {0, 0};
        }
    }

    void PolygonShape::setVertices(std::vector<Vector2> verts) {
        // Create array
        auto arr = new b2Vec2[verts.size()];
        for (auto i = 0; i < verts.size(); i++) arr[i] = {verts[i].X, verts[i].Y};

        // Set
        ((b2PolygonShape *) _Shape)->Set(arr, verts.size());

        // Delete
        delete[] arr;
    }
}