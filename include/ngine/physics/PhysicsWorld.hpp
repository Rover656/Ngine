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

#include "../graphics/Renderer.hpp"
#include "../Math.hpp"
#include "PhysicsBody.hpp"
#include "PhysicsContext.hpp"

// Forward declare
class b2World;

namespace ngine::physics {
    /**
     * Physics debug draw flags
     */
    enum PhysicsDebugDrawFlag {
        DRAW_SHAPE				= 0x0001,	///< draw shapes
        DRAW_JOINT				= 0x0002,	///< draw joint connections
        DRAW_AABB				= 0x0004,	///< draw axis aligned bounding boxes
        DRAW_PAIR				= 0x0008,	///< draw broad-phase pairs
        DRAW_CENTER_OF_MASS		= 0x0010	///< draw center of mass frame
    };

    /**
     * This contains all of the physics bodies.
     *
     * This only deals with units in pixels, it accepts a pixel per meter ratio for internal conversions for Box2D.
     */
    class NEAPI PhysicsWorld {
        /**
         * The Box2D world.
         */
        b2World *m_world;

        /**
         * Physics context for conversions.
         */
        const PhysicsContext *m_context = nullptr;

        /**
         * The internal debug draw class.
         */
        class PhysicsDebugDraw *m_debugDraw = nullptr;

        /**
         * Velocity iterations for the step call.
         */
        int m_velocityIterations = 8;

        /**
         * Position iterations for the step call.
         */
        int m_positionIterations = 3;

        /**
         * Track physics bodies for deletion.
         */
        std::vector<PhysicsBody *> m_bodies;
    public:
        /**
         * Create a new physics world.
         *
         * @param context Physics context used for conversions. Managed by you.
         * @param gravity The gravity vector.
         */
        PhysicsWorld(PhysicsContext *context, const Vector2 &gravity);

        ~PhysicsWorld();

        /**
         * Get the world physics context.
         *
         * @return The physics context.
         */
        const PhysicsContext *getContext() const;

        /**
         * Create a new physics body.
         *
         * @param type Body type.
         * @param position Body position (avoid setting to origin then setting elsewhere).
         */
        PhysicsBody *createBody(PhysicsBody::Type type, const Vector2 &position);

        /**
         * Destroy a physics body.
         *
         * @warning This will delete body.
         * @param body Body to destroy.
         */
        void destroyBody(PhysicsBody *body);

        /**
         * Get the number of bodies.
         *
         * @return The number of bodies.
         */
        int getBodyCount() const;

        /**
         * Get the height of the dynamic tree.
         *
         * @return The height of the dynamic tree.
         */
        int getTreeHeight() const;

        /**
         * Get the balance of the dynamic tree.
         *
         * @return The balance of the dynamic tree.
         */
        int getTreeBalance() const;

        /**
         * Get the quality metric of the dynamic tree.
         * The smaller the better.
         * The minimum is 1.
         *
         * @return The quality of the dynamic tree.
         */
        float getTreeQuality() const;

        /**
         * Get the gravity vector
         *
         * @return The gravity vector.
         */
        Vector2 getGravity() const;

        /**
         * Set the gravity vector.
         *
         * @param gravity The new gravity vector.
         */
        void setGravity(const Vector2 &gravity);

        /**
         * Determine whether or not forces are cleared after each step.
         *
         * @return Whether or not forces are cleared after each step.
         */
        bool getAutoClearForces() const;

        /**
         * Set whether or not forces are cleared after each step.
         *
         * @param flag Whether or not forces should be cleared after each step.
         */
        void setAutoClearForces(bool flag);

        /**
         * Manually clear the force buffer on all bodies.
         * By default, forces are cleared automatically after each call to Step.
         * The default behavior is modified by calling SetAutoClearForces.
         * The purpose of this function is to support sub-stepping.
         * Sub-stepping is often used to maintain a fixed sized time step under a variable frame-rate.
         * When you perform sub-stepping you will disable auto clearing of forces and instead call ClearForces after all sub-steps are complete in one pass of your game loop.
         */
        void clearForces();

        /**
         * Determine if sleeping is allowed.
         *
         * @return Whether or not sleeping is allowed.
         */
        bool getAllowSleeping() const;

        /**
         * Set whether or not sleeping is allowed.
         *
         * @param flag Allow or disallow sleeping.
         */
        void setAllowSleeping(bool flag);

        /**
         * Is warm starting enabled?
         *
         * @return Whether or not warm starting is enabled.
         */
        bool getWarmStarting() const;

        /**
         * Enable or disable warm starting.
         *
         * @param flag Warm starting enabled or disabled.
         */
        void setWarmStarting(bool flag);

        /**
         * Determine if continuous physics is enabled
         *
         * @return Whether or not continuous physics is enabled.
         */
        bool getContinuousPhysics() const;

        /**
         * Enable or disable continuous physics.
         *
         * @param flag Continuous physics enabled or disabled.
         */
        void setContinuousPhysics(bool flag);

        /**
         * Determine if single stepped continuous physics is enabled.
         *
         * @return Whether or not single stepped continuous physics is enabled.
         */
        bool getSubStepping() const;

        /**
         * Enable or disable single stepped continuous physics.
         *
         * @param flag Single stepped continuous physics enabled or disabled.
         */
        void setSubStepping(bool flag);

        /**
         * Determine if debug drawing is enabled.
         *
         * @return If debug drawing is enabled.
         */
        bool isDebugDrawEnabled() const;

        /**
         * Enable debug draw.
         */
        void enableDebugDraw();

        /**
         * Disable debug draw.
         */
        void disableDebugDraw();

        /**
         * Set debug draw flags
         */
        void setDebugDrawFlags(unsigned int flags);

        /**
         * Get debug draw flags
         */
        unsigned int getDebugDrawFlags() const;

        /**
         * Draw debug geometry.
         *
         * @param renderer Renderer to draw with.
         */
        void debugDraw(graphics::Renderer *renderer);

        /**
         * Get the velocity iterations for the velocity constraint solver.
         *
         * @return Current iteration count.
         */
        int getVelocityIterations() const;

        /**
         * Set the velocity iterations for the velocity constraint solver.
         *
         * @param iterations New iteration count.
         */
        void setVelocityIterations(int iterations);

        /**
         * Get the position iterations for the position constraint solver.
         *
         * @return Current iteration count.
         */
        int getPositionIterations() const;

        /**
         * Set the position iterations for the position constaint solver.
         *
         * @param iterations New iteration count.
         */
        void setPositionIterations(int iterations);

        /**
         * Take a time step. This performs collision detection, integration and constraint solution.
         *
         * @param timestep The amount of time to simulate (should not vary).
         */
        void step(float timestep);
    };
}

#endif //PHYSICSWORLD_HPP
