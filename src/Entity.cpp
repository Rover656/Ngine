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

#include "Entity.hpp"

#include "Component.hpp"
#include "Console.hpp"
#include "Entity.hpp"
#include "Game.hpp"

namespace ngine {
    bool Entity::_SortChildrenPredicate(Entity *a, Entity *b) {
        return a->m_zIndex < b->m_zIndex;
    }

    void Entity::_sortChildren() {
        // Sort
        std::sort(m_children.begin(), m_children.end(), &_SortChildrenPredicate);
    }

    void Entity::_updateModelView() {
        // We use relative stuff as modelview will already consist of parent's input.
        auto pos = getPosition();
        auto rot = getRotation();
        //auto ori = getOrigin();

        // Build modelview
        m_modelView = Matrix::Translate({pos.X, pos.Y, 0})
                      * Matrix::Scale(m_scale.X, m_scale.Y, 1)
                      * Matrix::RotateZ(rot);

        // Mark as clean
        m_modelViewDirty = false;
    }

    Entity::Entity() = default;

    Entity::~Entity() {
        // Mark as destroyed (so no infinite recursion happens)
        m_destroyed = true;

        // Get list of children to destroy
        std::vector<Entity *> ents;
        ents = m_children;

        // Destroy children first
        for (auto e : ents) {
            delete e;
        }

        // Get list of components to destroy
        std::vector<Component *> comps;
        for (const auto &c : m_components) {
            comps.push_back(c.second);
        }

        // Destroy components
        for (auto c : comps) {
            delete c;
        }

        // Destroy physics body
        delete m_physicsBody;

        // Remove from our parent
        if (m_parent != nullptr)
            m_parent->removeChild(this);
        else if (m_scene != nullptr)
            m_scene->removeChild(this);
    }

    void Entity::initialize(Scene *scene) {
        // Check we aren't initialized already
        if (m_initialized)
            Console::Fail("Entity", "Do not call initialize ever, unless you have to!");

        // Mark as initialized
        m_initialized = true;

        // Save scene
        m_scene = scene;

        // Pass on to children
        for (const auto &e : m_children) {
            e->initialize(scene);
        }
    }

    void Entity::cleanup() {
        if (!m_initialized)
            Console::Fail("Entity", "Cannot cleanup before initialization");
        m_initialized = false;
    }

    void Entity::destroy() {
        delete this;
    }

    Entity *Entity::getParent() const {
        return m_parent;
    }

    Scene *Entity::getScene() const {
        return m_scene;
    }

    Game *Entity::getGame() const {
        return m_scene->getGame();
    }

    filesystem::ResourceManager *Entity::getResourceManager() const {
        return m_scene->getResourceManager();
    }

    Vector2 Entity::getPosition() const {
        if (m_physicsBody != nullptr) {
            if (m_parent != nullptr) {
                // Must fix by removing parent's position
                return m_physicsBody->getPosition() - m_parent->getPosition();
            }
            return m_physicsBody->getPosition();
        }
        return m_position;
    }

    void Entity::setPosition(Vector2 position) {
        if (m_physicsBody != nullptr) {
            if (m_parent != nullptr) {
                // Must fix by accounting for parent's position
                m_physicsBody->setPosition(position + m_parent->getPosition());
            } else {
                m_physicsBody->setPosition(position);
            }
        } else {
            m_position = position;
            m_modelViewDirty = true;
        }
    }

    Angle Entity::getRotation() const {
        if (m_physicsBody != nullptr) {
            return m_physicsBody->getRotation();
        }
        return m_rotation;
    }

    void Entity::setRotation(Angle rotation) {
        if (m_physicsBody != nullptr) {
            m_physicsBody->setRotation(rotation);
        } else {
            m_rotation = rotation;
            m_modelViewDirty = true;
        }
    }

    Vector2 Entity::getScale() const {
        if (m_physicsBody != nullptr) {
            return {1, 1}; // TEMP
        }
        return m_scale;
    }

    void Entity::setScale(Vector2 scale) {
        if (m_physicsBody != nullptr) {
            Console::Warn("Entity", "Scale was not set as scaling physics entities is not implemented.");
        } else {
            m_scale = scale;
            m_modelViewDirty = true;
        }
    }

    physics::PhysicsBody *Entity::getPhysicsBody() {
        return m_physicsBody;
    }

    void Entity::setPhysicsBody(physics::PhysicsBody *body) {
        // TODO Passing in coordinates etc.
        m_physicsBody = body;
    }

    int Entity::getZIndex() const {
        return m_zIndex;
    }

    void Entity::setZIndex(int zIndex) {
        m_zIndex = zIndex;
    }

    void Entity::addComponent(const std::string &name, Component *component) {
        // Check we are initialized
        if (!m_initialized)
            Console::Fail("Entity",
                          "Cannot add components until initialized, use the initialize method to add components!");

        // Check that the component has no parent
        if (component->m_parent != nullptr)
            Console::Fail("Entity", "Component already has a parent.");

        // See if we already have this component
        for (const auto &c : m_components) {
            if (c.second == component)
                Console::Fail("Entity", "Component is already a child of this entity.");
            if (c.first == name)
                Console::Fail("Entity", "Component name has been used.");
        }

        // Add
        m_components.insert({name, component});

        // Setup
        component->initialize(this);
    }

