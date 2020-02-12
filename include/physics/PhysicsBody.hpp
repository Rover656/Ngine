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

#include "../graphics/Renderer.hpp"
#include "../Math.hpp"
#include "PhysicsFixture.hpp"

// Forward declare Box2D
class b2Body;

namespace ngine::physics {
    // Forward declare
    class PhysicsWorld;

    /**
     * A physics body.
     */
    class NEAPI PhysicsBody {
    private:
        /**
         * The Box2D body
         */
        b2Body *m_b2Body = nullptr;

        /**
         * The attached world.
         */
        PhysicsWorld *m_world = nullptr;

        /**
         * Create a physics body from a box2d body.
         *
         * @param body The Box2D body to be created from.
         */
        explicit PhysicsBody(b2Body *body);

    public:
        /**
         * The body type.
         */
        enum BodyType {
            BODY_STATIC = 0,
            BODY_KINEMATIC,
            BODY_DYNAMIC
        };

        /**
         * Body creation information.
         */
        struct BodyInfo {
            /**
             * Does this body start active
             */
            bool Active = true;

            /**
             * Allow the body to sleep
             */
            bool AllowSleep = true;

            /**
             * Angular damping
             */
            float AngularDamping = 0;

            /**
             * Body angular velocity
             */
            float AngularVelocity = 0;

            /**
             * Does the body begin awake?
             */
            bool Awake = true;

            /**
             * Is this a fast moving body that should be prevented from tunneling through moving bodies.
             * Only considered on dynamic bodies.
             */
            bool Bullet = false;

            /**
             * Should the body be prevented from rotating.
             */
            bool FixedRotation = false;

            /**
             * Body gravity scale.
             */
            float GravityScale = 1;

            /**
             * Linear damping
             */
            float LinearDamping = 0;

            /**
             * Linear velocity of the body.
             */
            Vector2 LinearVelocity = Vector2::Zero;

            /**
             * World position
             */
            Vector2 Position = Vector2::Zero;

            /**
             * Body rotation.
             */
            Angle Rotation = 0;

            /**
             * The body type
             */
            BodyType Type = BODY_STATIC;

            BodyInfo() = default;
        };

        /**
         * Create a new physics body.
         *
         * @param world The parent world.
         * @param info BodyInfo used to create the body.
         */
        PhysicsBody(PhysicsWorld *world, BodyInfo info);

        /**
         * Create a new physics body.
         *
         * @param world The parent world.
         * @param type The body type.
         * @param position The initial position.
         */
        PhysicsBody(PhysicsWorld *world, BodyType type = BODY_STATIC, Vector2 position = Vector2::Zero);

        ~PhysicsBody();

        /**
         * Set body properties using the `BodyInfo` structure.
         *
         * @warning This will overwrite every property!!! If you want to change a single property, use the set method for it instead.
         * @param info The properties to set.
         */
        void set(BodyInfo info);

        /**
         * Destroy ourself.
         */
        void destroy();

        /**
         * Get the physics body type.
         *
         * @return The physics body type.
         */
        BodyType getType() const;

        /**
         * Set the physics body type.
         *
         * @param type The new body type.
         */
        void setType(BodyType type);

        /**
         * Get the mass of the body.
         *
         * @return The mass of the body.
         */
        float getMass() const;

        // TODO: MassData struct

        /**
         * Reset the body mass data.
         */
        void resetMassData();

        /**
         * Whether or not the body is treated like a bullet for continuous collision detection.
         *
         * @return Whether or not the body is treated like a bullet.
         */
        bool isBullet() const;

        /**
         * Set whether or not the body is treated like a bullet.
         *
         * @param flag Whether or not the body should be treated like a bullet.
         */
        void setIsBullet(bool flag);

        /**
         * Get Box2D body.
         *
         * @return The Box2D body.
         */
        b2Body *getB2Body() const;

        /**
         * Get the parent physics world.
         *
         * @return The parent world.
         */
        PhysicsWorld *getWorld() const;

        /**
         * Get the body gravity scale.
         *
         * @return The gravity scale for the body.
         */
        float getGravityScale() const;

        /**
         * Set the body gravity scale.
         *
         * @param scale The gravity scale for the body.
         */
        void setGravityScale(float scale);

        /**
         * Get the body transform.
         *
         * @return The body's current transform.
         */
        Transform2D getTransform() const;

        /**
         * Set the body transform.
         *
         * @param transform The new body transform.
         */
        void setTransform(const Transform2D &transform);

        /**
         * Get body position.
         *
         * @return The body position.
         */
        Vector2 getPosition() const;

