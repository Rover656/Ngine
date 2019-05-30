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

namespace NerdThings::Ngine {
    // Private Methods

    bool TCircle::IsCompatible(ICollisionShape *shape_) {
        const auto boundingBox2D = dynamic_cast<TBoundingBox*>(shape_);
        const auto circle = dynamic_cast<TCircle*>(shape_);

        // Circle against Circle
        if (circle != nullptr)
            return true;

        // Bounding Box 2D against Circle
        if (boundingBox2D != nullptr)
            return true;

        return false;
    }

    bool TCircle::RunCollisionCheck(ICollisionShape *shape_) {
        auto collided = false;

        auto boundingBox2D = dynamic_cast<TBoundingBox*>(shape_);
        const auto circle = dynamic_cast<TCircle*>(shape_);

        // Circle against Circle
        if (circle != nullptr) {
            // https://github.com/RandyGaul/cute_headers/blob/master/cute_c2.h#L1127
            auto c = Center - circle->Center;
            float d2 = c.Dot(c);
            float r2 = Radius + circle->Radius;
            r2 = r2 * r2;
            collided = d2 < r2;
        }

        // Bounding Box 2D against Circle
        if (boundingBox2D != nullptr) {
            // https://github.com/RandyGaul/cute_headers/blob/master/cute_c2.h#L1136
            auto L = TVector2::Clamp(Center, boundingBox2D->Min, boundingBox2D->Max);
            auto ab = Center - L;
            float d2 = ab.Dot(ab);
            float r2 = Radius * Radius;
            collided = d2 < r2;
        }

        return collided;
    }
}
