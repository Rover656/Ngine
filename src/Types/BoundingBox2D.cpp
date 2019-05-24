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

#include "BoundingBox2D.h"

namespace NerdThings::Ngine {
    // Public Methods

    bool TBoundingBox2D::CheckCollision(ICollisionShape *shape_) {
        auto collided = false;

        auto boundingBox = dynamic_cast<TBoundingBox2D*>(shape_);

        if (boundingBox != nullptr) {
            // https://github.com/RandyGaul/cute_headers/blob/master/cute_c2.h#L1145
            const auto d0 = boundingBox->Max.X < Min.X;
            const auto d1 = Max.X < boundingBox->Min.X;
            const auto d2 = boundingBox->Max.Y < Min.Y;
            const auto d3 = Max.Y < boundingBox->Min.Y;
            collided = !(d0 | d1 | d2 | d3);
        }

        return collided;
    }
}