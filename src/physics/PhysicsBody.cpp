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

#include "physics/PhysicsBody.hpp"

#include "physics/PhysicsSpace.hpp"
#include "Console.hpp"

#include <chipmunk/chipmunk.h>

namespace ngine::physics {
    PhysicsBody::PhysicsBody(cpBody *body) {
        // Save body
        m_body = body;

        // Save ourselves within the body
        cpBodySetUserData(m_body, this);
    }

    PhysicsBody::~PhysicsBody() {
        // TODO: Get list of shapes and remove.
        if (m_body != nullptr) {
            cpBodyFree(m_body);
            m_body = nullptr;
        }
    }

    PhysicsBody *PhysicsBody::CreateDynamicBody(float mass, float moment) {
        // Create new body
        return new PhysicsBody(cpBodyNew(mass, moment));
    }

    PhysicsBody *PhysicsBody::CreateKinematicBody() {
        return new PhysicsBody(cpBodyNewKinematic());
    }

    PhysicsBody *PhysicsBody::CreateStaticBody() {
        return new PhysicsBody(cpBodyNewStatic());
    }

    PhysicsBodyType PhysicsBody::getType() const {
        switch (cpBodyGetType(m_body)) {
            case CP_BODY_TYPE_DYNAMIC:
                return PhysicsBodyType::Dynamic;
            case CP_BODY_TYPE_KINEMATIC:
                return PhysicsBodyType::Kinematic;
            case CP_BODY_TYPE_STATIC:
                return PhysicsBodyType::Static;
        }
    }

    void PhysicsBody::setType(PhysicsBodyType type) {
        switch (type) {
            case PhysicsBodyType::Static:
                cpBodySetType(m_body, CP_BODY_TYPE_STATIC);
                break;
            case PhysicsBodyType::Kinematic:
                cpBodySetType(m_body, CP_BODY_TYPE_KINEMATIC);
                break;
            case PhysicsBodyType::Dynamic:
                cpBodySetType(m_body, CP_BODY_TYPE_DYNAMIC);
                break;
        }
    }

    PhysicsSpace *PhysicsBody::getSpace() {
        auto space = cpBodyGetSpace(m_body);
        if (space != nullptr)
            return (PhysicsSpace *) cpSpaceGetUserData(space);
        return nullptr;
    }

    void PhysicsBody::addShape(PhysicsShape *shape) {
        // Get space.
        auto space = getSpace();
        if (space == nullptr)
            Console::Fail("PhysicsBody", "Body must be a member of a space before having shapes added to it!");

        // Check body is attached to us
        if (shape->getBody() != this)
            shape->setBody(this);

        // Add shape to space
        cpSpaceAddShape(space->m_space, shape->m_shape);

        // TODO: Track.
    }

    void PhysicsBody::removeShape(PhysicsShape *shape, bool free) {
        // Get space.
        auto space = getSpace();
        if (space == nullptr)
            Console::Fail("PhysicsBody", "Body is not attached to a space!");

        // Remove from space
        cpSpaceRemoveShape(space->m_space, shape->m_shape);

        // TODO: Stop tracking

        // If we are told to free this, do so.
        // TODO: Just delete anyway??
        if (free)
            delete shape;
    }

    float PhysicsBody::getMass() {
        return cpBodyGetMass(m_body);
    }

    void PhysicsBody::setMass(float mass) {
        cpBodySetMass(m_body, mass);
    }

    float PhysicsBody::getMoment() {
        return cpBodyGetMoment(m_body);
    }

    void PhysicsBody::setMoment(float moment) {
        cpBodySetMoment(m_body, moment);
    }

    Vector2 PhysicsBody::getCenterOfGravity() {
        auto c = cpBodyGetCenterOfGravity(m_body);
        return {(float)c.x, (float)c.y};
    }

    void PhysicsBody::setCenterOfGravity(const Vector2 &center) {
        cpBodySetCenterOfGravity(m_body, {(double)center.X, (double)center.Y});
    }

    Vector2 PhysicsBody::getPosition() {
        auto v = cpBodyGetPosition(m_body);
        return {(float)v.x, (float)v.y};
    }

    void PhysicsBody::setPosition(const Vector2 &position) {
        cpBodySetPosition(m_body, {(double)position.X, (double)position.Y});

        if (getType() == PhysicsBodyType::Static) {
            reindexShapes();
        }
    }

    Vector2 PhysicsBody::getVelocity() {
        auto v = cpBodyGetVelocity(m_body);
        return {(float)v.x, (float)v.y};
    }

    void PhysicsBody::setVelocity(const Vector2 &vel) {
        cpBodySetVelocity(m_body, {(double)vel.X, (double)vel.Y});
    }

    Vector2 PhysicsBody::getForce() {
        auto f = cpBodyGetForce(m_body);
        return {(float)f.x, (float)f.y};
    }

    void PhysicsBody::setForce(const Vector2 &force) {
        cpBodySetForce(m_body, {(double)force.X, (double)force.Y});
    }

    Angle PhysicsBody::getRotation() {
        return Angle(RadToDeg(cpBodyGetAngle(m_body)));
    }

    void PhysicsBody::setRotation(Angle angle) {
        cpBodySetAngle(m_body, DegToRad(angle.getDegrees()));
    }

    float PhysicsBody::getAngularVelocity() {
        return cpBodyGetAngularVelocity(m_body);
    }

    void PhysicsBody::setAngularVelocity(float vel) {
        cpBodySetAngularVelocity(m_body, vel);
    }

    float PhysicsBody::getTorque() {
        return cpBodyGetTorque(m_body);
    }

    void PhysicsBody::setTorque(float torque) {
        cpBodySetTorque(m_body, torque);
    }

    void PhysicsBody::reindexShapes() {
        auto space = cpBodyGetSpace(m_body);
        if (space == nullptr) return;
        cpSpaceReindexShapesForBody(space, m_body);
    }
}