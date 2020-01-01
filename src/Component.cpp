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

#include "Component.hpp"

#include "Entity.hpp"

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

    Filesystem::ResourceManager *Component::GetResourceManager() const {
        return m_parentEntity->GetResourceManager();
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
