/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Polygon.h"

#include "BoundingBox.h"
#include "Circle.h"

namespace NerdThings::Ngine::Physics {
    // Private Methods

    bool Polygon::IsCompatible(ICollisionShape *shape_) {
        const auto boundingBox2D = dynamic_cast<BoundingBox*>(shape_);
        const auto circle = dynamic_cast<Circle*>(shape_);
        const auto polygon = dynamic_cast<Polygon*>(shape_);

        if (boundingBox2D != nullptr)
            return true;
        if (circle != nullptr)
            return true;
        if (polygon != nullptr)
            return true;
        return false;
    }

    bool Polygon::RunCollisionCheck(ICollisionShape *shape_) {
        auto collided = false;

        const auto boundingBox2D = dynamic_cast<BoundingBox*>(shape_);
        const auto circle = dynamic_cast<Circle*>(shape_);
        const auto polygon = dynamic_cast<Polygon*>(shape_);

        auto myPoly = ToB2Shape();

        if (boundingBox2D != nullptr) {
            auto theirAABB = boundingBox2D->ToB2Shape();
            collided = b2TestOverlap(&myPoly, &theirAABB);
        } else if (circle != nullptr) {
            auto theirCircle = circle->ToB2Shape();
            collided = b2TestOverlap(&myPoly, &theirCircle);
        } else if (polygon != nullptr) {
            auto theirPoly = polygon->ToB2Shape();
            collided = b2TestOverlap(&myPoly, &theirPoly);
        }

        return collided;
    }

    // Public Methods

#ifdef INCLUDE_BOX2D
    b2PolygonShape Polygon::ToB2Shape() {
        b2PolygonShape tmpShape;
        b2Vec2 vertices[b2_maxPolygonVertices];
        for (auto i = 0; i < VertexCount; i++) vertices[i] = {Vertices[i].X, Vertices[i].Y};
        tmpShape.Set(vertices, VertexCount);
        return tmpShape;
    }
#endif

    void Polygon::GenerateNormals() {
        // Create c2 poly
        auto tmpPoly = ToB2Shape();

        // Bring in normals
        for (auto i = 0; i < VertexCount; i++) Normals[i] = {tmpPoly.m_normals[i].x, tmpPoly.m_normals[i].y};
    }
}
