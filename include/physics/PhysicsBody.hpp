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

#ifndef PHYSICSBODY_HPP
#define PHYSICSBODY_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "PhysicsContext.hpp"
#include "PhysicsFixture.hpp"
#include "PhysicsShape.hpp"

// Forward declaring Box2D
class b2Body;

namespace ngine::physics {
    class PhysicsWorld;

    class NEAPI PhysicsBody final {
        friend class PhysicsWorld;

        /**
         * The Box2D body.
         */
        b2Body *m_body = nullptr;

        /**
         * Our version of the physics world.
         */
        PhysicsWorld *m_world = nullptr;

        /**
         * Our physics context.
         */
        const PhysicsContext *m_context = nullptr;

        /**
         * List of fixtures.
         * This is soley for memory management when Box2D forgets us...
         */
        std::vector<PhysicsFixture *> m_fixtures;

        /**
         * Create a physics body.
         */
        PhysicsBody(const PhysicsContext *context, PhysicsWorld *world, b2Body *body);

        /**
         * Destroy fixtures.
         */
        void _destroy();
    public:
        /**
         * Physics body type.
         */
        enum class Type {
            Static,
            Kinematic,
            Dynamic
        };

        /**
         * Create a fixture for the body.
         *
         * @param shape The shape for the fixture.
         * @param density Fixture density, 0 for static shapes.
         */
        PhysicsFixture *createFixture(PhysicsShape *shape, float density);

        /**
         * Destroy a fixture. This deletes the fixture parameter.
         *
         * @param fixture Fixture to destroy.
         */
        void destroyFixture(PhysicsFixture *fixture);

        /**
         * Get the attached physics world.
         */
        PhysicsWorld *getWorld() const;

        Vector2 getPosition() const;

        void setPosition(const Vector2 &position);

        Angle getRotation() const;

        void setRotation(const Angle &rotation);

        Transform2D getTransform() const;

        void setTransform(const Transform2D &transform);

        Vector2 getWorldCenter() const;

        Vector2 getLocalCenter() const;

        Vector2 getLinearVelocity() const;

        void setLinearVelocity(const Vector2 &velocity);

        float getAngularVelocity() const;

        void setAngularVelocity(float velocity);

        float getMass() const;

        /**
         * Get inertia in kgpixels^2.
         */
        float getInertia() const;

        void resetMassData();

        Type getType() const;

        void setType(Type type);
    };
}

#endif //PHYSICSBODY_HPP
