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

        /**
         * Get the body position.
         *
         * @return The body position.
         */
        Vector2 getPosition() const;

        /**
         * Set the body position.
         *
         * @param position New body position.
         */
        void setPosition(const Vector2 &position);

        /**
         * Get body rotation.
         *
         * @return Body rotation.
         */
        float getRotation() const;

        /**
         * Set the body rotation.
         *
         * @param rotation New body rotation.
         */
        void setRotation(float rotation);

        /**
         * Get the transform for the body.
         *
         * @return The body transform.
         */
        Transform2D getTransform() const;

        /**
         * Set the body transform.
         *
         * @param position New position.
         * @param angle New rotation.
         */
        void setTransform(const Vector2 &position, float angle);

        /**
         * Get the center of the body in world coordinates.
         *
         * @return Center in world coordinates.
         */
        Vector2 getWorldCenter() const;

        /**
         * Get the center of the body in local coordinates.
         *
         * @return Center in local coordinates.
         */
        Vector2 getLocalCenter() const;

        /**
         * Get the linear velocity of the body
         *
         * @return Body linear velocity.
         */
        Vector2 getLinearVelocity() const;

        /**
         * Set the linear velocity of the body.
         *
         * @param velocity New velocity.
         */
        void setLinearVelocity(const Vector2 &velocity);

        /**
         * Get the angular velocity of the body.
         *
         * @param The body's angular velocity.
         */
        float getAngularVelocity() const;

        /**
         * Set the angular velocity of the body.
         *
         * @param velocity New angular velocity.
         */
        void setAngularVelocity(float velocity);

        /**
         * Get the mass of the body.
         *
         * @return The body's mass.
         */
        float getMass() const;

        /**
         * Get inertia in kgpixels^2.
         */
        float getInertia() const;

        /**
         * Get the body's mass data.
         *
         * @return The mass data for the body.
         */
        MassData getMassData() const;

        /**
         * Set the body's mass data.
         *
         * @param data The new mass data.
         */
        void setMassData(const MassData &data);

        /**
         * Reset the body's mass data.
         */
        void resetMassData();

        /**
         * Get the body type.
         *
         * @return The body type.
         */
        Type getType() const;

        /**
         * Set the body type.
         *
         * @param type New body type.
         */
        void setType(Type type);

        /**
         * Get the next body in the world's list.
         */
        PhysicsBody *getNext();

        /**
         * Get the next body in the world's list.
         */
        const PhysicsBody *getNext() const;
    };
}

#endif //PHYSICSBODY_HPP
