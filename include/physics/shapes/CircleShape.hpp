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

#ifndef CIRCLESHAPE_HPP
#define CIRCLESHAPE_HPP

#include "../../Config.hpp"

#include "PhysicsShape.hpp"

class b2CircleShape;

namespace ngine::physics::shapes {
    class NEAPI CircleShape : public PhysicsShape {
    public:
        // Public Constructors

        /*
         * Create a circle shape.
         */
        CircleShape();

        /*
         * Create a circle shape.
         */
        explicit CircleShape(float radius, Vector2 position = Vector2::Zero);

        /*
         * Create a circle shape from a Box2D owned circle.
         */
        explicit CircleShape(b2CircleShape *circle);

        /*
         * Move constructor
         */
        CircleShape(CircleShape &&old);

        /*
         * Copy constructor
         */
        CircleShape(const CircleShape &circle);

        // Public Methods

        void debugDraw(graphics::Renderer *renderer, float ppm, Vector2 pos, float angle) const override;

        /*
         * Get circle position
         */
        Vector2 getPosition() const;

        /*
         * Set circle position
         */
        void setPosition(Vector2 pos);
    };
}

#endif //CIRCLESHAPE_HPP
