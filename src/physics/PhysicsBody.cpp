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

#include "physics/PhysicsWorld.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics {
    PhysicsBody::PhysicsBody(b2Body *body) {
        // Save body
        m_b2Body = body;

        // Get world
        m_world = ((PhysicsBody *) m_b2Body->GetUserData())->m_world;
    }

    PhysicsBody::PhysicsBody(PhysicsWorld *world, PhysicsBody::BodyInfo info) {
        // Save world
        m_world = world;

        // Create body def
        b2BodyDef bodyDef;

        // Set fields
        bodyDef.active = info.Active;
        bodyDef.allowSleep = info.AllowSleep;
        bodyDef.angularDamping = info.AngularDamping;
        bodyDef.angularVelocity = info.AngularVelocity;
        bodyDef.awake = info.Awake;
        bodyDef.bullet = info.Bullet;
        bodyDef.fixedRotation = info.FixedRotation;
        bodyDef.gravityScale = info.GravityScale;
        bodyDef.linearDamping = info.LinearDamping;
        bodyDef.linearVelocity.Set(info.LinearVelocity.X, info.LinearVelocity.Y);
        bodyDef.position.Set(info.Position.X / m_world->PixelsPerMeter, info.Position.Y / m_world->PixelsPerMeter);
        bodyDef.angle = DegToRad(info.Rotation.getDegrees());
        bodyDef.type = (b2BodyType) info.Type;

        // Create the body
        m_b2Body = world->getB2World()->CreateBody(&bodyDef);

        // Save our PhysicsBody in box2d for later.
        m_b2Body->SetUserData(this);
    }

    PhysicsBody::PhysicsBody(PhysicsWorld *world, PhysicsBody::BodyType type, Vector2 position) {
        // Save world
        m_world = world;

        // Create body def
        b2BodyDef bodyDef;
        bodyDef.type = (b2BodyType) type;
        bodyDef.position.Set(position.X / m_world->PixelsPerMeter, position.Y / m_world->PixelsPerMeter);

        // Create the body
        m_b2Body = world->getB2World()->CreateBody(&bodyDef);

        // Save our PhysicsBody in box2d for later.
        m_b2Body->SetUserData(this);
    }

    PhysicsBody::~PhysicsBody() {
        // Delete fixtures
        auto next = m_b2Body->GetFixtureList();
        b2Fixture *current;
        while (next != nullptr) {
            current = next;
            next = next->GetNext();
            if (current->GetUserData() != nullptr) {
                delete (PhysicsFixture *) current->GetUserData();
            }
        }
    }

    void PhysicsBody::set(PhysicsBody::BodyInfo info) {
        setIsActive(info.Active);
        setIsSleepingAllowed(info.AllowSleep);
        setAngularDamping(info.AngularDamping);
        setAngularVelocity(info.AngularVelocity);
        setIsAwake(info.Awake);
        setIsBullet(info.Bullet);
        setIsFixedRotation(info.FixedRotation);
        setGravityScale(info.GravityScale);
        setLinearDamping(info.LinearDamping);
        setLinearVelocity(info.LinearVelocity);
        setTransform({info.Position, info.Rotation});
        setType(info.Type);
    }

    void PhysicsBody::destroy() {
        m_world->destroyBody(this);
    }

    PhysicsBody::BodyType PhysicsBody::getType() const {
        return (BodyType) m_b2Body->GetType();
    }

    void PhysicsBody::setType(PhysicsBody::BodyType type) {
        m_b2Body->SetType((b2BodyType) type);
    }

    float PhysicsBody::getMass() const {
        return m_b2Body->GetMass();
    }

    void PhysicsBody::resetMassData() {
        m_b2Body->ResetMassData();
    }

    bool PhysicsBody::isBullet() const {
        return m_b2Body->IsBullet();
    }

    void PhysicsBody::setIsBullet(bool flag) {
        m_b2Body->SetBullet(flag);
    }

    b2Body *PhysicsBody::getB2Body() const {
        return m_b2Body;
    }

    PhysicsWorld *PhysicsBody::getWorld() const {
        return m_world;
    }

    float PhysicsBody::getGravityScale() const {
        return m_b2Body->GetGravityScale();
    }

    void PhysicsBody::setGravityScale(float scale) {
        m_b2Body->SetGravityScale(scale);
    }

    Transform2D PhysicsBody::getTransform() const {
        auto t = m_b2Body->GetTransform();
        return {
                {
                        t.p.x * m_world->PixelsPerMeter,
                        t.p.y * m_world->PixelsPerMeter
                },
                {
                        t.q.s,
                        t.q.c
                }};
    }

    void PhysicsBody::setTransform(const Transform2D &transform) {
        m_b2Body->SetTransform({
                                       transform.Position.X / m_world->PixelsPerMeter,
                                       transform.Position.Y / m_world->PixelsPerMeter
                               },
                               DegToRad(transform.Rotation.getDegrees()));
    }

    Vector2 PhysicsBody::getPosition() const {
        auto p = m_b2Body->GetPosition();
        Vector2 pos(
                p.x * m_world->PixelsPerMeter,
                p.y * m_world->PixelsPerMeter);
        return pos;
    }

    void PhysicsBody::setPosition(Vector2 pos) {
        m_b2Body->SetTransform({
                                       pos.X / m_world->PixelsPerMeter,
                                       pos.Y / m_world->PixelsPerMeter
                               }, m_b2Body->GetAngle());
    }

    Vector2 PhysicsBody::getLinearVelocity() const {
        auto vel = m_b2Body->GetLinearVelocity();
        return {
                vel.x * m_world->PixelsPerMeter,
                vel.y * m_world->PixelsPerMeter
        };
    }

    void PhysicsBody::setLinearVelocity(Vector2 vel) {
        m_b2Body->SetLinearVelocity(
                {
                        vel.X / m_world->PixelsPerMeter,
                        vel.Y / m_world->PixelsPerMeter
                });
    }

    void PhysicsBody::applyForce(const Vector2 &force, const Vector2 &point, bool wake) {
        m_b2Body->ApplyForce(
                {
                        force.X / m_world->PixelsPerMeter,
                        force.Y / m_world->PixelsPerMeter
                },
                {
                        point.X / m_world->PixelsPerMeter,
                        point.Y / m_world->PixelsPerMeter
                }, wake);
    }

    void PhysicsBody::applyForceToCenter(const Vector2 &force, bool wake) {
        m_b2Body->ApplyForceToCenter(
                {
                        force.X / m_world->PixelsPerMeter,
                        force.Y / m_world->PixelsPerMeter
                }, wake);
    }

    void PhysicsBody::applyLinearImpulse(const Vector2 &impulse, const Vector2 &point, bool wake) {
        m_b2Body->ApplyLinearImpulse(
                {
                        impulse.X / m_world->PixelsPerMeter,
                        impulse.Y / m_world->PixelsPerMeter
                },
                {
                        point.X / m_world->PixelsPerMeter,
                        point.Y / m_world->PixelsPerMeter
                }, wake);
    }

    void PhysicsBody::applyLinearImpulseToCenter(const Vector2 &impulse, bool wake) {
        m_b2Body->ApplyLinearImpulseToCenter(
                {
                        impulse.X / m_world->PixelsPerMeter,
                        impulse.Y / m_world->PixelsPerMeter
                }, wake);
    }

    float PhysicsBody::getLinearDamping() const {
        return m_b2Body->GetLinearDamping();
    }

    void PhysicsBody::setLinearDamping(float damping) {
        m_b2Body->SetLinearDamping(damping);
    }

    Angle PhysicsBody::getRotation() const {
        return getTransform().Rotation;
    }

    void PhysicsBody::setRotation(Angle angle) {
        auto d = angle.getDegrees();
        m_b2Body->SetTransform(m_b2Body->GetPosition(), DegToRad(angle.getDegrees()));
    }

    bool PhysicsBody::isFixedRotation() const {
        return m_b2Body->IsFixedRotation();
    }

    void PhysicsBody::setIsFixedRotation(bool flag) {
        m_b2Body->SetFixedRotation(flag);
    }

    float PhysicsBody::getInertia() const {
        return m_b2Body->GetInertia();
    }

    // TODO: Use Angle class...
    float PhysicsBody::getAngularVelocity() const {
        return m_b2Body->GetAngularVelocity();
    }

    void PhysicsBody::setAngularVelocity(float omega) {
        m_b2Body->SetAngularVelocity(DegToRad(omega));
    }

    void PhysicsBody::applyTorque(float torque, bool wake) {
        m_b2Body->ApplyTorque(torque, wake);
    }

    void PhysicsBody::applyAngularImpulse(float impulse, bool wake) {
        m_b2Body->ApplyAngularImpulse(impulse, wake);
    }

    float PhysicsBody::getAngularDamping() const {
        return m_b2Body->GetAngularDamping();
    }

    void PhysicsBody::setAngularDamping(float damping) {
        m_b2Body->SetAngularDamping(damping);
    }

    // TODO: Rewrite fixtures and shapes once and for all!!!
    PhysicsFixture *PhysicsBody::createFixture(shapes::PhysicsShape *shape, float density) {
        auto fixture = m_b2Body->CreateFixture(shape->getB2Shape(), density);
        return new PhysicsFixture(fixture);
    }

    void PhysicsBody::destroyFixture(PhysicsFixture *fixture) {
        m_b2Body->DestroyFixture(fixture->getB2Fixture());
        delete fixture;
    }

    bool PhysicsBody::isActive() const {
        return m_b2Body->IsActive();
    }

    void PhysicsBody::setIsActive(bool flag) {
        m_b2Body->SetActive(flag);
    }

    bool PhysicsBody::isAwake() const {
        return m_b2Body->IsAwake();
    }

    void PhysicsBody::setIsAwake(bool flag) {
        m_b2Body->SetAwake(flag);
    }

    bool PhysicsBody::isSleepingAllowed() const {
        return m_b2Body->IsSleepingAllowed();
    }

    void PhysicsBody::setIsSleepingAllowed(bool flag) {
        m_b2Body->SetSleepingAllowed(flag);
    }

    void PhysicsBody::debugDraw(graphics::Renderer *renderer) {
        // Process fixtures
        auto next = m_b2Body->GetFixtureList();
        b2Fixture *current;
        while (next != nullptr) {
            current = next;
            next = next->GetNext();
            if (current->GetUserData() != nullptr) {
                auto fixture = (PhysicsFixture *) current->GetUserData();
                fixture->getShape()->debugDraw(renderer, m_world->PixelsPerMeter, getPosition(),
                                               getRotation().getDegrees());
            }
        }
    }
}