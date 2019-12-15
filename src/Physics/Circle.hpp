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

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "CollisionShape.hpp"

namespace NerdThings::Ngine::Physics {
    /*
     * A circle shape
     */
    struct NEAPI Circle : public ICollisionShape {
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
    };
}

#endif //CIRCLE_HPP
