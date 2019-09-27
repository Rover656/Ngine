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

#include "Circle.h"

#include "BoundingBox.h"

namespace NerdThings::Ngine::Physics {
    // Private Methods

    bool Circle::IsCompatible(ICollisionShape *shape_) {
        const auto boundingBox2D = dynamic_cast<BoundingBox*>(shape_);
        const auto circle = dynamic_cast<Circle*>(shape_);

        // Circle against Circle
        if (circle != nullptr)
            return true;

        // Bounding Box 2D against Circle
        if (boundingBox2D != nullptr)
            return true;

        return false;
    }

    bool Circle::RunCollisionCheck(ICollisionShape *shape_) {
        auto collided = false;

        auto boundingBox2D = dynamic_cast<BoundingBox*>(shape_);
        const auto circle = dynamic_cast<Circle*>(shape_);

        // Circle against Circle

        auto myCircle = ToB2Shape();
        if (circle != nullptr) {
            auto theirCircle = circle->ToB2Shape();
            collided = b2TestOverlap(&myCircle, &theirCircle);
        }

        // Bounding Box 2D against Circle
        if (boundingBox2D != nullptr) {
            auto theirAABB = boundingBox2D->ToB2Shape();
            collided = b2TestOverlap(&myCircle, &theirAABB);
        }

        return collided;
    }

    // Public Methods

#ifdef INCLUDE_BOX2D
    b2CircleShape Circle::ToB2Shape() {
        b2CircleShape shape;
        shape.m_p.Set(Center.X, Center.Y);
        shape.m_radius = Radius;
        return shape;
    }
#endif
}