    void Entity::removeComponent(const std::string &name) {
        if (m_components.find(name) == m_components.end())
            Console::Fail("Entity", "Cannot find component.");

        // Get component pointer
        auto comp = m_components[name];

        // Remove from list
        m_components.erase(name);
        comp->m_parent = nullptr;

        // Run cleanup if initialized
        if (comp->m_initialized)
            comp->cleanup();

        // Delete component
        if (!comp->m_destroyed)
            delete comp;
    }

    void Entity::removeComponent(Component *component) {
        // Find component and remove.
        for (const auto &c : m_components) {
            if (c.second == component) {
                removeComponent(c.first);
                return;
            }
        }

        Console::Fail("Entity", "Cannot find component.");
    }

    Component *Entity::getComponent(const std::string &name) {
        if (m_components.find(name) != m_components.end())
            return m_components[name];
        return nullptr;
    }

    bool Entity::isAsleep() {
        return m_asleep;
    }

    void Entity::setIsAsleep(bool asleep) {
        m_asleep = asleep;
    }

    bool Entity::isVisible() {
        return m_visible;
    }

    void Entity::setIsVisible(bool visible) {
        m_visible = visible;
    }

    std::string Entity::getName() {
        return m_name;
    }

    void Entity::setName(const std::string &name) {
        m_name = name;
        m_nameHash = std::hash<std::string>{}(name);
    }

    void Entity::addChild(Entity *entity) {
        // Ensure the child has no parent
        if (entity->m_parent != nullptr)
            Console::Fail("Entity", "Entity already has a parent!");

        // Check the child is not a member of a scene
        if (entity->m_scene != nullptr)
            Console::Fail("Entity", "Entity is already a member of a scene.");

        // Ensure the child is not us
        if (entity == this)
            Console::Fail("Entity", "Cannot add an entity to itself!");

        // Add to map
        m_children.push_back(entity);

        // Set parent.
        entity->m_parent = this;

        // Initialize entity if we are already initialized
        // If we aren't, this will be done once we ourselves are ready.
        if (m_initialized)
            entity->initialize(m_scene);
    }

    void Entity::removeChild(Entity *entity) {
        // Check we own this child.
        if (entity->m_parent != this)
            Console::Fail("Entity", "Entity is not parented by me!");

        // Remove
        m_children.erase(std::remove(m_children.begin(), m_children.end(), entity), m_children.end());

        // If the entity is still initialized, cleanup now
        if (entity->m_initialized)
            entity->cleanup();

        // Unset parents
        entity->m_parent = nullptr;
        entity->m_scene = nullptr;

        // If the entity isn't destroyed yet, delete it
        if (!entity->m_destroyed)
            delete entity;
    }

    Entity *Entity::getChildByName(const std::string &name) {
        auto nHash = std::hash<std::string>{}(name);
        for (const auto &e : m_children) {
            if (e->m_nameHash == nHash && e->m_name == name)
                return e;
        }
        return nullptr;
    }

    std::vector<Entity *> Entity::getChildren() {
        return m_children;
    }

    std::vector<Entity *> Entity::getChildrenByName(const std::string &name) {
        std::vector<Entity *> ents;
        auto nHash = std::hash<std::string>{}(name);
        for (const auto &e : m_children) {
            if (e->m_nameHash == nHash && e->m_name == name)
                ents.push_back(e);
        }
        return ents;
    }

    bool Entity::isVisibleInCamera(graphics::Camera *camera) {
        // TODO: Implement.
        return true;
    }

    void Entity::render(graphics::Renderer *renderer, Matrix modelView, graphics::Camera *currentCamera) {
        // Skip if not visible.
        if (!m_visible)
            return;

        // Test if we are visible
        bool visibleInCamera = true;
        if (currentCamera != nullptr)
            visibleInCamera = isVisibleInCamera(currentCamera);

        // TODO: Camera flags??

        // Update model view if dirty
        if (m_modelViewDirty || m_physicsBody != nullptr)
            _updateModelView();

        // Mix modelView with our own
        modelView = modelView * m_modelView;

        if (!m_children.empty()) {
            // Sort children
            _sortChildren();

            int i = 0;
            for (; i < m_children.size() && m_children[i]->m_zIndex < 0; i++)
                m_children[i]->render(renderer, modelView, currentCamera);

            // Render self
            if (visibleInCamera) {
                renderer->setModelViewMatrix(modelView);
                draw(renderer, modelView);
                renderer->setModelViewMatrix(Matrix::Identity);
            }

            // Render children
            for (; i < m_children.size(); i++)
                m_children[i]->render(renderer, modelView, currentCamera);
        } else if (visibleInCamera) {
            // Render self.
            renderer->setModelViewMatrix(modelView);
            draw(renderer, modelView);
            renderer->setModelViewMatrix(Matrix::Identity);
        }
    }

    void Entity::draw(graphics::Renderer *renderer, Matrix modelView) {
        for (const auto &c : m_components) {
            c.second->draw(renderer);
        }
    }

    void Entity::update() {
        // Update components
        for (const auto &c : m_components) {
            c.second->update();
        }

        // Update children
        for (const auto &e : m_children) {
            if (!e->m_asleep)
                e->update();
        }
    }
}