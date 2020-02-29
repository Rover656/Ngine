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

#include "ngine/Component.hpp"

#include "ngine/Console.hpp"
#include "ngine/Entity.hpp"

namespace ngine {
    Component::Component() = default;

    Component::~Component() {
        // Mark as deleted so no recursion occurs
        m_destroyed = true;

        // Remove from parent if we still have one.
        if (m_parent != nullptr)
            m_parent->removeComponent(this);
    }

    void Component::initialize(Entity *parent) {
        // Check we aren't already initialized.
        if (m_initialized)
            Console::fail("Component", "Do not call initialize ever, unless you have to!");

        // Mark as initialized
        m_initialized = true;

        // Save parent
        m_parent = parent;
    }

    void Component::cleanup() {
        // Mark as not initialized
        m_initialized = false;
    }

    Entity *Component::getParent() const {
        if (!m_initialized)
            Console::fail("Component", "Component must be initialized before being used.");
        return m_parent;
    }

    Scene *Component::getScene() const {
        if (!m_initialized)
            Console::fail("Component", "Component must be initialized before being used.");
        return m_parent->getScene();
    }

    Game *Component::getGame() const {
        if (!m_initialized)
            Console::fail("Component", "Component must be initialized before being used.");
        return m_parent->getGame();
    }

    input::Keyboard *Component::getKeyboard() const {
        if (!m_initialized)
            Console::fail("Component", "Component must be initialized before being used.");
        return m_parent->getKeyboard();
    }

    input::Mouse *Component::getMouse() const {
        if (!m_initialized)
            Console::fail("Component", "Component must be initialized before being used.");
        return m_parent->getMouse();
    }

    filesystem::ResourceManager *Component::getResourceManager() const {
        if (!m_initialized)
            Console::fail("Component", "Component must be initialized before being used.");
        return m_parent->getResourceManager();
    }

    void Component::draw(graphics::Renderer *renderer) {}

    void Component::update() {}
}