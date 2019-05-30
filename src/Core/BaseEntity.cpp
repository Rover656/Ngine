/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "BaseEntity.h"

#include "Component.h"          // Required for: Component

namespace NerdThings::Ngine::Core {
    // Public Constructor(s)

    BaseEntity::BaseEntity(Scene *parentScene_, const TVector2 position_)
        : _Position(position_), ParentScene(parentScene_) {}

    // Destructor

    BaseEntity::~BaseEntity() {
        // Delete all components
        for (auto comp : _Components) {
            delete comp.second;
        }
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

    TVector2 BaseEntity::GetPosition() const {
        return _Position;
    }

    bool BaseEntity::HasComponent(const std::string &name_) {
        return _Components.find(name_) != _Components.end();
    }

    void BaseEntity::MoveBy(const TVector2 moveBy_) {
        _Position += moveBy_;
        OnPositionChanged({_Position});
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

    void BaseEntity::SetPosition(const TVector2 position) {
        _Position = position;
        OnPositionChanged({_Position});
    }

    bool BaseEntity::SubscribeToCameraDraw() {
        if (ParentScene != nullptr) {
            if (_OnDrawCameraRef.ID < 0) {
                _OnDrawCameraRef = ParentScene->OnDrawCamera.Bind<BaseEntity>(this, &BaseEntity::DrawCamera);
                return true;
            } else {
                // We still have an event, soooo...
                return true;
            }
        }
        return false;
    }

    bool BaseEntity::SubscribeToDraw() {
        if (ParentScene != nullptr) {
            if (_OnDrawRef.ID < 0) {
                _OnDrawRef = ParentScene->OnDraw.Bind<BaseEntity>(this, &BaseEntity::Draw);
                return true;
            } else {
                // We still have an event, soooo...
                return true;
            }
        }
        return false;
    }

    bool BaseEntity::SubscribeToUpdate() {
        if (ParentScene != nullptr) {
            if (_OnUpdateRef.ID < 0) {
                _OnUpdateRef = ParentScene->OnUpdate.Bind<BaseEntity>(this, &BaseEntity::Update);
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
