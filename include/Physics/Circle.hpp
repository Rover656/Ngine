/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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
