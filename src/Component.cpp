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

#include "Component.h"

#include "BaseEntity.h"

namespace NerdThings::Ngine {
    // Destructor

    Component::~Component() {
        UnsubscribeFromDraw();
        UnsubscribeFromUpdate();
    }

    // Public Methods

    void Component::Draw() { }

    Scene *Component::GetScene() const {
        return m_parentEntity->GetScene();
    }

    Game *Component::GetGame() const {
        return m_parentEntity->GetGame();
    }

    bool Component::HasParent() const {
        return m_parentEntity != nullptr;
    }

    void Component::SubscribeToDraw() {
        if (HasParent()) {
            m_onDrawRef = m_parentEntity->OnDraw += new ClassMethodEventHandler<Component>(this, &Component::Draw);
        }
    }

    void Component::SubscribeToUpdate() {
        if (HasParent()) {
            // Check the entity subscribed to update
            // If not, subscribe
            if (m_parentEntity->SubscribeToUpdate()) {
                m_onUpdateRef = m_parentEntity->OnUpdate += new ClassMethodEventHandler<Component>(this, &Component::Update);
            }
        }
    }

    void Component::UnsubscribeFromDraw() {
        m_onDrawRef.Detach();
    }

    void Component::UnsubscribeFromUpdate() {
        m_onUpdateRef.Detach();
    }

    void Component::Update() { }

    // Protected Constructor(s)

    Component::Component(BaseEntity *parent_)
            : m_parentEntity(parent_) {
        // Check our parent is valid
        if (parent_ == nullptr) {
            throw std::runtime_error("A valid parent must be attached to this component.");
        }
    }
}
