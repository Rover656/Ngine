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

#include "../Graphics/Renderer.hpp"
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
         * @param body_ The Box2D body to be created from.
         */
        explicit PhysicsBody(b2Body *body_);

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
         * @param world_ The parent world.
         * @param info_ BodyInfo used to create the body.
         */
        PhysicsBody(PhysicsWorld *world_, BodyInfo info_);

        /**
         * Create a new physics body.
         *
         * @param world_ The parent world.
         * @param type_ The body type.
         * @param position_ The initial position.
         */
        PhysicsBody(PhysicsWorld *world_, BodyType type_ = BODY_STATIC, Vector2 position_ = Vector2::Zero);
        ~PhysicsBody();

        /**
         * Set body properties using the `BodyInfo` structure.
         *
         * @warning This will overwrite every property!!! If you want to change a single property, use the set method for it instead.
         * @param info_ The properties to set.
         */
        void set(BodyInfo info_);

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
         * @param type_ The new body type.
         */
        void setType(BodyType type_);

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
         * @param flag_ Whether or not the body should be treated like a bullet.
         */
        void setIsBullet(bool flag_);

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
         * @param scale_ The gravity scale for the body.
         */
        void setGravityScale(float scale_);

        /**
         * Get the body transform.
         *
         * @return The body's current transform.
         */
        Transform2D getTransform() const;

        /**
         * Set the body transform.
         *
         * @param transform_ The new body transform.
         */
        void setTransform(const Transform2D &transform_);

        /**
         * Get body position.
         *
         * @return The body position.
         */
        Vector2 getPosition() const;

        /**
         * Set body position.
         *
         * @param pos_ The new body position.
         */
        void setPosition(Vector2 pos_);

        /**
         * Get the body linear velocity.
         *
         * @return The current body linear velocity.
         */
        Vector2 getLinearVelocity() const;

        /**
         * Set the body linear velocity.
         *
         * @param vel_ The new body linear velocity.
         */
        void setLinearVelocity(Vector2 vel_);

        /**
         * Apply a force at a point on the body.
         *
         * @param force_ Force to apply.
         * @param point_ Point to apply force.
         * @param wake_ Whether or not to wake up the body.
         */
        void applyForce(const Vector2 &force_, const Vector2 &point_, bool wake_);

        /**
         * Apply a force to the center of the body.
         *
         * @param force_ Force to apply.
         * @param wake_ Whether or not to wake up the body.
         */
        void applyForceToCenter(const Vector2 &force_, bool wake_);

        /**
        * Apply a linear impulse at a point on the body.
         *
         * @param impulse_ Impulse to apply.
         * @param point_ Point to apply impulse.
         * @param wake_ Whether or not to wake up the body.
        */
        void applyLinearImpulse(const Vector2 &impulse_, const Vector2 &point_, bool wake_);

        /**
         * Apply a linear impuse at the center of the body.
         *
         * @param impulse_ The impulse to apply.
         * @param wake_ Whether or not to wake up the body.
         */
        void applyLinearImpulseToCenter(const Vector2 &impulse_, bool wake_);

        /**
         * Get the linear damping value.
         *
         * @return The body's linear damping value.
         */
        float getLinearDamping() const;

        /**
         * Set the body's linear damping value.
         *
         * @param damping_ The new damping value.
         */
        void setLinearDamping(float damping_);

        /**
         * Get body rotation.
         *
         * @return The current body rotation.
         */
        Angle getRotation() const;

        /**
         * Set body rotation.
         *
         * @param angle_ The new body rotation.
         */
        void setRotation(Angle angle_);

        /**
         * Should the body be able to rotate?
         *
         * @return Whether or not the body can rotate.
         */
        bool isFixedRotation() const;

        /**
         * Set whether or not the body can rotate.
         *
         * @param flag_ Whether or not the body can rotate.
         */
        void setIsFixedRotation(bool flag_);

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
         * @param omega_ The new angular velocity (in degrees).
         */
        void setAngularVelocity(float omega_);

        /**
         * Apply a torque on the body.
         *
         * @param torque_ The torque to apply.
         * @param wake_ Whether or not to wake up the body.
         */
        void applyTorque(float torque_, bool wake_);

        /**
         * Apply an angular impulse on the body.
         *
         * @param impulse_ The impulse to apply.
         * @param wake_ Whether or not to wake up the body.
         */
        void applyAngularImpulse(float impulse_, bool wake_);

        /**
         * Get the angular damping value.
         *
         * @return The angular damping value.
         */
        float getAngularDamping() const;

        /**
         * Set the angular damping value.
         *
         * @param damping_ New damping value.
         */
        void setAngularDamping(float damping_);

        /**
         * Create a fixture.
         */
        PhysicsFixture *createFixture(shapes::PhysicsShape *shape_, float density_);

        /**
         * Destroy a fixture.
         *
         * @param fixture_ The fixture to destroy.
         */
        void destroyFixture(PhysicsFixture *fixture_);

        /**
         * Get whether or not the body is active.
         *
         * @return Whether or not the body is active.
         */
        bool isActive() const;

        /**
         * Set whether or not the body is active.
         *
         * @param flag_ Whether or not the body should be active.
         */
        void setIsActive(bool flag_);

        /**
         * Determine whether or not the body is awake.
         *
         * @return Whether or not the body is awake.
         */
        bool isAwake() const;

        /**
         * Set whether or not the body is awake.
         *
         * @param flag_ Whether or not the body should be awake.
         */
        void setIsAwake(bool flag_);

        /**
         * Determine if the body can sleep.
         *
         * @return Whether or not the body is allowed to sleep.
         */
        bool isSleepingAllowed() const;

        /**
         * Set whether or not sleeping is allowed.
         *
         * @param flag_ Whether or not sleeping should be allowed.
         */
        void setIsSleepingAllowed(bool flag_);

        /**
         * Draw debug geometry.
         *
         * @todo Rework the debug drawing system.
         * @param renderer_ The game renderer.
         */
        void debugDraw(graphics::Renderer *renderer_);

        // TODO: Get fixtures, joints and contacts
    };
}

#endif //PHYSICSBODY_HPP
