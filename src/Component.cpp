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

namespace ngine {
    Component::Component(Entity *parent)
            : m_parentEntity(parent) {
        // Check our parent is valid
        if (parent == nullptr) {
            throw std::runtime_error("A valid parent must be attached to this component.");
        }
    }

    Component::~Component() {
        // Fire OnDestroy
        OnDestroy();

        // Detach events
        unsubscribeFromUpdate();
    }

    Game *Component::getGame() const {
        return m_parentEntity->getGame();
    }

    filesystem::ResourceManager *Component::getResourceManager() const {
        return m_parentEntity->getResourceManager();
    }

    void Component::subscribeToUpdate() {
        // Ensure the entity is subscribed to updates.
        if (m_parentEntity->subscribeToUpdate()) {
            m_onUpdateRef = m_parentEntity->OnUpdate += new ClassMethodEventHandler<Component>(this, &Component::update);
        }
    }

    bool Component::subscribedToUpdate() const {
        return m_onUpdateRef.isAttached();
    }

    void Component::unsubscribeFromUpdate() {
        m_onUpdateRef.detach();
    }

    void Component::draw(graphics::Renderer *renderer) {}

    void Component::update() {}
}
