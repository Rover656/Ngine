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

#include "Physics/PhysicsBody.hpp"

#include "Physics/PhysicsWorld.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics {
    PhysicsBody::PhysicsBody(b2Body *body_) {
        // Save body
        m_b2Body = body_;

        // Get world
        m_world = ((PhysicsBody *) m_b2Body->GetUserData())->m_world;
    }

    PhysicsBody::PhysicsBody(PhysicsWorld *world_, PhysicsBody::BodyInfo info_) {
        // Save world
        m_world = world_;

        // Create body def
        b2BodyDef bodyDef;

        // Set fields
        bodyDef.active = info_.Active;
        bodyDef.allowSleep = info_.AllowSleep;
        bodyDef.angularDamping = info_.AngularDamping;
        bodyDef.angularVelocity = info_.AngularVelocity;
        bodyDef.awake = info_.Awake;
        bodyDef.bullet = info_.Bullet;
        bodyDef.fixedRotation = info_.FixedRotation;
        bodyDef.gravityScale = info_.GravityScale;
        bodyDef.linearDamping = info_.LinearDamping;
        bodyDef.linearVelocity.Set(info_.LinearVelocity.X, info_.LinearVelocity.Y);
        bodyDef.position.Set(info_.Position.X / m_world->PixelsPerMeter, info_.Position.Y / m_world->PixelsPerMeter);
        bodyDef.angle = DegToRad(info_.Rotation.getDegrees());
        bodyDef.type = (b2BodyType) info_.Type;

        // Create the body
        m_b2Body = world_->getB2World()->CreateBody(&bodyDef);

        // Save our PhysicsBody in box2d for later.
        m_b2Body->SetUserData(this);
    }

    PhysicsBody::PhysicsBody(PhysicsWorld *world_, PhysicsBody::BodyType type_, Vector2 position_) {
        // Save world
        m_world = world_;

        // Create body def
        b2BodyDef bodyDef;
        bodyDef.type = (b2BodyType) type_;
        bodyDef.position.Set(position_.X / m_world->PixelsPerMeter, position_.Y / m_world->PixelsPerMeter);

        // Create the body
        m_b2Body = world_->getB2World()->CreateBody(&bodyDef);

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

    void PhysicsBody::set(PhysicsBody::BodyInfo info_) {
        setIsActive(info_.Active);
        setIsSleepingAllowed(info_.AllowSleep);
        setAngularDamping(info_.AngularDamping);
        setAngularVelocity(info_.AngularVelocity);
        setIsAwake(info_.Awake);
        setIsBullet(info_.Bullet);
        setIsFixedRotation(info_.FixedRotation);
        setGravityScale(info_.GravityScale);
        setLinearDamping(info_.LinearDamping);
        setLinearVelocity(info_.LinearVelocity);
        setTransform({info_.Position, info_.Rotation});
        setType(info_.Type);
    }

    void PhysicsBody::destroy() {
        m_world->destroyBody(this);
    }

    PhysicsBody::BodyType PhysicsBody::getType() const {
        return (BodyType) m_b2Body->GetType();
    }

    void PhysicsBody::setType(PhysicsBody::BodyType type_) {
        m_b2Body->SetType((b2BodyType) type_);
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

    void PhysicsBody::setIsBullet(bool flag_) {
        m_b2Body->SetBullet(flag_);
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

    void PhysicsBody::setGravityScale(float scale_) {
        m_b2Body->SetGravityScale(scale_);
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

    void PhysicsBody::setTransform(const Transform2D &transform_) {
        m_b2Body->SetTransform({
                                       transform_.Position.X / m_world->PixelsPerMeter,
                                       transform_.Position.Y / m_world->PixelsPerMeter
                               },
                               DegToRad(transform_.Rotation.getDegrees()));
    }

    Vector2 PhysicsBody::getPosition() const {
        auto p = m_b2Body->GetPosition();
        Vector2 pos(
                p.x * m_world->PixelsPerMeter,
                p.y * m_world->PixelsPerMeter);
        return pos;
    }

    void PhysicsBody::setPosition(Vector2 pos_) {
        m_b2Body->SetTransform({
                                       pos_.X / m_world->PixelsPerMeter,
                                       pos_.Y / m_world->PixelsPerMeter
                               }, m_b2Body->GetAngle());
    }

    Vector2 PhysicsBody::getLinearVelocity() const {
        auto vel = m_b2Body->GetLinearVelocity();
        return {
                vel.x * m_world->PixelsPerMeter,
                vel.y * m_world->PixelsPerMeter
        };
    }

    void PhysicsBody::setLinearVelocity(Vector2 vel_) {
        m_b2Body->SetLinearVelocity(
                {
                        vel_.X / m_world->PixelsPerMeter,
                        vel_.Y / m_world->PixelsPerMeter
                });
    }

    void PhysicsBody::applyForce(const Vector2 &force_, const Vector2 &point_, bool wake_) {
        m_b2Body->ApplyForce(
                {
                        force_.X / m_world->PixelsPerMeter,
                        force_.Y / m_world->PixelsPerMeter
                },
                {
                        point_.X / m_world->PixelsPerMeter,
                        point_.Y / m_world->PixelsPerMeter
                }, wake_);
    }

    void PhysicsBody::applyForceToCenter(const Vector2 &force_, bool wake_) {
        m_b2Body->ApplyForceToCenter(
                {
                        force_.X / m_world->PixelsPerMeter,
                        force_.Y / m_world->PixelsPerMeter
                }, wake_);
    }

    void PhysicsBody::applyLinearImpulse(const Vector2 &impulse_, const Vector2 &point_, bool wake_) {
        m_b2Body->ApplyLinearImpulse(
                {
                        impulse_.X / m_world->PixelsPerMeter,
                        impulse_.Y / m_world->PixelsPerMeter
                },
                {
                        point_.X / m_world->PixelsPerMeter,
                        point_.Y / m_world->PixelsPerMeter
                }, wake_);
    }

    void PhysicsBody::applyLinearImpulseToCenter(const Vector2 &impulse_, bool wake_) {
        m_b2Body->ApplyLinearImpulseToCenter(
                {
                        impulse_.X / m_world->PixelsPerMeter,
                        impulse_.Y / m_world->PixelsPerMeter
                }, wake_);
    }

    float PhysicsBody::getLinearDamping() const {
        return m_b2Body->GetLinearDamping();
    }

    void PhysicsBody::setLinearDamping(float damping_) {
        m_b2Body->SetLinearDamping(damping_);
    }

    Angle PhysicsBody::getRotation() const {
        return getTransform().Rotation;
    }

    void PhysicsBody::setRotation(Angle angle_) {
        auto d = angle_.getDegrees();
        m_b2Body->SetTransform(m_b2Body->GetPosition(), DegToRad(angle_.getDegrees()));
    }

    bool PhysicsBody::isFixedRotation() const {
        return m_b2Body->IsFixedRotation();
    }

    void PhysicsBody::setIsFixedRotation(bool flag_) {
        m_b2Body->SetFixedRotation(flag_);
    }

    float PhysicsBody::getInertia() const {
        return m_b2Body->GetInertia();
    }

    // TODO: Use Angle class...
    float PhysicsBody::getAngularVelocity() const {
        return m_b2Body->GetAngularVelocity();
    }

    void PhysicsBody::setAngularVelocity(float omega_) {
        m_b2Body->SetAngularVelocity(DegToRad(omega_));
    }

    void PhysicsBody::applyTorque(float torque_, bool wake_) {
        m_b2Body->ApplyTorque(torque_, wake_);
    }

    void PhysicsBody::applyAngularImpulse(float impulse_, bool wake_) {
        m_b2Body->ApplyAngularImpulse(impulse_, wake_);
    }

    float PhysicsBody::getAngularDamping() const {
        return m_b2Body->GetAngularDamping();
    }

    void PhysicsBody::setAngularDamping(float damping_) {
        m_b2Body->SetAngularDamping(damping_);
    }

    // TODO: Rewrite fixtures and shapes once and for all!!!
    PhysicsFixture *PhysicsBody::createFixture(shapes::PhysicsShape *shape_, float density_) {
        auto fixture = m_b2Body->CreateFixture(shape_->getB2Shape(), density_);
        return new PhysicsFixture(fixture);
    }

    void PhysicsBody::destroyFixture(PhysicsFixture *fixture_) {
        m_b2Body->DestroyFixture(fixture_->getB2Fixture());
        delete fixture_;
    }

    bool PhysicsBody::isActive() const {
        return m_b2Body->IsActive();
    }

    void PhysicsBody::setIsActive(bool flag_) {
        m_b2Body->SetActive(flag_);
    }

    bool PhysicsBody::isAwake() const {
        return m_b2Body->IsAwake();
    }

    void PhysicsBody::setIsAwake(bool flag_) {
        m_b2Body->SetAwake(flag_);
    }

    bool PhysicsBody::isSleepingAllowed() const {
        return m_b2Body->IsSleepingAllowed();
    }

    void PhysicsBody::setIsSleepingAllowed(bool flag_) {
        m_b2Body->SetSleepingAllowed(flag_);
    }

    void PhysicsBody::debugDraw(graphics::Renderer *renderer_) {
        // Process fixtures
        auto next = m_b2Body->GetFixtureList();
        b2Fixture *current;
        while (next != nullptr) {
            current = next;
            next = next->GetNext();
            if (current->GetUserData() != nullptr) {
                auto fixture = (PhysicsFixture *) current->GetUserData();
                fixture->getShape()->debugDraw(renderer_, m_world->PixelsPerMeter, getPosition(),
                                               getRotation().getDegrees());
            }
        }
    }
}