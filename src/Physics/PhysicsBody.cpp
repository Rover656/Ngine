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

#include "PhysicsBody.h"

#include "PhysicsWorld.h"

namespace NerdThings::Ngine::Physics {
    // Public Constructor

    PhysicsBody::PhysicsBody(b2Body *body_) {
        // Save body
        _B2Body = body_;

        // Get world
        _World = ((PhysicsBody *)_B2Body->GetUserData())->_World;
    }

    PhysicsBody::PhysicsBody(PhysicsWorld *world_, BodyInfo info_) {
        // Save world
        _World = world_;

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
        bodyDef.position.Set(info_.Position.X / _World->PixelsPerMeter, info_.Position.Y / _World->PixelsPerMeter);
        bodyDef.angle = info_.Rotation;
        bodyDef.type = (b2BodyType)info_.Type;

        // Create the body
        _B2Body = world_->GetB2World()->CreateBody(&bodyDef);

        // Save our PhysicsBody in box2d for later.
        _B2Body->SetUserData(this);
    }

    PhysicsBody::PhysicsBody(PhysicsWorld *world_, PhysicsBody::BodyType type_, Vector2 position_) {
        // Save world
        _World = world_;

        // Create body def
        b2BodyDef bodyDef;
        bodyDef.type = (b2BodyType)type_;
        bodyDef.position.Set(position_.X / _World->PixelsPerMeter, position_.Y / _World->PixelsPerMeter);

        // Create the body
        _B2Body = world_->GetB2World()->CreateBody(&bodyDef);

        // Save our PhysicsBody in box2d for later.
        _B2Body->SetUserData(this);
    }

    // Destructor

    PhysicsBody::~PhysicsBody() {
        // Delete fixtures
        auto next = _B2Body->GetFixtureList();
        b2Fixture *current;
        while (next != nullptr) {
            current = next;
            next = next->GetNext();
            if (current->GetUserData() != nullptr) {
                delete (PhysicsFixture *)current->GetUserData();
            }
        }
    }

    // Public Methods

    void PhysicsBody::ApplyAngularImpulse(float impulse_, bool wake_) {
        _B2Body->ApplyAngularImpulse(impulse_, wake_);
    }

    void PhysicsBody::ApplyForce(const Vector2 &force_, const Vector2 &point_, bool wake_) {
        _B2Body->ApplyForce({force_.X, force_.Y}, {point_.X, point_.Y}, wake_);
    }

    void PhysicsBody::ApplyForceToCenter(const Vector2 &force_, bool wake_) {
        _B2Body->ApplyForceToCenter({force_.X, force_.Y}, wake_);
    }

    void PhysicsBody::ApplyLinearImpulse(const Vector2 &impulse_, const Vector2 &point_, bool wake_) {
        _B2Body->ApplyLinearImpulse({impulse_.X, impulse_.Y}, {point_.X, point_.Y}, wake_);
    }

    void PhysicsBody::ApplyLinearImpulseToCenter(const Vector2 &impulse_, bool wake_) {
        _B2Body->ApplyLinearImpulseToCenter({impulse_.X, impulse_.Y}, wake_);
    }

    void PhysicsBody::ApplyTorque(float torque_, bool wake_) {
        _B2Body->ApplyTorque(torque_, wake_);
    }

    PhysicsFixture *PhysicsBody::CreateFixture(Shapes::PhysicsShape *shape_, float density_) {
        auto fixture = _B2Body->CreateFixture(shape_->GetB2Shape(), density_);
        return new PhysicsFixture(fixture);
    }

    void PhysicsBody::DebugDraw() {
        // Delete fixtures
        auto next = _B2Body->GetFixtureList();
        b2Fixture *current;
        while (next != nullptr) {
            current = next;
            next = next->GetNext();
            if (current->GetUserData() != nullptr) {
                auto fixture = (PhysicsFixture *)current->GetUserData();
                fixture->GetShape()->DebugDraw(_World->PixelsPerMeter, GetPosition(), GetRotation());
            }
        }
    }

    void PhysicsBody::Destroy() {
        _World->DestroyBody(this);
    }

    void PhysicsBody::DestroyFixture(PhysicsFixture *fixture_) {
        _B2Body->DestroyFixture(fixture_->GetB2Fixture());
        delete fixture_;
    }

