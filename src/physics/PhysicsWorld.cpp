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

#include "physics/PhysicsWorld.hpp"

#include "PhysicsDebugDraw.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics {
    PhysicsWorld::PhysicsWorld(PhysicsContext *context, const Vector2 &gravity) : m_context(context) {
        // Convert gravity
        auto grav = m_context->convertPixelsToMeters(gravity);

        // Create Box2D world
        m_world = new b2World({grav.X, grav.Y});
    }

    PhysicsWorld::~PhysicsWorld() {
        // Delete bodies
        std::vector<PhysicsBody *> bodies;
        bodies = m_bodies;
        for (auto b : bodies) {
            delete b;
        }

        // Delete world
        delete m_world;
        m_world = nullptr;
    }

    const PhysicsContext *PhysicsWorld::getContext() const {
        return m_context;
    }

    PhysicsBody *PhysicsWorld::createBody(PhysicsBody::Type type, const Vector2 &position) {
        // Build params
        auto pos = m_context->convertPixelsToMeters(position);
        b2BodyDef def;
        def.position = {pos.X, pos.Y};
        switch (type) {
            case PhysicsBody::Type::Static:
                def.type = b2_staticBody;
                break;
            case PhysicsBody::Type::Kinematic:
                def.type = b2_kinematicBody;
                break;
            case PhysicsBody::Type::Dynamic:
                def.type = b2_dynamicBody;
                break;
        }

        // Create
        auto body = new PhysicsBody(m_context, this, m_world->CreateBody(&def));

        // Track
        m_bodies.push_back(body);
        return body;
    }

    void PhysicsWorld::destroyBody(PhysicsBody *body) {
        // Destroy body's fixtures
        body->_destroy();

        // Destroy and delete body
        m_world->DestroyBody(body->m_body);

        // Stop tracking
        m_bodies.erase(std::remove(m_bodies.begin(), m_bodies.end(), body), m_bodies.end());

        // Delete
        delete body;
    }

    int PhysicsWorld::getBodyCount() const {
        return m_world->GetBodyCount();
    }

    int PhysicsWorld::getTreeHeight() const {
        return m_world->GetTreeHeight();
    }

    int PhysicsWorld::getTreeBalance() const {
        return m_world->GetTreeBalance();
    }

    float PhysicsWorld::getTreeQuality() const {
        return m_world->GetTreeQuality();
    }

    Vector2 PhysicsWorld::getGravity() const {
        auto g = m_world->GetGravity();
        return m_context->convertMetersToPixels(Vector2(g.x, g.y));
    }

    void PhysicsWorld::setGravity(const Vector2 &gravity) {
        auto g = m_context->convertPixelsToMeters(gravity);
        m_world->SetGravity(b2Vec2(g.X, g.Y));
    }

    bool PhysicsWorld::getAutoClearForces() const {
        return m_world->GetAutoClearForces();
    }

    void PhysicsWorld::setAutoClearForces(bool flag) {
        m_world->SetAutoClearForces(flag);
    }

    void PhysicsWorld::clearForces() {
        m_world->ClearForces();
    }

    bool PhysicsWorld::getAllowSleeping() const {
        return m_world->GetAllowSleeping();
    }

    void PhysicsWorld::setAllowSleeping(bool flag) {
        m_world->SetAllowSleeping(flag);
    }

    bool PhysicsWorld::getWarmStarting() const {
        return m_world->GetWarmStarting();
    }

    void PhysicsWorld::setWarmStarting(bool flag) {
        m_world->SetWarmStarting(flag);
    }

    bool PhysicsWorld::getContinuousPhysics() const {
        return m_world->GetContinuousPhysics();
    }

    void PhysicsWorld::setContinuousPhysics(bool flag) {
        m_world->SetContinuousPhysics(flag);
    }

    bool PhysicsWorld::getSubStepping() const {
        return m_world->GetSubStepping();
    }

    void PhysicsWorld::setSubStepping(bool flag) {
        m_world->SetSubStepping(flag);
    }

    bool PhysicsWorld::isDebugDrawEnabled() const {
        return m_debugDraw != nullptr;
    }

    void PhysicsWorld::enableDebugDraw() {
        // Create debug drawer
        if (m_debugDraw != nullptr) return;
        m_debugDraw = new PhysicsDebugDraw(this);

        // Set debug drawer
        m_world->SetDebugDraw(m_debugDraw);
    }

    void PhysicsWorld::disableDebugDraw() {
        if (m_debugDraw != nullptr) {
            // Remove and delete.
            m_world->SetDebugDraw(nullptr);
            delete m_debugDraw;
            m_debugDraw = nullptr;
        }
    }

    void PhysicsWorld::setDebugDrawFlags(unsigned int flags) {
        if (m_debugDraw != nullptr)
            m_debugDraw->SetFlags(flags);
    }

    unsigned int PhysicsWorld::getDebugDrawFlags() const {
        if (m_debugDraw != nullptr)
            return m_debugDraw->GetFlags();
        return 0;
    }

    void PhysicsWorld::debugDraw(graphics::Renderer *renderer) {
        // Set renderer and draw
        if (m_debugDraw != nullptr) {
            m_debugDraw->setRenderer(renderer);
            m_world->DrawDebugData();
        }
    }

    int PhysicsWorld::getVelocityIterations() const {
        return m_velocityIterations;
    }

    void PhysicsWorld::setVelocityIterations(int iterations) {
        m_velocityIterations = iterations;
    }

    int PhysicsWorld::getPositionIterations() const {
        return m_positionIterations;
    }

    void PhysicsWorld::setPositionIterations(int iterations) {
        m_positionIterations = iterations;
    }

    void PhysicsWorld::step(float timestep) {
        m_world->Step(timestep, m_velocityIterations, m_positionIterations);
    }
}