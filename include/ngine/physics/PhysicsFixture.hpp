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

#ifndef PHYSICSFIXTURE_HPP
#define PHYSICSFIXTURE_HPP

#include "../Config.hpp"

#include "PhysicsShape.hpp"

// Box2D declare
class b2Fixture;

namespace ngine::physics {
    class PhysicsBody;

    /**
     * A physics fixture attaches a `PhysicsShape` to a `PhysicsBody`.
     */
    class NEAPI PhysicsFixture {
        friend class PhysicsBody;

        /**
         * The Box2D fixture.
         */
        b2Fixture *m_fixture = nullptr;

        /**
         * The body we are attached to.
         */
        PhysicsBody *m_body = nullptr;

        /**
         * Our shape.
         */
        PhysicsShape *m_shape = nullptr;

        /**
         * Context for unit conversions.
         */
        PhysicsContext *m_context = nullptr;

        /**
         * Create a new fixture.
         */
        PhysicsFixture(const PhysicsContext *context, b2Fixture *fixture);
    public:
        ~PhysicsFixture();

        /**
         * Get a reference to the shape.
         *
         * @return The shape. (Use its type to determine how to cast it).
         */
        PhysicsShape *getShape() const;
    };
}

#endif //PHYSICSFIXTURE_HPP