        /**
         * Set body position.
         *
         * @param pos The new body position.
         */
        void setPosition(Vector2 pos);

        /**
         * Get the body linear velocity.
         *
         * @return The current body linear velocity.
         */
        Vector2 getLinearVelocity() const;

        /**
         * Set the body linear velocity.
         *
         * @param vel The new body linear velocity.
         */
        void setLinearVelocity(Vector2 vel);

        /**
         * Apply a force at a point on the body.
         *
         * @param force Force to apply.
         * @param point Point to apply force.
         * @param wake Whether or not to wake up the body.
         */
        void applyForce(const Vector2 &force, const Vector2 &point, bool wake);

        /**
         * Apply a force to the center of the body.
         *
         * @param force Force to apply.
         * @param wake Whether or not to wake up the body.
         */
        void applyForceToCenter(const Vector2 &force, bool wake);

        /**
        * Apply a linear impulse at a point on the body.
         *
         * @param impulse Impulse to apply.
         * @param point Point to apply impulse.
         * @param wake Whether or not to wake up the body.
        */
        void applyLinearImpulse(const Vector2 &impulse, const Vector2 &point, bool wake);

        /**
         * Apply a linear impuse at the center of the body.
         *
         * @param impulse The impulse to apply.
         * @param wake Whether or not to wake up the body.
         */
        void applyLinearImpulseToCenter(const Vector2 &impulse, bool wake);

        /**
         * Get the linear damping value.
         *
         * @return The body's linear damping value.
         */
        float getLinearDamping() const;

        /**
         * Set the body's linear damping value.
         *
         * @param damping The new damping value.
         */
        void setLinearDamping(float damping);

        /**
         * Get body rotation.
         *
         * @return The current body rotation.
         */
        Angle getRotation() const;

        /**
         * Set body rotation.
         *
         * @param angle The new body rotation.
         */
        void setRotation(Angle angle);

        /**
         * Should the body be able to rotate?
         *
         * @return Whether or not the body can rotate.
         */
        bool isFixedRotation() const;

        /**
         * Set whether or not the body can rotate.
         *
         * @param flag Whether or not the body can rotate.
         */
        void setIsFixedRotation(bool flag);

        /**
         * Get rotational inertia about the local origin.
         *
         * @return Body rotational intertia.
         */
        float getInertia() const;

        /**
         * Get the body angular velocity.
         *
         * @return The current body angular velocity.
         */
        float getAngularVelocity() const;

        /**
         * Set the body angular velocity.
         *
         * @param omega The new angular velocity (in degrees).
         */
        void setAngularVelocity(float omega);

        /**
         * Apply a torque on the body.
         *
         * @param torque The torque to apply.
         * @param wake Whether or not to wake up the body.
         */
        void applyTorque(float torque, bool wake);

        /**
         * Apply an angular impulse on the body.
         *
         * @param impulse The impulse to apply.
         * @param wake Whether or not to wake up the body.
         */
        void applyAngularImpulse(float impulse, bool wake);

        /**
         * Get the angular damping value.
         *
         * @return The angular damping value.
         */
        float getAngularDamping() const;

        /**
         * Set the angular damping value.
         *
         * @param damping New damping value.
         */
        void setAngularDamping(float damping);

        /**
         * Create a fixture.
         */
        PhysicsFixture *createFixture(shapes::PhysicsShape *shape, float density);

        /**
         * Destroy a fixture.
         *
         * @param fixture The fixture to destroy.
         */
        void destroyFixture(PhysicsFixture *fixture);

        /**
         * Get whether or not the body is active.
         *
         * @return Whether or not the body is active.
         */
        bool isActive() const;

        /**
         * Set whether or not the body is active.
         *
         * @param flag Whether or not the body should be active.
         */
        void setIsActive(bool flag);

        /**
         * Determine whether or not the body is awake.
         *
         * @return Whether or not the body is awake.
         */
        bool isAwake() const;

        /**
         * Set whether or not the body is awake.
         *
         * @param flag Whether or not the body should be awake.
         */
        void setIsAwake(bool flag);

        /**
         * Determine if the body can sleep.
         *
         * @return Whether or not the body is allowed to sleep.
         */
        bool isSleepingAllowed() const;

        /**
         * Set whether or not sleeping is allowed.
         *
         * @param flag Whether or not sleeping should be allowed.
         */
        void setIsSleepingAllowed(bool flag);

        /**
         * Draw debug geometry.
         *
         * @todo Rework the debug drawing system.
         * @param renderer The game renderer.
         */
        void debugDraw(graphics::Renderer *renderer);

        // TODO: Get fixtures, joints and contacts
    };
}

#endif //PHYSICSBODY_HPP
