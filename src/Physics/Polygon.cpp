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

#include <cute_c2.h>

#include "BoundingBox.h"
#include "Circle.h"

// Helper functions

c2Poly createC2Poly(int count, NerdThings::Ngine::Math::TVector2 data[]) {
    c2Poly tmpPoly;
    tmpPoly.count = count;
    for (auto i = 0; i < count; i++) tmpPoly.verts[i] = {data[i].X, data[i].Y};
    return tmpPoly;
}

c2Poly createC2Poly(int count, NerdThings::Ngine::Math::TVector2 data[], NerdThings::Ngine::Math::TVector2 norms[]) {
    c2Poly tmpPoly = createC2Poly(count, data);
    for (auto i = 0; i < count; i++) tmpPoly.norms[i] = {norms[i].X, norms[i].Y};
    return tmpPoly;
}

namespace NerdThings::Ngine::Physics {
    // Private Methods

    bool TPolygon::IsCompatible(ICollisionShape *shape_) {
        const auto boundingBox2D = dynamic_cast<TBoundingBox*>(shape_);
        const auto circle = dynamic_cast<TCircle*>(shape_);
        const auto polygon = dynamic_cast<TPolygon*>(shape_);

        if (boundingBox2D != nullptr)
            return true;
        if (circle != nullptr)
            return true;
        if (polygon != nullptr)
            return true;
        return false;
    }

    bool TPolygon::RunCollisionCheck(ICollisionShape *shape_) {
        auto collided = false;

        const auto boundingBox2D = dynamic_cast<TBoundingBox*>(shape_);
        const auto circle = dynamic_cast<TCircle*>(shape_);
        const auto polygon = dynamic_cast<TPolygon*>(shape_);

        auto myPoly = createC2Poly(VertexCount, Vertices, Normals);

        if (boundingBox2D != nullptr) {
            // Bounding 
            collided = c2AABBtoPoly({
                                        {
                                            boundingBox2D->Min.X,
                                            boundingBox2D->Min.Y
                                        },
                                        {
                                            boundingBox2D->Max.X,
                                            boundingBox2D->Max.Y
                                        }
                                    },
                                    &myPoly,
                                    0);
        } else if (circle != nullptr) {
            collided = c2CircletoPoly({
                                          {
                                              circle->Center.X,
                                              circle->Center.Y
                                          },
                                          circle->Radius
                                      },
                                      &myPoly,
                                      0);
        } else if (polygon != nullptr) {
            auto theirPoly = createC2Poly(polygon->VertexCount, polygon->Vertices, polygon->Normals);
            collided = c2PolytoPoly(&myPoly, 0, &theirPoly, 0);
        }

        return collided;
    }

    // Public Methods

    void TPolygon::GenerateNormals() {
        // Create c2 poly
        c2Poly tmpPoly = createC2Poly(VertexCount, Vertices);

        // Make vertices
        c2MakePoly(&tmpPoly);

        // Bring back in our data
        for (auto i = 0; i < VertexCount; i++) Vertices[i] = {tmpPoly.verts[i].x, tmpPoly.verts[i].y};
    }
}
