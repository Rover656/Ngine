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
        GetParentContainer()->RemoveEntity(this);
    }

    void BaseEntity::Draw() {
        // Trigger draw
        OnDraw({});
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

    Vector2 BaseEntity::GetOrigin() const {
        return _Origin;
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
        return _Position;
    }

    float BaseEntity::GetRotation() const {
        return _Rotation;
    }

     float BaseEntity::GetScale() const {
         return _Scale;
     }

    bool BaseEntity::HasComponent(const std::string &name_) {
        return _Components.find(name_) != _Components.end();
    }

    void BaseEntity::MoveBy(const Vector2 moveBy_) {
        _Position += moveBy_;
        OnTransformChanged({_Origin, _Position, _Rotation, _Scale});
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

    void BaseEntity::SetOrigin(Vector2 origin_) {
        _Origin = origin_;
        OnTransformChanged({_Origin, _Position, _Rotation, _Scale});
    }

    void BaseEntity::SetDoPersistentUpdates(bool persistentUpdates_) {
        if (_OnUpdateRef.IsValid())
            throw std::runtime_error("This property cannot be changed once update has been subscribed.");
        _PersistentUpdates = persistentUpdates_;
    }

    void BaseEntity::SetPosition(const Vector2 position_) {
        _Position = position_;
        OnTransformChanged({_Origin, _Position, _Rotation, _Scale});
    }

    void BaseEntity::SetRotation(float rotation_) {
        _Rotation = rotation_;
        OnTransformChanged({_Origin, _Position, _Rotation, _Scale});
    }

     void BaseEntity::SetScale(float scale_) {
         _Scale = scale_;
         OnTransformChanged({ _Origin, _Position, _Rotation, _Scale });
     }

    bool BaseEntity::SubscribeToUpdate() {
        if (_ParentScene != nullptr) {
            if (!_OnUpdateRef.IsValid()) {
                if (_PersistentUpdates) _OnUpdateRef = _ParentScene->OnPersistentUpdate.Bind<BaseEntity>(this, &BaseEntity::Update);
                else _OnUpdateRef = _ParentScene->OnUpdate.Bind<BaseEntity>(this, &BaseEntity::Update);
                return true;
            } else {
                // We still have an event, soooo...
                return true;
            }
        }
        return false;
    }

    void BaseEntity::UnsubscribeFromUpdate() {
        _OnUpdateRef.UnBind();
    }

    void BaseEntity::Update(EventArgs &e) {
        // Trigger update
        OnUpdate();
    }
}
