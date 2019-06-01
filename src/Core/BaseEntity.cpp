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

#include "Component.h"          // Required for: Component

namespace NerdThings::Ngine::Core {
    // Private Methods

    void BaseEntity::RemoveEntityParent(BaseEntity *ent_) {
        ent_->_ParentEntity = nullptr;
    }

    void BaseEntity::SetEntityParent(BaseEntity *ent_) {
        ent_->_ParentEntity = this;
    }

    // Public Constructor(s)

    BaseEntity::BaseEntity(Scene *parentScene_, const TVector2 position_)
        : _Position(position_), _ParentScene(parentScene_) {
        if (parentScene_ == nullptr)
            throw std::runtime_error("Cannot give an entity a null parent scene.");
    }

    // Destructor

    BaseEntity::~BaseEntity() {
        // Delete all components
        for (auto comp : _Components) {
            delete comp.second;
        }

        // Unbind all events
        UnsubscribeFromCameraDraw();
        UnsubscribeFromDraw();
        UnsubscribeFromUpdate();
    }

    // Public Methods

    void BaseEntity::Draw(EventArgs &e) {
        // Trigger draw
        OnDraw({});
    }

    void BaseEntity::DrawCamera(EventArgs &e) {
        // Trigger drawcamera
        OnDrawCamera({});
    }

    std::vector<Component *> BaseEntity::GetComponents() {
        std::vector<Component*> vec;

        for (auto it = _Components.begin(); it != _Components.end(); ++it) {
            vec.push_back(it->second);
        }

        return vec;
    }

    TVector2 BaseEntity::GetOrigin() const {
        return _Origin;
    }

    BaseEntity *BaseEntity::GetParentEntity() const {
        return _ParentEntity;
    }

    Scene *BaseEntity::GetParentScene() const {
        return _ParentScene;
    }

    TVector2 BaseEntity::GetPosition() const {
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

    void BaseEntity::MoveBy(const TVector2 moveBy_) {
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

    void BaseEntity::SetOrigin(TVector2 origin_) {
        _Origin = origin_;
        OnTransformChanged({_Origin, _Position, _Rotation, 1});
    }

    void BaseEntity::SetPosition(const TVector2 position_) {
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

    bool BaseEntity::SubscribeToCameraDraw() {
        if (_ParentScene != nullptr) {
            if (_OnDrawCameraRef.ID < 0) {
                _OnDrawCameraRef = _ParentScene->OnDrawCamera.Bind<BaseEntity>(this, &BaseEntity::DrawCamera);
                return true;
            } else {
                // We still have an event, soooo...
                return true;
            }
        }
        return false;
    }

    bool BaseEntity::SubscribeToDraw() {
        if (_ParentScene != nullptr) {
            if (_OnDrawRef.ID < 0) {
                _OnDrawRef = _ParentScene->OnDraw.Bind<BaseEntity>(this, &BaseEntity::Draw);
                return true;
            } else {
                // We still have an event, soooo...
                return true;
            }
        }
        return false;
    }

    bool BaseEntity::SubscribeToUpdate() {
        if (_ParentScene != nullptr) {
            if (_OnUpdateRef.ID < 0) {
                _OnUpdateRef = _ParentScene->OnUpdate.Bind<BaseEntity>(this, &BaseEntity::Update);
                return true;
            } else {
                // We still have an event, soooo...
                return true;
            }
        }
        return false;
    }

    void BaseEntity::UnsubscribeFromCameraDraw() {
        _OnDrawCameraRef.UnBind();
    }

    void BaseEntity::UnsubscribeFromDraw() {
        _OnDrawRef.UnBind();
    }

    void BaseEntity::UnsubscribeFromUpdate() {
        _OnUpdateRef.UnBind();
    }

    void BaseEntity::Update(EventArgs &e) {
        // Trigger update
        OnUpdate({});
    }
}
