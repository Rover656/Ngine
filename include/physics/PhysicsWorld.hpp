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

// Forward declare
class b2World;

namespace ngine::physics {
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
         * Pixels per meter unit.
         */
        const int m_pixelsPerMeter;

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
    public:
        /**
         *
         * @param gravity The gravity vector.
         * @param ppm The number of pixels per meter (for internal calculations). Normally the pixel height of your 2D character.
         */
        PhysicsWorld(Vector2 gravity, int ppm);

        ~PhysicsWorld();

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
        void setGravity(Vector2 gravity);

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

        bool getWarmStarting() const;

        void setWarmStarting(bool flag);

        bool getContinuousPhysics() const;

        void setContinuousPhysics(bool flag);

        bool getSubStepping() const;

        void setSubStepping(bool flag);

        /**
         * Enable debug draw.
         */
        void enableDebugDraw();

        /**
         * Disable debug draw.
         */
        void disableDebugDraw();

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

        /**
         * Convert a dimension in pixels to meters.
         *
         * @tparam T Type to convert.
         * @param pixels Dimension in pixels.
         * @return The dimension in meters.
         */
        template <typename T>
        T convertPixelsToMeters(T pixels) const {
            return pixels / m_pixelsPerMeter;
        }

        /**
         * Convert a dimension in meters to pixels.
         *
         * @tparam T Type to convert.
         * @param pixels Dimension in meters.
         * @return The dimension in pixels.
         */
        template <typename T>
        T convertMetersToPixels(T meters) const {
            return meters / m_pixelsPerMeter;
        }
    };
}

#endif //PHYSICSWORLD_HPP
