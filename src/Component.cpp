/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#include "Component.hpp"

#include "Entity.hpp"

// TODO: SORT

namespace NerdThings::Ngine {
    Component::~Component() {
        // Fire OnDestroy
        OnDestroy();

        // Detach events
        UnsubscribeFromDraw();
        UnsubscribeFromUpdate();
    }

    void Component::Draw(Graphics::Renderer *renderer_) {}

    Game *Component::GetGame() const {
        return m_parentEntity->GetGame();
    }

    Filesystem::ResourceManager *Component::GetResourceManager() const {
        return m_parentEntity->GetResourceManager();
    }

    void Component::SubscribeToDraw() {
        m_onDrawRef = m_parentEntity->OnDraw += new ClassMethodEventHandler<Component, Graphics::Renderer *>(this, &Component::Draw);
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
