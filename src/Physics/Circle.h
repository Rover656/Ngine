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

#ifndef CIRCLE_H
#define CIRCLE_H

#include "../ngine.h"

#include "../Vector2.h"
#include "CollisionShape.h"

namespace NerdThings::Ngine::Physics {
    struct NEAPI TCircle : public ICollisionShape {
    private:
        // Private Methods

        bool IsCompatible(ICollisionShape *shape_) override;

        bool RunCollisionCheck(ICollisionShape *shape_) override;
    public:
        // Public Fields

        /*
         * Circle center
         */
        TVector2 Center;

        /*
         * Circle radius
         */
        float Radius;

        // Public Constructor(s)

        /*
         * Create a circle
         */
        TCircle()
            : Center(TVector2::Zero), Radius(0) {}

        /*
         * Create a circle
         */
        TCircle(const TVector2 center_, const float radius_)
            : Center(center_), Radius(radius_) {}

        // Public Methods

#ifdef INCLUDE_BOX2D
        b2CircleShape ToB2Shape();
#endif
    };
}

#endif //CIRCLE_H
