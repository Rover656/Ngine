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

#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "PhysicsBody.hpp"

// Forward declare Box2D stuff
class b2World;

namespace NerdThings::Ngine::Physics {
    class NEAPI PhysicsWorld {
    private:
        // Private Fields

        b2World *_B2World;
    public:
        // Public Fields

        /*
         * Whether or not we should draw debug geometry.
         */
        bool DebugDraw = false;

        /*
         * The number of pixels a meter.
         * Default: 1
         */
        const float PixelsPerMeter = 1;

        // Public Constructor

        /*
         * Create a physics world.
         * Gravity is measured in meters.
         */
        explicit PhysicsWorld(const Vector2 &gravVector = Vector2::Zero, float ppm = 1);

        // Destructor

        ~PhysicsWorld();

        // Public Methods

        /*
         * Clear force buffer on all bodies.
         */
        void ClearForces();

        /*
         * Create a body
         */
        PhysicsBody *CreateBody(PhysicsBody::BodyInfo info_);

        /*
         * Destroy a body
         */
        void DestroyBody(PhysicsBody *body_);

        /*
         * Get is sleeping allowed
         */
        bool GetAllowSleeping();

        /*
         * Get the Box2D world.
         * Should be used carefully.
         */
        b2World *GetB2World();

        /*
         * Get the number of bodies
         */
        int GetBodyCount();

        /*
         * Get the number of contacts
         */
        int GetContactCount();

        /*
         * Is using continuous physics.
         */
        bool GetContinuousPhysics();

        /*
         * Get the current gravity vector (meters).
         */
        Vector2 GetGravity();

        /*
         * Get the current gravity vector (pixels).
         */
        Vector2 GetGravityPixels();

        /*
         * Get the number of joints
         */
        int GetJointCount();

        /*
         * Get the number of broad-phase proxies
         */
        int GetProxyCount();

        /*
         * Is single stepping continual physics?
         */
        bool GetSubStepping();

        /*
         * Get the balance of the dynamic tree
         */
        bool GetTreeBalance();

        /*
         * Get the height of the dynamic tree
         */
        bool GetTreeHeight();

        /*
         * Is warm starting?
         */
        bool GetWarmStarting();

        /*
         * Is world mid-timestep.
         */
        bool IsLocked();

        /*
         * Enable/Disable sleep
         */
        void SetAllowSleeping(bool flag_);

        /*
         * Enable/Disable automatic clearing of forces after each timestep.
         */
        void SetAutoClearForces(bool flag_);

        /*
         * Enable/disable continuous physics.
         */
        void SetContinuousPhysics(bool flag_);

        /*
         * Set the gravity vector (meters).
         */
        void SetGravity(const Vector2 &gravVector);

        /*
         * enable/disable single stepped continuous physics.
         */
        void SetSubStepping(bool flag_);

        /*
         * Enable/disable warm starting.
         */
        void SetWarmStarting(bool flag_);

        /*
         * Take a time step
         */
        void Step(float timeStep, int velocityIterations, int positionIterations);
    };
}

#endif //PHYSICSWORLD_HPP
