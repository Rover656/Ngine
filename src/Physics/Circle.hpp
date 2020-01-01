/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "Shape.hpp"

namespace NerdThings::Ngine::Physics {
    /*
     * A circle shape
     */
    struct NEAPI Circle : public Shape {
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
