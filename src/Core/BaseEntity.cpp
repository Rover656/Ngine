/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*   
*   File reviewed on 01/06/2019 by R.M
*
**********************************************************************************************/

#include "BaseEntity.h"

#include "Component.h"

namespace NerdThings::Ngine::Core {
    // Private Methods

    void BaseEntity::RemoveEntityParent(BaseEntity *ent_) {
        ent_->_ParentEntity = nullptr;
    }

    void BaseEntity::SetEntityParent(BaseEntity *ent_) {
        ent_->_ParentEntity = this;
    }

    // Public Constructor(s)

    BaseEntity::BaseEntity(Scene *parentScene_, const Math::TVector2 position_, int depth_, bool canCull_)
        : _CanCull(canCull_), _Depth(depth_), _ParentScene(parentScene_), _Position(position_) {
        if (parentScene_ == nullptr)
            throw std::runtime_error("Cannot give an entity a null parent scene.");

        // Set initial depth
        _ParentScene->InternalSetEntityDepth(_Depth, this);
    }

    // Destructor

    BaseEntity::~BaseEntity() {
        // Delete all components
        ConsoleMessage("Deleting components and removing update subscription.", "NOTICE", "BASEENTITY");
        for (auto comp : _Components) {
            delete comp.second;
        }

        // Unbind all events
        UnsubscribeFromUpdate();
    }

    // Public Methods

    bool BaseEntity::CheckForCulling(Math::TRectangle cullArea_) {
        return cullArea_.Contains(GetPosition());
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

        for (auto it = _Components.begin(); it != _Components.end(); ++it) {
            vec.push_back(it->second);
        }

        return vec;
    }

    int BaseEntity::GetDepth() const {
        return _Depth;
    }

    Math::TVector2 BaseEntity::GetOrigin() const {
        return _Origin;
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

    Math::TVector2 BaseEntity::GetPosition() const {
        return _Position;
    }

    float BaseEntity::GetRotation() const {
        return _Rotation;
    }

    // float BaseEntity::GetScale() const {
    //     return _Scale;
    // }

    bool BaseEntity::HasComponent(const std::string &name_) {
        return _Components.find(name_) != _Components.end();
    }

    void BaseEntity::MoveBy(const Math::TVector2 moveBy_) {
        _Position += moveBy_;
        OnTransformChanged({_Origin, _Position, _Rotation, 1});
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

    void BaseEntity::SetDepth(int depth_) {
        _ParentScene->InternalUpdateEntityDepth(_Depth, depth_, this);
        _Depth = depth_;
    }

    void BaseEntity::SetOrigin(Math::TVector2 origin_) {
        _Origin = origin_;
        OnTransformChanged({_Origin, _Position, _Rotation, 1});
    }

    void BaseEntity::SetDoPersistentUpdates(bool persistentUpdates_) {
        if (_OnUpdateRef.ID > 0)
            throw std::runtime_error("This property cannot be changed once update has been subscribed.");
        _PersistentUpdates = persistentUpdates_;
    }

    void BaseEntity::SetPosition(const Math::TVector2 position_) {
        _Position = position_;
        OnTransformChanged({_Origin, _Position, _Rotation, 1});
    }

    void BaseEntity::SetRotation(float rotation_) {
        _Rotation = rotation_;
        OnTransformChanged({_Origin, _Position, _Rotation, 1});
    }

    // void BaseEntity::SetScale(float scale_) {
    //     _Scale = scale_;
    //     OnTransformChanged({ _Origin, _Position, _Rotation, _Scale });
    // }

    bool BaseEntity::SubscribeToUpdate() {
        if (_ParentScene != nullptr) {
            if (_OnUpdateRef.ID < 0) {
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
        OnUpdate({});
    }
}
