/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#include "PhysicsBody.hpp"

#include <Box2D/Box2D.h>

#include "PhysicsWorld.hpp"

namespace NerdThings::Ngine::Physics {
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
        bodyDef.angle = DegToRad(info_.Rotation.GetDegrees());
        bodyDef.type = (b2BodyType) info_.Type;

        // Create the body
        m_b2Body = world_->GetB2World()->CreateBody(&bodyDef);

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
        m_b2Body = world_->GetB2World()->CreateBody(&bodyDef);

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

    void PhysicsBody::Set(PhysicsBody::BodyInfo info_) {
        SetIsActive(info_.Active);
        SetIsSleepingAllowed(info_.AllowSleep);
        SetAngularDamping(info_.AngularDamping);
        SetAngularVelocity(info_.AngularVelocity);
        SetIsAwake(info_.Awake);
        SetIsBullet(info_.Bullet);
        SetIsFixedRotation(info_.FixedRotation);
        SetGravityScale(info_.GravityScale);
        SetLinearDamping(info_.LinearDamping);
        SetLinearVelocity(info_.LinearVelocity);
        SetTransform({info_.Position, info_.Rotation});
        SetType(info_.Type);
    }

    void PhysicsBody::Destroy() {
        m_world->DestroyBody(this);
    }

    PhysicsBody::BodyType PhysicsBody::GetType() const {
        return (BodyType) m_b2Body->GetType();
    }

    void PhysicsBody::SetType(PhysicsBody::BodyType type_) {
        m_b2Body->SetType((b2BodyType) type_);
    }

    float PhysicsBody::GetMass() const {
        return m_b2Body->GetMass();
    }

    void PhysicsBody::ResetMassData() {
        m_b2Body->ResetMassData();
    }

    bool PhysicsBody::IsBullet() const {
        return m_b2Body->IsBullet();
    }

    void PhysicsBody::SetIsBullet(bool flag_) {
        m_b2Body->SetBullet(flag_);
    }

    b2Body *PhysicsBody::GetB2Body() const {
        return m_b2Body;
    }

    PhysicsWorld *PhysicsBody::GetWorld() const {
        return m_world;
    }

    float PhysicsBody::GetGravityScale() const {
        return m_b2Body->GetGravityScale();
    }

    void PhysicsBody::SetGravityScale(float scale_) {
        m_b2Body->SetGravityScale(scale_);
    }