    float PhysicsBody::GetAngularDamping() const {
        return _B2Body->GetAngularDamping();
    }

    float PhysicsBody::GetAngularVelocity() const {
        return _B2Body->GetAngularVelocity();
    }

    b2Body *PhysicsBody::GetB2Body() const {
        return _B2Body;
    }

    float PhysicsBody::GetGravityScale() const {
        return _B2Body->GetGravityScale();
    }

    float PhysicsBody::GetInertia() const {
        return _B2Body->GetInertia();
    }

    float PhysicsBody::GetLinearDamping() const {
        return _B2Body->GetLinearDamping();
    }

    Vector2 PhysicsBody::GetLinearVelocity() const {
        auto vel = _B2Body->GetLinearVelocity();
        return {vel.x, vel.y};
    }

    Vector2 PhysicsBody::GetLinearVelocityPixels() const {
        auto vel = _B2Body->GetLinearVelocity();
        return {vel.x * _World->PixelsPerMeter, vel.y * _World->PixelsPerMeter};
    }

    float PhysicsBody::GetMass() const {
        return _B2Body->GetMass();
    }

    Vector2 PhysicsBody::GetPosition() const {
        auto p = _B2Body->GetPosition();
        Vector2 pos(p.x * _World->PixelsPerMeter, p.y * _World->PixelsPerMeter);
        return pos;
    }

    float PhysicsBody::GetRotation() const {
        return RadToDeg(_B2Body->GetAngle());
    }

    PhysicsWorld *PhysicsBody::GetWorld() const {
        return _World;
    }

    bool PhysicsBody::IsActive() const {
        return _B2Body->IsActive();
    }

    bool PhysicsBody::IsAwake() const {
        return _B2Body->IsAwake();
    }

    bool PhysicsBody::IsBullet() const {
        return _B2Body->IsBullet();
    }

    bool PhysicsBody::IsFixedRotation() const {
        return _B2Body->IsFixedRotation();
    }

    bool PhysicsBody::IsSleepingAllowed() const {
        return _B2Body->IsSleepingAllowed();
    }

    void PhysicsBody::ResetMassData() {
        _B2Body->ResetMassData();
    }

    void PhysicsBody::SetActive(bool flag_) {
        _B2Body->SetActive(flag_);
    }

    void PhysicsBody::SetAngularDamping(float damping_) {
        _B2Body->SetAngularDamping(damping_);
    }

    void PhysicsBody::SetAngularVelocity(float omega_) {
        _B2Body->SetAngularVelocity(DegToRad(omega_));
    }

    void PhysicsBody::SetAwake(bool flag_) {
        _B2Body->SetAwake(flag_);
    }

    void PhysicsBody::SetBullet(bool flag_) {
        _B2Body->SetBullet(flag_);
    }

    void PhysicsBody::SetFixedRotation(bool flag_) {
        _B2Body->SetFixedRotation(flag_);
    }

    void PhysicsBody::SetGravityScale(float scale_) {
        _B2Body->SetGravityScale(scale_);
    }

    void PhysicsBody::SetLinearDamping(float damping_) {
        _B2Body->SetLinearDamping(damping_);
    }

    void PhysicsBody::SetLinearVelocity(Vector2 vel_) {
        _B2Body->SetLinearVelocity({vel_.X, vel_.Y});
    }

    void PhysicsBody::SetLinearVelocityPixels(Vector2 vel_) {
        _B2Body->SetLinearVelocity({vel_.X / _World->PixelsPerMeter, vel_.Y / _World->PixelsPerMeter});
    }

    void PhysicsBody::SetPosition(Vector2 pos_) {
        _B2Body->SetTransform({
            pos_.X / _World->PixelsPerMeter,
            pos_.Y / _World->PixelsPerMeter
            }, _B2Body->GetAngle());
    }

    void PhysicsBody::SetRotation(float angle_) {
        _B2Body->SetTransform(_B2Body->GetPosition(), DegToRad(angle_));
    }

    void PhysicsBody::SetSleepingAllowed(bool flag_) {
        _B2Body->SetSleepingAllowed(flag_);
    }

    void PhysicsBody::SetType(PhysicsBody::BodyType type_) {
        _B2Body->SetType((b2BodyType)type_);
    }
}