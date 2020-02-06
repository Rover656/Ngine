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
        explicit CircleShape(float radius_, Vector2 position_ = Vector2::Zero);

        /*
         * Create a circle shape from a Box2D owned circle.
         */
        explicit CircleShape(b2CircleShape *circle_);

        /*
         * Move constructor
         */
        CircleShape(CircleShape &&old);

        /*
         * Copy constructor
         */
        CircleShape(const CircleShape &circle_);

        // Public Methods

        void debugDraw(graphics::Renderer *renderer_, float ppm_, Vector2 pos_, float angle_) const override;

        /*
         * Get circle position
         */
        Vector2 getPosition() const;

        /*
         * Set circle position
         */
        void setPosition(Vector2 pos_);
    };
}

#endif //CIRCLESHAPE_HPP
