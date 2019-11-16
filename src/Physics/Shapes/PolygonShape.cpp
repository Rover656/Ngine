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

#include "PolygonShape.h"

#include "../../Graphics/Renderer.h"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>

namespace NerdThings::Ngine::Physics::Shapes {
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
        SetCentroid(polygon_.GetCentroid());
        SetVertices(polygon_.GetVertices());
        SetNormals(polygon_.GetNormals());
        SetRadius(polygon_.GetRadius());
    }

    // Public Methods

    PolygonShape PolygonShape::CreateAsBox(float hx, float hy) {
        PolygonShape s;
        s.SetAsBox(hx, hy);
        return std::move(s);
    }

    void PolygonShape::DebugDraw(float ppm_, Vector2 pos_, float angle_) const {
        auto vts = GetVertices();
        auto c = GetCentroid() + pos_;
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
            v1 = v1.Rotate(c, angle_);
            v2 = v2.Rotate(c, angle_);

            Graphics::Renderer::DrawLine(v1, v2, Graphics::Color::Red);
        }
    }

    Vector2 PolygonShape::GetCentroid() const {
        auto c = ((b2PolygonShape *) _Shape)->m_centroid;
        return {c.x, c.y};
    }

    std::vector<Vector2> PolygonShape::GetNormals() const {
        // Get shape
        auto p = ((b2PolygonShape *) _Shape);

        // Init vector
        std::vector<Vector2> norms(p->m_count);

        // Fill
        for (auto i = 0; i < p->m_count; i++) norms.emplace_back(p->m_normals[i].x, p->m_normals[i].y);

        return norms;
    }

    int PolygonShape::GetVertexCount() const {
        return ((b2PolygonShape *) _Shape)->m_count;
    }

    std::vector<Vector2> PolygonShape::GetVertices() const {
        // Get shape
        auto p = ((b2PolygonShape *) _Shape);

        // Init vector
        std::vector<Vector2> verts(p->m_count);

        // Fill
        for (auto i = 0; i < p->m_count; i++) verts[i] = {p->m_vertices[i].x, p->m_vertices[i].y};

        return verts;
    }

    void PolygonShape::SetAsBox(float hx, float hy) {
        ((b2PolygonShape *) _Shape)->SetAsBox(hx, hy);
    }

    void PolygonShape::SetAsBox(float hx, float hy, const Vector2 &center_, float rotation_) {
        ((b2PolygonShape *) _Shape)->SetAsBox(hx, hy, {center_.X, center_.Y}, rotation_);
    }

    void PolygonShape::SetCentroid(Vector2 centroid_) {
        ((b2PolygonShape *) _Shape)->m_centroid = {centroid_.X, centroid_.Y};
    }

    void PolygonShape::SetNormals(std::vector<Vector2> verts_) {
        for (auto i = 0; i < b2_maxPolygonVertices; i++) {
            if (i < verts_.size()) {
                ((b2PolygonShape *) _Shape)->m_normals[i] = {verts_[i].X, verts_[i].Y};
            } else ((b2PolygonShape *) _Shape)->m_normals[i] = {0, 0};
        }
    }

    void PolygonShape::SetVertices(std::vector<Vector2> verts_) {
        // Create array
        auto arr = new b2Vec2[verts_.size()];
        for (auto i = 0; i < verts_.size(); i++) arr[i] = {verts_[i].X, verts_[i].Y};

        // Set
        ((b2PolygonShape *) _Shape)->Set(arr, verts_.size());

        // Delete
        delete[] arr;
    }
}