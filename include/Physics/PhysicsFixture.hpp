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

#include "Shapes/PhysicsShape.hpp"

// Box2D forward declare
class b2Fixture;

// BIG TODO: Work out how to allow modification of shapes.

namespace Ngine::Physics {
    class PhysicsBody;

    class NEAPI PhysicsFixture {
        // Private Fields

        /*
         * The fixture
         */
        b2Fixture *_Fixture = nullptr;

        /*
         * The attached shape
         */
        Shapes::PhysicsShape *_Shape = nullptr;
    public:
        // Public Constructor

        PhysicsFixture(b2Fixture *fixture_);

        // Destructor

        ~PhysicsFixture();

        // Public Methods

        /*
         * Destroy ourself.
         */
        void Destroy();

        /*
         * Get the Box2D fixture.
         */
        b2Fixture *GetB2Fixture();

        /*
         * Get the physics body.
         */
        PhysicsBody *GetBody();

        /*
         * Get the density of this fixture.
         */
        float GetDensity() const;

        /*
         * Get the coefficient of friction.
         */
        float GetFriction() const;

        /*
         * Get the coefficient of restitution.
         */
        float GetRestitution() const;

        /*
         * Get the attached shape.
         */
        Shapes::PhysicsShape *GetShape();

        /*
         * Get the attached shape.
         */
        const Shapes::PhysicsShape *GetShape() const;

        /*
         * Get the shape type.
         */
        Shapes::PhysicsShapeType GetType() const;

        /*
         * Is this fixture a sensor (non-solid)?
         */
        bool IsSensor();

        /*
         * Call this if you want to establish collision that was disabled by ContactFilter::ShouldCollide
         */
        void Refilter();

        /*
         * Set if this fixture is a sensor.
         */
        void SetSensor(bool sensor_);

        /*
         * Test a point for containment in this fixture.
         */
        bool TestPoint(const Vector2 &p);
    };
}

#endif //PHYSICSFIXTURE_HPP
