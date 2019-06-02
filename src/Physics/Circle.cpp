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

#include <cute_c2.h>

namespace NerdThings::Ngine::Physics {
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
            collided = c2CircletoCircle({
                                            {
                                                Center.X,
                                                Center.Y
                                            },
                                            Radius
                                        },
                                        {
                                            {
                                                circle->Center.X,
                                                circle->Center.Y
                                            },
                                            circle->Radius
                                        });
        }

        // Bounding Box 2D against Circle
        if (boundingBox2D != nullptr) {
            collided = c2CircletoAABB({
                                          {
                                              Center.X,
                                              Center.Y
                                          },
                                          Radius
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
}
