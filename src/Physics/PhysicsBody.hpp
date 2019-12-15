/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef PHYSICSBODY_HPP
#define PHYSICSBODY_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "PhysicsFixture.hpp"

// Forward declare Box2D
class b2Body;

namespace NerdThings::Ngine::Physics {
    // Forward declare
    class PhysicsWorld;

    /*
     * A physics body
     */
    class NEAPI PhysicsBody {
    private:
        // Private Fields

        /*
         * The Box2D body
         */
        b2Body *_B2Body = nullptr;

        /*
         * The attached world.
         */
        PhysicsWorld *_World = nullptr;

    public:
        // Enums

        enum BodyType {
            BODY_STATIC = 0,
            BODY_KINEMATIC,
            BODY_DYNAMIC
        };

        // Structs

        struct BodyInfo {
            // Public Fields

            /*
             * Does this body start active
             */
            bool Active = true;

            /*
             * Allow the body to sleep
             */
            bool AllowSleep = true;

            /*
             * Angular damping
             */
            float AngularDamping = 0;

            /*
             * Body angular velocity
             */
            float AngularVelocity = 0;

            /*
             * Does the body begin awake?
             */
            bool Awake = true;

            /*
             * Is this a fast moving body that should be prevented from tunneling through moving bodies.
             * Only considered on dynamic bodies.
             */
            bool Bullet = false;

            /*
             * Should the body be prevented from rotating.
             */
            bool FixedRotation = false;

            /*
             * Body gravity scale.
             */
            float GravityScale = 1;

            /*
             * Linear damping
             */
            float LinearDamping = 0;

            /*
             * Linear velocity of the body.
             */
            Vector2 LinearVelocity = Vector2::Zero;

            /*
             * World position
             */
            Vector2 Position = Vector2::Zero;

            /*
             * Body rotation in degrees.
             */
            float Rotation = 0;

            /*
             * The body type
             */
            BodyType Type = BODY_STATIC;

            // Public Constructor

            BodyInfo() {}
        };

        // Public Constructor

        /*
         * Create a physics body from a box2d body.
         */
        PhysicsBody(b2Body *body_);

        /*
         * Create a new physics body
         */
        PhysicsBody(PhysicsWorld *world_, BodyInfo info_);

        /*
         * Create a new physics body
         */
        PhysicsBody(PhysicsWorld *world_, BodyType type_, Vector2 position_ = Vector2::Zero);

        // Destructor

        ~PhysicsBody();

        // Public Methods

        void ApplyAngularImpulse(float impulse_, bool wake_);

        void ApplyForce(const Vector2 &force_, const Vector2 &point_, bool wake_);

        void ApplyForceToCenter(const Vector2 &force_, bool wake_);

        void ApplyLinearImpulse(const Vector2 &impulse_, const Vector2 &point_, bool wake_);

        void ApplyLinearImpulseToCenter(const Vector2 &impulse_, bool wake_);

        void ApplyTorque(float torque_, bool wake_);

        /*
         * Create a fixture
         */
        PhysicsFixture *CreateFixture(Shapes::PhysicsShape *shape_, float density_);

        /*
         * Draw debug geometry
         */
        void DebugDraw();

        /*
         * Destroy ourself.
         */
        void Destroy();

        /*
         * Destroy a fixture.
         */
        void DestroyFixture(PhysicsFixture *fixture_);

        float GetAngularDamping() const;

        float GetAngularVelocity() const;

        /*
         * Get Box2D body.
         */
        b2Body *GetB2Body() const;

        float GetGravityScale() const;

        float GetInertia() const;

        float GetLinearDamping() const;

        /*
         * Get the body linear velocity.
         */
        Vector2 GetLinearVelocity() const;

        /*
         * Get the body linear velocity in pixels.
         */
        Vector2 GetLinearVelocityPixels() const;

        float GetMass() const;

        /*
         * Get body position in pixels.
         */
        Vector2 GetPosition() const;

        /*
         * Get body rotation (degrees).
         */
        float GetRotation() const;

        /*
         * Get the parent world.
         */
        PhysicsWorld *GetWorld() const;

        bool IsActive() const;

        bool IsAwake() const;

        bool IsBullet() const;

        bool IsFixedRotation() const;

        bool IsSleepingAllowed() const;

        void ResetMassData();

        void SetActive(bool flag_);

        void SetAngularDamping(float damping_);

        void SetAngularVelocity(float omega_);

        void SetAwake(bool flag_);

        void SetBullet(bool flag_);

        void SetFixedRotation(bool flag_);

        void SetGravityScale(float scale_);

        void SetLinearDamping(float damping_);

        /*
         * Set the body linear velocity (meters).
         */
        void SetLinearVelocity(Vector2 vel_);

        /*
         * Set the body linear velocity (pixels).
         */
        void SetLinearVelocityPixels(Vector2 vel_);

        /*
         * Set body position in pixels.
         */
        void SetPosition(Vector2 pos_);

        /*
         * Set body rotation (degrees).
         */
        void SetRotation(float angle_);

        void SetSleepingAllowed(bool flag_);

        void SetType(BodyType type_);

        // TODO: Get fixtures, joints and contacts + MassData
    };
}

#endif //PHYSICSBODY_HPP
