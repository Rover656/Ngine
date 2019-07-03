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

#include "BoundingBox.h"

namespace NerdThings::Ngine::Physics {
    // Private Methods

    bool TBoundingBox::IsCompatible(ICollisionShape *shape_) {
        const auto boundingBox2D = dynamic_cast<TBoundingBox*>(shape_);

        // Bounding Box 2D against Bounding Box 2D
        if (boundingBox2D != nullptr)
            return true;

        // No support for circle, bb2d against circle implemented in circle struct

        return false;
    }

    bool TBoundingBox::RunCollisionCheck(ICollisionShape *shape_) {
        auto collided = false;

        auto boundingBox2D = dynamic_cast<TBoundingBox*>(shape_);

        // Bounding Box 2D against Bounding Box 2D
        auto myAABB = ToB2Shape();
        if (boundingBox2D != nullptr) {
            auto theirAABB = boundingBox2D->ToB2Shape();
            collided = b2TestOverlap(&myAABB, &theirAABB);
        }

        return collided;
    }

    // Public Methods

#ifdef INCLUDE_BOX2D

    b2PolygonShape TBoundingBox::ToB2Shape() {
        b2PolygonShape tmpShape;
        b2Vec2 vertices[4];

        vertices[0] = {Min.X, Min.Y};
        vertices[1] = {Min.X, Max.Y};
        vertices[2] = {Max.X, Max.Y};
        vertices[3] = {Max.X, Min.Y};

        tmpShape.Set(vertices, 4);
        return tmpShape;
    }

#endif
}