    Transform2D PhysicsBody::GetTransform() const {
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

    void PhysicsBody::SetTransform(const Transform2D &transform_) {
        m_b2Body->SetTransform({
                                       transform_.Position.X / m_world->PixelsPerMeter,
                                       transform_.Position.Y / m_world->PixelsPerMeter
                               },
                               DegToRad(transform_.Rotation.GetDegrees()));
    }

    Vector2 PhysicsBody::GetPosition() const {
        auto p = m_b2Body->GetPosition();
        Vector2 pos(
                p.x * m_world->PixelsPerMeter,
                p.y * m_world->PixelsPerMeter);
        return pos;
    }

    void PhysicsBody::SetPosition(Vector2 pos_) {
        m_b2Body->SetTransform({
                                       pos_.X / m_world->PixelsPerMeter,
                                       pos_.Y / m_world->PixelsPerMeter
                               }, m_b2Body->GetAngle());
    }

    Vector2 PhysicsBody::GetLinearVelocity() const {
        auto vel = m_b2Body->GetLinearVelocity();
        return {
                vel.x * m_world->PixelsPerMeter,
                vel.y * m_world->PixelsPerMeter
        };
    }

    void PhysicsBody::SetLinearVelocity(Vector2 vel_) {
        m_b2Body->SetLinearVelocity(
                {
                        vel_.X / m_world->PixelsPerMeter,
                        vel_.Y / m_world->PixelsPerMeter
                });
    }

    void PhysicsBody::ApplyForce(const Vector2 &force_, const Vector2 &point_, bool wake_) {
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

    void PhysicsBody::ApplyForceToCenter(const Vector2 &force_, bool wake_) {
        m_b2Body->ApplyForceToCenter(
                {
                        force_.X / m_world->PixelsPerMeter,
                        force_.Y / m_world->PixelsPerMeter
                }, wake_);
    }

    void PhysicsBody::ApplyLinearImpulse(const Vector2 &impulse_, const Vector2 &point_, bool wake_) {
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

    void PhysicsBody::ApplyLinearImpulseToCenter(const Vector2 &impulse_, bool wake_) {
        m_b2Body->ApplyLinearImpulseToCenter(
                {
                        impulse_.X / m_world->PixelsPerMeter,
                        impulse_.Y / m_world->PixelsPerMeter
                }, wake_);
    }

    float PhysicsBody::GetLinearDamping() const {
        return m_b2Body->GetLinearDamping();
    }

    void PhysicsBody::SetLinearDamping(float damping_) {
        m_b2Body->SetLinearDamping(damping_);
    }

    Angle PhysicsBody::GetRotation() const {
        return GetTransform().Rotation;
    }

    void PhysicsBody::SetRotation(Angle angle_) {
        auto d = angle_.GetDegrees();
        m_b2Body->SetTransform(m_b2Body->GetPosition(), DegToRad(angle_.GetDegrees()));
    }

    bool PhysicsBody::IsFixedRotation() const {
        return m_b2Body->IsFixedRotation();
    }

    void PhysicsBody::SetIsFixedRotation(bool flag_) {
        m_b2Body->SetFixedRotation(flag_);
    }

    float PhysicsBody::GetInertia() const {
        return m_b2Body->GetInertia();
    }

    // TODO: Use Angle class...
    float PhysicsBody::GetAngularVelocity() const {
        return m_b2Body->GetAngularVelocity();
    }

    void PhysicsBody::SetAngularVelocity(float omega_) {
        m_b2Body->SetAngularVelocity(DegToRad(omega_));
    }

    void PhysicsBody::ApplyTorque(float torque_, bool wake_) {
        m_b2Body->ApplyTorque(torque_, wake_);
    }

    void PhysicsBody::ApplyAngularImpulse(float impulse_, bool wake_) {
        m_b2Body->ApplyAngularImpulse(impulse_, wake_);
    }

    float PhysicsBody::GetAngularDamping() const {
        return m_b2Body->GetAngularDamping();
    }

    void PhysicsBody::SetAngularDamping(float damping_) {
        m_b2Body->SetAngularDamping(damping_);
    }

    // TODO: Rewrite fixtures and shapes once and for all!!!
    PhysicsFixture *PhysicsBody::CreateFixture(Shapes::PhysicsShape *shape_, float density_) {
        auto fixture = m_b2Body->CreateFixture(shape_->GetB2Shape(), density_);
        return new PhysicsFixture(fixture);
    }

    void PhysicsBody::DestroyFixture(PhysicsFixture *fixture_) {
        m_b2Body->DestroyFixture(fixture_->GetB2Fixture());
        delete fixture_;
    }

    bool PhysicsBody::IsActive() const {
        return m_b2Body->IsActive();
    }

    void PhysicsBody::SetIsActive(bool flag_) {
        m_b2Body->SetActive(flag_);
    }

    bool PhysicsBody::IsAwake() const {
        return m_b2Body->IsAwake();
    }

    void PhysicsBody::SetIsAwake(bool flag_) {
        m_b2Body->SetAwake(flag_);
    }

    bool PhysicsBody::IsSleepingAllowed() const {
        return m_b2Body->IsSleepingAllowed();
    }

    void PhysicsBody::SetIsSleepingAllowed(bool flag_) {
        m_b2Body->SetSleepingAllowed(flag_);
    }

    void PhysicsBody::DebugDraw() {
        // Process fixtures
        auto next = m_b2Body->GetFixtureList();
        b2Fixture *current;
        while (next != nullptr) {
            current = next;
            next = next->GetNext();
            if (current->GetUserData() != nullptr) {
                auto fixture = (PhysicsFixture *) current->GetUserData();
                fixture->GetShape()->DebugDraw(m_world->PixelsPerMeter, GetPosition(), GetRotation().GetDegrees());
            }
        }
    }
}