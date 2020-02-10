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

#include "physics/PhysicsSpace.hpp"

#include <chipmunk/chipmunk.h>

namespace ngine::physics {
    PhysicsSpace::PhysicsSpace() {
        // Create space.
        m_space = cpSpaceNew();

        // Save this in the userdata
        cpSpaceSetUserData(m_space, this);

        // Reference the built in body
        m_staticBody = new PhysicsBody(cpSpaceGetStaticBody(m_space));
    }

    PhysicsSpace::~PhysicsSpace() {
        // Delete all tracked bodies
        for (auto body : m_trackedBodies) {
            delete body;
        }

        // Delete space
        if (m_space != nullptr) {
            cpSpaceFree(m_space);
            m_space = nullptr;
        }
    }

    void PhysicsSpace::addBody(PhysicsBody *body) {
        // Add to space
        cpSpaceAddBody(m_space, body->m_body);

        // Track
        m_trackedBodies.push_back(body);
    }

    void PhysicsSpace::removeBody(PhysicsBody *body) {
        // Remove from space
        cpSpaceRemoveBody(m_space, body->m_body);

        // Remove from tracking
        m_trackedBodies.erase(std::remove(m_trackedBodies.begin(), m_trackedBodies.end(), body),
                              m_trackedBodies.end());
    }

    bool PhysicsSpace::containsBody(PhysicsBody *body) {
        return cpSpaceContainsBody(m_space, body->m_body);
    }

    int PhysicsSpace::getIterations() const {
        return cpSpaceGetIterations(m_space);
    }

    void PhysicsSpace::setIterations(int iterations) {
        cpSpaceSetIterations(m_space, iterations);
    }

    Vector2 PhysicsSpace::getGravity() const {
        auto g = cpSpaceGetGravity(m_space);
        return {(float) g.x, (float) g.y};
    }

    void PhysicsSpace::setGravity(const Vector2 &grav) {
        cpSpaceSetGravity(m_space, {(double) grav.X, (double) grav.Y});
    }

    float PhysicsSpace::getIdleSpeedThreshold() const {
        return cpSpaceGetIdleSpeedThreshold(m_space);
    }

    void PhysicsSpace::setIdleSpeedThreshold(float threshold) {
        cpSpaceSetIdleSpeedThreshold(m_space, threshold);
    }

    float PhysicsSpace::getSleepTimeThreshold() const {
        return cpSpaceGetSleepTimeThreshold(m_space);
    }

    void PhysicsSpace::setSleepTimeThreshold(float threshold) {
        cpSpaceSetSleepTimeThreshold(m_space, threshold);
    }

    float PhysicsSpace::getCollisionSlop() const {
        return cpSpaceGetCollisionSlop(m_space);
    }

    void PhysicsSpace::setCollisionSlop(float value) {
        cpSpaceSetCollisionSlop(m_space, value);
    }

    float PhysicsSpace::getCollisionBias() const {
        return cpSpaceGetCollisionBias(m_space);
    }

    void PhysicsSpace::setCollisionBias(float bias) {
        cpSpaceSetCollisionBias(m_space, bias);
    }

    unsigned int PhysicsSpace::getCollisionPersistence() const {
        return cpSpaceGetCollisionBias(m_space);
    }

    void PhysicsSpace::setCollisionPersistence(unsigned int persistence) {
        cpSpaceSetCollisionPersistence(m_space, persistence);
    }

    float PhysicsSpace::getCurrentTimeStep() const {
        return cpSpaceGetCurrentTimeStep(m_space);
    }

    bool PhysicsSpace::isLocked() const {
        return cpSpaceIsLocked(m_space);
    }

    PhysicsBody *PhysicsSpace::getStaticBody() const {
        return m_staticBody;
    }

    void PhysicsSpace::step(float dt) {
        cpSpaceStep(m_space, dt);
    }

    void PhysicsSpace::useSpatialHash(float dim, int count) {
        cpSpaceUseSpatialHash(m_space, dim, count);
    }
}