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
    PhysicsWorld::PhysicsWorld(Vector2 gravity, int ppm)
            : m_pixelsPerMeter(ppm) {
        // Convert gravity
        auto grav = convertPixelsToMeters(gravity);

        // Create Box2D world
        m_world = new b2World({grav.X, grav.Y});
    }

    PhysicsWorld::~PhysicsWorld() {
        // Delete world
        delete m_world;
        m_world = nullptr;
    }

    PhysicsBody *PhysicsWorld::createBody(PhysicsBodyType type, Vector2 position) {
        auto pos = convertPixelsToMeters(position);
        b2BodyDef def;
        def.position = {pos.X, pos.Y};
        return new PhysicsBody(this, m_world->CreateBody(&def));
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
        return convertMetersToPixels(Vector2(g.x, g.y));
    }

    void PhysicsWorld::setGravity(Vector2 gravity) {
        auto g = convertPixelsToMeters(gravity);
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

    void PhysicsWorld::enableDebugDraw() {
        // Create debug drawer
        if (m_debugDraw != nullptr)
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

    void PhysicsWorld::debugDraw(graphics::Renderer *renderer) {
        // Set renderer and draw
        m_debugDraw->setRenderer(renderer);
        m_world->DrawDebugData();
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