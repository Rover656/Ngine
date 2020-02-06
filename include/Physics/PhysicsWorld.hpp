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

#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "PhysicsBody.hpp"

// Forward declare Box2D stuff
class b2World;

namespace ngine::physics {
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
        void clearForces();

        /*
         * Create a body
         */
        PhysicsBody *createBody(PhysicsBody::BodyInfo info_);

        /*
         * Destroy a body
         */
        void destroyBody(PhysicsBody *body_);

        /*
         * Get is sleeping allowed
         */
        bool getAllowSleeping();

        /*
         * Get the Box2D world.
         * Should be used carefully.
         */
        b2World *getB2World();

        /*
         * Get the number of bodies
         */
        int getBodyCount();

        /*
         * Get the number of contacts
         */
        int getContactCount();

        /*
         * Is using continuous physics.
         */
        bool getContinuousPhysics();

        /*
         * Get the current gravity vector (meters).
         */
        Vector2 getGravity();

        /*
         * Get the current gravity vector (pixels).
         */
        Vector2 getGravityPixels();

        /*
         * Get the number of joints
         */
        int getJointCount();

        /*
         * Get the number of broad-phase proxies
         */
        int getProxyCount();

        /*
         * Is single stepping continual physics?
         */
        bool getSubStepping();

        /*
         * Get the balance of the dynamic tree
         */
        bool getTreeBalance();

        /*
         * Get the height of the dynamic tree
         */
        bool getTreeHeight();

        /*
         * Is warm starting?
         */
        bool getWarmStarting();

        /*
         * Is world mid-timestep.
         */
        bool isLocked();

        /*
         * Enable/Disable sleep
         */
        void setAllowSleeping(bool flag_);

        /*
         * Enable/Disable automatic clearing of forces after each timestep.
         */
        void setAutoClearForces(bool flag_);

        /*
         * Enable/disable continuous physics.
         */
        void setContinuousPhysics(bool flag_);

        /*
         * Set the gravity vector (meters).
         */
        void setGravity(const Vector2 &gravVector);

        /*
         * enable/disable single stepped continuous physics.
         */
        void setSubStepping(bool flag_);

        /*
         * Enable/disable warm starting.
         */
        void setWarmStarting(bool flag_);

        /*
         * Take a time step
         */
        void step(float timeStep, int velocityIterations, int positionIterations);
    };
}

#endif //PHYSICSWORLD_HPP
