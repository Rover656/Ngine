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

#ifndef PHYSICSSPACE_HPP
#define PHYSICSSPACE_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "PhysicsBody.hpp"

// Forward declare Box2D stuff
class cpSpace;

namespace ngine::physics {
    class NEAPI PhysicsSpace final {
        friend class PhysicsBody;

        /**
         * The chipmunk space
         */
        cpSpace *m_space;

        /**
         * The static body reference.
         */
        PhysicsBody *m_staticBody;

        /**
         * List of the tracked bodies for deletion.
         */
        std::vector<PhysicsBody *> m_trackedBodies;
    public:
        /**
         * Create a new physics space.
         */
        PhysicsSpace();
        ~PhysicsSpace();

        void addBody(PhysicsBody *body);

        void removeBody(PhysicsBody *body);

        bool containsBody(PhysicsBody *body);

        int getIterations() const;

        void setIterations(int iterations);

        Vector2 getGravity() const;

        void setGravity(const Vector2 &grav);

        float getIdleSpeedThreshold() const;

        void setIdleSpeedThreshold(float threshold);

        float getSleepTimeThreshold() const;

        void setSleepTimeThreshold(float threshold);

        float getCollisionSlop() const;

        void setCollisionSlop(float value);

        float getCollisionBias() const;

        void setCollisionBias(float bias);

        unsigned int getCollisionPersistence() const;

        void setCollisionPersistence(unsigned int persistence);

        float getCurrentTimeStep() const;

        bool isLocked() const;

        PhysicsBody *getStaticBody() const;

        /**
         * Update space for given time step.
         *
         * @param dt Delta time, should be 1/FPS.
         */
        void step(float dt);

        void useSpatialHash(float dim, int count);

        // TODO: Iterators
    };
}

#endif //PHYSICSSPACE_HPP
