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

#include "BaseEntity.h"

#include "Component.h"

namespace NerdThings::Ngine {
    // Private Methods

    void BaseEntity::RemoveEntityParent(BaseEntity *ent_) {
        ent_->_ParentEntity = nullptr;
    }

    void BaseEntity::SetEntityParent(BaseEntity *ent_) {
        ent_->_ParentEntity = this;
    }

    // Public Constructor(s)

    BaseEntity::BaseEntity(Scene *parentScene_, const Vector2 position_, int depth_, bool canCull_)
        : _CanCull(canCull_), _Depth(depth_), _ParentScene(parentScene_), _Position(position_) {
        if (parentScene_ == nullptr)
            throw std::runtime_error("Cannot give an entity a null parent scene.");

        // Set initial depth
        _ParentScene->InternalSetEntityDepth(_Depth, this);
    }

    // Destructor

    BaseEntity::~BaseEntity() {
        // Destroy physics body
        if (_PhysicsBody != nullptr) _PhysicsBody->Destroy();

        // Unbind all events
        UnsubscribeFromUpdate();
    }

    // Public Methods

    bool BaseEntity::CheckForCulling(Rectangle cullArea_) {
        return !cullArea_.Contains(GetPosition());
    }

    void BaseEntity::Destroy() {
        // Detach all events
        OnDraw.Clear();
        OnTransformChanged.Clear();
        OnUpdate.Clear();

        // Detach components
        _Components.clear();

        // Unsubscribe from update
        UnsubscribeFromUpdate();

        // Remove from our parent
        GetParentContainer()->RemoveChild(this);
    }

    void BaseEntity::Draw() {
        // Trigger draw
        OnDraw();

        // Debug draw
        if (_PhysicsBody != nullptr) {
            if (_PhysicsBody->GetWorld()->DebugDraw) {
                _PhysicsBody->DebugDraw();
            }
        }
    }

    bool BaseEntity::GetCanCull() {
        return _CanCull;
    }

    std::vector<Component *> BaseEntity::GetComponents() {
        std::vector<Component*> vec;

        for (auto & _Component : _Components) {
            vec.push_back(_Component.second.get());
        }

        return vec;
    }

    int BaseEntity::GetDepth() const {
        return _Depth;
    }

    EntityContainer *BaseEntity::GetParentContainer() const {
        if (_ParentEntity != nullptr)
            return static_cast<EntityContainer *>(_ParentEntity);
        else if (_ParentScene != nullptr)
            return static_cast<EntityContainer *>(_ParentScene);
        return nullptr;
    }

    BaseEntity *BaseEntity::GetParentEntity() const {
        return _ParentEntity;
    }

    Scene *BaseEntity::GetParentScene() const {
        return _ParentScene;
    }

    bool BaseEntity::GetDoPersistentUpdates() {
        return _PersistentUpdates;
    }

    Vector2 BaseEntity::GetPosition() const {
        if (_PhysicsBody == nullptr) {
            return _Position;
        } else {
            return _PhysicsBody->GetPosition();
        }
    }

    Physics::PhysicsBody *BaseEntity::GetPhysicsBody() const {
        return _PhysicsBody;
    }

    Physics::PhysicsWorld *BaseEntity::GetPhysicsWorld() const {
        return _PhysicsBody->GetWorld();
    }

    float BaseEntity::GetRotation() const {
        if (_PhysicsBody == nullptr) {
            return _Rotation;
        } else {
            return _PhysicsBody->GetRotation();
        }
    }

    bool BaseEntity::HasComponent(const std::string &name_) {
        return _Components.find(name_) != _Components.end();
    }

    bool BaseEntity::IsPhysicsEntity() {
        return _PhysicsBody != nullptr;
    }

    void BaseEntity::MoveBy(const Vector2 moveBy_) {
        if (_PhysicsBody == nullptr) {
            _Position += moveBy_;
        } else {
            auto p = _PhysicsBody->GetPosition();
            _PhysicsBody->SetPosition(p + moveBy_);
        }

        // Fire event
        OnTransformChanged({GetPosition(), GetRotation()});
    }

    bool BaseEntity::RemoveComponent(const std::string &name_) {
        const auto comp = GetComponent<Component>(name_);

        if (comp != nullptr) {
            // Remove component from map
            _Components.erase(name_);

            return true;
        }

        // We don't have this component
        return false;
    }

    void BaseEntity::SetCanCull(bool canCull_) {
        _CanCull = canCull_;
    }

    void BaseEntity::SetDepth(int depth_) {
        _ParentScene->InternalUpdateEntityDepth(_Depth, depth_, this);
        _Depth = depth_;
    }

    void BaseEntity::SetDoPersistentUpdates(bool persistentUpdates_) {
        if (_OnUpdateRef.IsAttached())
            throw std::runtime_error("This property cannot be changed once update has been subscribed.");
        _PersistentUpdates = persistentUpdates_;
    }

    void BaseEntity::SetPosition(const Vector2 position_) {
        if (_PhysicsBody == nullptr) {
            _Position = position_;
        } else {
            _PhysicsBody->SetPosition(position_);
        }

        // Fire event
        OnTransformChanged({GetPosition(), GetRotation()});
    }

    void BaseEntity::SetRotation(float rotation_) {
        if (_PhysicsBody == nullptr) {
            // Set our rotation
            _Rotation = rotation_;
        } else {
            // Set the body rotation
            _PhysicsBody->SetRotation(rotation_);
        }

        // Fire event
        OnTransformChanged({GetPosition(), GetRotation()});
    }

    void BaseEntity::SetPhysicsBody(Physics::PhysicsBody *body_) {
        auto physicsWorld = GetParentScene()->GetPhysicsWorld();
        if (physicsWorld == nullptr) throw std::runtime_error("Cannot make a physics entity in non-physics scene.");
        if (_PhysicsBody != nullptr) _PhysicsBody->Destroy(); // Destroy current fixture;
        _PhysicsBody = body_;
    }

    bool BaseEntity::SubscribeToUpdate() {
        if (_ParentScene != nullptr) {
            if (!_OnUpdateRef.IsAttached()) {
                if (_PersistentUpdates) _OnUpdateRef = _ParentScene->OnPersistentUpdate += new ClassMethodEventHandler<BaseEntity>(this, &BaseEntity::Update);
                else _OnUpdateRef = _ParentScene->OnUpdate += new ClassMethodEventHandler<BaseEntity>(this, &BaseEntity::Update);
                return true;
            } else {
                // We still have an event, soooo...
                return true;
            }
        }
        return false;
    }

    void BaseEntity::UnsubscribeFromUpdate() {
        _OnUpdateRef.Detach();
    }

    void BaseEntity::Update() {
        // Trigger update
        OnUpdate();

        if (_PhysicsBody != nullptr) {
            // As we are a physics entity, fire this each frame
            OnTransformChanged({GetPosition(), GetRotation()});
        }
    }
}
