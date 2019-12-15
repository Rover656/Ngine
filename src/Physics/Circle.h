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

#ifndef CIRCLE_H
#define CIRCLE_H

#include "../Ngine.h"

#include "../Vector2.h"
#include "CollisionShape.h"

namespace NerdThings::Ngine::Physics {
    /*
     * A circle shape
     */
    struct NEAPI Circle : public ICollisionShape {
    private:
        bool _isCompatible(ICollisionShape *shape_) override;
        bool _runCollisionCheck(ICollisionShape *shape_) override;
    public:
        /*
         * Circle center
         */
        Vector2 Center;

        /*
         * Circle radius
         */
        float Radius;

        /*
         * Create a circle
         */
        Circle()
            : Center(Vector2::Zero), Radius(0) {}

        /*
         * Create a circle
         */
        Circle(const Vector2 center_, const float radius_)
            : Center(center_), Radius(radius_) {}

#ifdef INCLUDE_BOX2D
        b2CircleShape ToB2Shape();
#endif
    };
}

#endif //CIRCLE_H
