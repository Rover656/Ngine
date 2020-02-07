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

#include "shapes/PhysicsShape.hpp"

// Box2D forward declare
class b2Fixture;

// BIG TODO: Work out how to allow modification of shapes.

namespace ngine::physics {
    class PhysicsBody;

    class NEAPI PhysicsFixture {
        // Private Fields

        /*
         * The fixture
         */
        b2Fixture *m_fixture = nullptr;

        /*
         * The attached shape
         */
        shapes::PhysicsShape *m_shape = nullptr;
    public:
        // Public Constructor

        PhysicsFixture(b2Fixture *fixture);

        // Destructor

        ~PhysicsFixture();

        // Public Methods

        /*
         * Destroy ourself.
         */
        void destroy();

        /*
         * Get the Box2D fixture.
         */
        b2Fixture *getB2Fixture();

        /*
         * Get the physics body.
         */
        PhysicsBody *getBody();

        /*
         * Get the density of this fixture.
         */
        float getDensity() const;

        /*
         * Get the coefficient of friction.
         */
        float getFriction() const;

        /*
         * Get the coefficient of restitution.
         */
        float getRestitution() const;

        /*
         * Get the attached shape.
         */
        shapes::PhysicsShape *getShape();

        /*
         * Get the attached shape.
         */
        const shapes::PhysicsShape *getShape() const;

        /*
         * Get the shape type.
         */
        shapes::PhysicsShapeType getType() const;

        /*
         * Is this fixture a sensor (non-solid)?
         */
        bool isSensor();

        /*
         * Call this if you want to establish collision that was disabled by ContactFilter::ShouldCollide
         */
        void refilter();

        /*
         * Set if this fixture is a sensor.
         */
        void setSensor(bool sensor);

        /*
         * Test a point for containment in this fixture.
         */
        bool testPoint(const Vector2 &p);
    };
}

#endif //PHYSICSFIXTURE_HPP
