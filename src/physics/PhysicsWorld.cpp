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

#include <Box2D/Box2D.h>

namespace ngine::physics {
    // Public Constructor

    PhysicsWorld::PhysicsWorld(const Vector2 &gravVector, float ppm)
            : PixelsPerMeter(ppm) {
        _B2World = new b2World({gravVector.X, gravVector.Y});
        _B2World->SetAllowSleeping(true);
    }

    // Destructor

    PhysicsWorld::~PhysicsWorld() {
        // Delete bodies
        auto next = _B2World->GetBodyList();
        b2Body *current;
        while (next != nullptr) {
            current = next;
            next = next->GetNext();
            if (current->GetUserData() != nullptr) {
                delete (PhysicsBody *)current->GetUserData();
            }
        }

        // Delete world.
        delete _B2World;
    }

    // Public Methods

    void PhysicsWorld::clearForces() {
        _B2World->ClearForces();
    }

    PhysicsBody *PhysicsWorld::createBody(PhysicsBody::BodyInfo info) {
        return new PhysicsBody(this, info);
    }

    void PhysicsWorld::destroyBody(PhysicsBody *body) {
        _B2World->DestroyBody(body->getB2Body());
        delete body;
    }

    bool PhysicsWorld::getAllowSleeping() {
        return _B2World->GetAllowSleeping();
    }

    b2World *PhysicsWorld::getB2World() {
        return _B2World;
    }

    int PhysicsWorld::getBodyCount() {
        return _B2World->GetBodyCount();
    }

    int PhysicsWorld::getContactCount() {
        return _B2World->GetContactCount();
    }

    bool PhysicsWorld::getContinuousPhysics() {
        return _B2World->GetContinuousPhysics();
    }

    Vector2 PhysicsWorld::getGravity() {
        auto vec = _B2World->GetGravity();
        return {vec.x, vec.y};
    }

    Vector2 PhysicsWorld::getGravityPixels() {
        auto vec = _B2World->GetGravity();
        return {vec.x * PixelsPerMeter, vec.y * PixelsPerMeter};
    }

    int PhysicsWorld::getJointCount() {
        return _B2World->GetJointCount();
    }

    int PhysicsWorld::getProxyCount() {
        return _B2World->GetProxyCount();
    }

    bool PhysicsWorld::getSubStepping() {
        return _B2World->GetSubStepping();
    }

    bool PhysicsWorld::getTreeBalance() {
        return _B2World->GetTreeBalance();
    }

    bool PhysicsWorld::getTreeHeight() {
        return _B2World->GetTreeHeight();
    }

    bool PhysicsWorld::getWarmStarting() {
        return _B2World->GetWarmStarting();
    }

    bool PhysicsWorld::isLocked() {
        return _B2World->IsLocked();
    }

    void PhysicsWorld::setAllowSleeping(bool flag) {
        _B2World->SetAllowSleeping(flag);
    }

    void PhysicsWorld::setAutoClearForces(bool flag) {
        _B2World->SetAutoClearForces(flag);
    }

    void PhysicsWorld::setContinuousPhysics(bool flag) {
        _B2World->SetContinuousPhysics(flag);
    }

    void PhysicsWorld::setGravity(const Vector2 &gravVector) {
        _B2World->SetGravity({gravVector.X, gravVector.Y});
    }

    void PhysicsWorld::setSubStepping(bool flag) {
        _B2World->SetSubStepping(flag);
    }

    void PhysicsWorld::setWarmStarting(bool flag) {
        _B2World->SetWarmStarting(flag);
    }

    void PhysicsWorld::step(float timeStep, int velocityIterations, int positionIterations) {
        _B2World->Step(timeStep, velocityIterations, positionIterations);
    }
}