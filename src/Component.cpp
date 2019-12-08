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

#include "Entity.h"

namespace NerdThings::Ngine {
    Component::~Component() {
        // Fire OnDestroy
        OnDestroy();

        // Detach events
        UnsubscribeFromDraw();
        UnsubscribeFromUpdate();
    }

    void Component::Draw() {}

    Game *Component::GetGame() const {
        return m_parentEntity->GetGame();
    }

    void Component::SubscribeToDraw() {
        m_onDrawRef = m_parentEntity->OnDraw += new ClassMethodEventHandler<Component>(this, &Component::Draw);
    }

    bool Component::SubscribedToDraw() const {
        return m_onDrawRef.IsAttached();
    }

    void Component::UnsubscribeFromDraw() {
        m_onDrawRef.Detach();
    }

    void Component::SubscribeToUpdate() {
        // Ensure the entity is subscribed to updates.
        if (m_parentEntity->SubscribeToUpdate()) {
            m_onUpdateRef = m_parentEntity->OnUpdate += new ClassMethodEventHandler<Component>(this, &Component::Update);
        }
    }

    bool Component::SubscribedToUpdate() const {
        return m_onUpdateRef.IsAttached();
    }

    void Component::UnsubscribeFromUpdate() {
        m_onUpdateRef.Detach();
    }

    void Component::Update() { }

    Component::Component(Entity *parent_)
            : m_parentEntity(parent_) {
        // Check our parent is valid
        if (parent_ == nullptr) {
            throw std::runtime_error("A valid parent must be attached to this component.");
        }
    }
}
