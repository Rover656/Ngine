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

#include "PhysicsWorld.hpp"

#include <Box2D/Box2D.h>

namespace NerdThings::Ngine::Physics {
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

    void PhysicsWorld::ClearForces() {
        _B2World->ClearForces();
    }

    PhysicsBody *PhysicsWorld::CreateBody(PhysicsBody::BodyInfo info_) {
        return new PhysicsBody(this, info_);
    }

    void PhysicsWorld::DestroyBody(PhysicsBody *body_) {
        _B2World->DestroyBody(body_->GetB2Body());
        delete body_;
    }

    bool PhysicsWorld::GetAllowSleeping() {
        return _B2World->GetAllowSleeping();
    }

    b2World *PhysicsWorld::GetB2World() {
        return _B2World;
    }

    int PhysicsWorld::GetBodyCount() {
        return _B2World->GetBodyCount();
    }

    int PhysicsWorld::GetContactCount() {
        return _B2World->GetContactCount();
    }

    bool PhysicsWorld::GetContinuousPhysics() {
        return _B2World->GetContinuousPhysics();
    }

    Vector2 PhysicsWorld::GetGravity() {
        auto vec = _B2World->GetGravity();
        return {vec.x, vec.y};
    }

    Vector2 PhysicsWorld::GetGravityPixels() {
        auto vec = _B2World->GetGravity();
        return {vec.x * PixelsPerMeter, vec.y * PixelsPerMeter};
    }

    int PhysicsWorld::GetJointCount() {
        return _B2World->GetJointCount();
    }

    int PhysicsWorld::GetProxyCount() {
        return _B2World->GetProxyCount();
    }

    bool PhysicsWorld::GetSubStepping() {
        return _B2World->GetSubStepping();
    }

    bool PhysicsWorld::GetTreeBalance() {
        return _B2World->GetTreeBalance();
    }

    bool PhysicsWorld::GetTreeHeight() {
        return _B2World->GetTreeHeight();
    }

    bool PhysicsWorld::GetWarmStarting() {
        return _B2World->GetWarmStarting();
    }

    bool PhysicsWorld::IsLocked() {
        return _B2World->IsLocked();
    }

    void PhysicsWorld::SetAllowSleeping(bool flag_) {
        _B2World->SetAllowSleeping(flag_);
    }

    void PhysicsWorld::SetAutoClearForces(bool flag_) {
        _B2World->SetAutoClearForces(flag_);
    }

    void PhysicsWorld::SetContinuousPhysics(bool flag_) {
        _B2World->SetContinuousPhysics(flag_);
    }

    void PhysicsWorld::SetGravity(const Vector2 &gravVector) {
        _B2World->SetGravity({gravVector.X, gravVector.Y});
    }

    void PhysicsWorld::SetSubStepping(bool flag_) {
        _B2World->SetSubStepping(flag_);
    }

    void PhysicsWorld::SetWarmStarting(bool flag_) {
        _B2World->SetWarmStarting(flag_);
    }

    void PhysicsWorld::Step(float timeStep, int velocityIterations, int positionIterations) {
        _B2World->Step(timeStep, velocityIterations, positionIterations);
    }
}