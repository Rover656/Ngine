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

#include "Component.h"

#include "BaseEntity.h"

namespace NerdThings::Ngine {
    // Public Methods

    void Component::Draw(EventArgs &e) { }

    bool Component::HasParent() const {
        return _ParentEntity != nullptr;
    }

    void Component::SubscribeToDraw() {
        if (HasParent()) {
            _OnDrawRef = _ParentEntity->OnDraw.Bind(this, &Component::Draw);
        }
    }

    void Component::SubscribeToUpdate() {
        if (HasParent()) {
            // Check the entity subscribed to update
            // If not, subscribe
            if (_ParentEntity->SubscribeToUpdate()) {
                _OnUpdateRef = _ParentEntity->OnUpdate.Bind(this, &Component::Update);
            }
        }
    }

    void Component::UnsubscribeFromDraw() {
        _OnDrawRef.UnBind();
    }

    void Component::UnsubscribeFromUpdate() {
        _OnUpdateRef.UnBind();
    }

    void Component::Update(EventArgs &e) { }

    // Protected Constructor(s)

    Component::Component(BaseEntity *parent_)
        : _ParentEntity(parent_) {
        // Check our parent is valid
        if (parent_ == nullptr) {
            throw std::runtime_error("A valid parent must be attached to this component.");
        }
    }
}
