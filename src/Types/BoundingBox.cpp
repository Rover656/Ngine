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

#include "../3rd-party/cute-headers/cute_c2.h"

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
            collided = c2AABBtoAABB({
                                        {
                                            Min.X,
                                            Min.Y
                                        },
                                        {
                                            Max.X,
                                            Max.Y
                                        }
                                    },
                                    {
                                        {
                                            boundingBox2D->Min.X,
                                            boundingBox2D->Min.Y
                                        },
                                        {
                                            boundingBox2D->Max.X,
                                            boundingBox2D->Max.Y
                                        }
                                    });
        }

        return collided;
    }

    // Public Fields

    void TBoundingBox::Offset(TVector2 offset_) {
        Max += offset_;
        Min += offset_;
    }
}
