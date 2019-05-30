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

namespace NerdThings::Ngine {
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
        if (boundingBox2D != nullptr) {
            // https://github.com/RandyGaul/cute_headers/blob/master/cute_c2.h#L1145
            const auto d0 = boundingBox2D->Max.X < Min.X;
            const auto d1 = Max.X < boundingBox2D->Min.X;
            const auto d2 = boundingBox2D->Max.Y < Min.Y;
            const auto d3 = Max.Y < boundingBox2D->Min.Y;
            collided = !(d0 | d1 | d2 | d3);
        }

        return collided;
    }
}
