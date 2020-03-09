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

#include "ngine/physics/PhysicsBody.hpp"

#include "ngine/Console.hpp"
#include "ngine/physics/PhysicsWorld.hpp"


#include <box2d/box2d.h>

namespace ngine::physics {
    PhysicsBody::PhysicsBody(const PhysicsContext *context, PhysicsWorld *world,
                             b2Body *body)
        : m_context(context) {
        // Save body and world
        m_body = body;
        m_world = world;

        // Save ourselves
        m_body->SetUserData(this);
    }

    void PhysicsBody::_destroy() {
        // Destroy fixtures
        std::vector<PhysicsFixture *> fixtures;
        fixtures = m_fixtures;
        for (auto f : fixtures) {
            destroyFixture(f);
        }
    }

    PhysicsFixture *PhysicsBody::createFixture(PhysicsShape *shape,
                                               float density) {
        // Check shape context
        if (shape->m_context != m_context)
            Console::fail(
                "PhysicsBody",
                "Shape must be same context as body to create a fixture!");

        // Create
        auto f = new PhysicsFixture(
            m_context, m_body->CreateFixture(shape->m_shape, density));

        // Track
        m_fixtures.push_back(f);
        return f;
    }

    void PhysicsBody::destroyFixture(PhysicsFixture *fixture) {
        // Destroy
        m_body->DestroyFixture(fixture->m_fixture);

        // Stop tracking
        m_fixtures.erase(
            std::remove(m_fixtures.begin(), m_fixtures.end(), fixture),
            m_fixtures.end());

        // Delete
        delete fixture;
    }

    PhysicsWorld *PhysicsBody::getWorld() const { return m_world; }

    Vector2 PhysicsBody::getPosition() const {
        auto p = m_body->GetPosition();
        return m_context->convertMetersToPixels(Vector2(p.x, p.y));
    }

    void PhysicsBody::setPosition(const Vector2 &position) {
        setTransform(position, getRotation());
    }

    float PhysicsBody::getRotation() const {
        return -RadToDeg(m_body->GetAngle());
    }

    void PhysicsBody::setRotation(float rotation) {
        setTransform(getPosition(), rotation);
    }

    Transform2D PhysicsBody::getTransform() const {
        auto tr = m_body->GetTransform();
        auto rot = -RadToDeg(tr.q.GetAngle());
        auto pos = m_context->convertMetersToPixels(Vector2(tr.p.x, tr.p.y));
        return Transform2D(pos, rot);
    }

    void PhysicsBody::setTransform(const Vector2 &position, float angle) {
        auto pos = m_context->convertPixelsToMeters(position);
        auto rad = DegToRad(-angle);
        m_body->SetTransform({pos.X, pos.Y}, rad);
    }

    Vector2 PhysicsBody::getWorldCenter() const {
        auto c = m_body->GetWorldCenter();
        return m_context->convertMetersToPixels(Vector2(c.x, c.y));
    }

    Vector2 PhysicsBody::getLocalCenter() const {
        auto c = m_body->GetLocalCenter();
        return m_context->convertMetersToPixels(Vector2(c.x, c.y));
    }

    Vector2 PhysicsBody::getLinearVelocity() const {
        auto v = m_body->GetLinearVelocity();
        return m_context->convertMetersToPixels(Vector2(v.x, v.y));
    }

    void PhysicsBody::setLinearVelocity(const Vector2 &velocity) {
        auto v = m_context->convertPixelsToMeters(velocity);
        m_body->SetLinearVelocity({v.X, v.Y});
    }

    float PhysicsBody::getAngularVelocity() const {
        return -RadToDeg(m_body->GetAngularVelocity());
    }

    void PhysicsBody::setAngularVelocity(float velocity) {
        m_body->SetAngularVelocity(-DegToRad(velocity));
    }

    float PhysicsBody::getMass() const { return m_body->GetMass(); }

    float PhysicsBody::getInertia() const {
        // TODO: Verify unit conversion
        auto oneM = m_context->convertMetersToPixels(1.0f);
        return m_body->GetInertia() * oneM * oneM;
    }

    MassData PhysicsBody::getMassData() const {
        auto oneM = m_context->convertMetersToPixels(1.0f);

        b2MassData d;
        m_body->GetMassData(&d);

        MassData data;
        data.Center =
            m_context->convertMetersToPixels(Vector2(d.center.x, d.center.y));
        data.Mass = d.mass;
        data.Inertia = d.I * oneM * oneM;

        return data;
    }

    void PhysicsBody::setMassData(const MassData &data) {
        auto c = m_context->convertPixelsToMeters(data.Center);
        auto oneM = m_context->convertMetersToPixels(1.0f);

        b2MassData dat;
        dat.center = {c.X, c.Y};
        dat.I = data.Inertia / (oneM * oneM);
        dat.mass = data.Mass;

        m_body->SetMassData(&dat);
    }

    void PhysicsBody::resetMassData() { m_body->ResetMassData(); }

    PhysicsBody::Type PhysicsBody::getType() const {
        switch (m_body->GetType()) {
        case b2_staticBody:
            return Type::Static;
        case b2_kinematicBody:
            return Type::Kinematic;
        case b2_dynamicBody:
            return Type::Dynamic;
        }
    }

    void PhysicsBody::setType(PhysicsBody::Type type) {
        switch (type) {
        case Type::Static:
            m_body->SetType(b2_staticBody);
            break;
        case Type::Kinematic:
            m_body->SetType(b2_kinematicBody);
            break;
        case Type::Dynamic:
            m_body->SetType(b2_dynamicBody);
            break;
        }
    }

    PhysicsBody *PhysicsBody::getNext() {
        auto nxt = m_body->GetNext();
        if (nxt == nullptr)
            return nullptr;
        return (PhysicsBody *)nxt->GetUserData();
    }

    const PhysicsBody *PhysicsBody::getNext() const {
        auto nxt = m_body->GetNext();
        if (nxt == nullptr)
            return nullptr;
        return (PhysicsBody *)nxt->GetUserData();
    }
} // namespace ngine::physics
