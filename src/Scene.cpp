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

#include "Scene.hpp"

#include "Console.hpp"
#include "Entity.hpp"
#include "Game.hpp"

// TODO: I had an idea, maybe we could put an "AudioEnvironment" in the scene...
//  This would be for things like directional sound etc.

namespace ngine {
    void Scene::_sortEntities() {
        std::sort(m_entities.begin(), m_entities.end(), &Entity::_SortChildrenPredicate);
    }

    Scene::Scene(Game *game, bool physicsEnabled)
            : m_game(game) {
        // Add default camera
        createCamera("default");
        setCurrentCamera("default");

        // Create physics space if enabled
        if (physicsEnabled) {
            m_space = new physics::PhysicsSpace();
        }
    }

    Scene::~Scene() {
        // Delete space if enabled
        if (m_space != nullptr) {
            delete m_space;
            m_space = nullptr;
        }

        // Delete entities
        std::vector<Entity *> ents;
        ents = m_entities;

        for (auto e : ents) {
            delete e;
        }
    }

    bool Scene::isPhysicsEnabled() const {
        return m_space != nullptr;
    }

    physics::PhysicsSpace *Scene::getSpace() {
        return m_space;
    }

    const physics::PhysicsSpace *Scene::getSpace() const {
        return m_space;
    }

    Game *Scene::getGame() const {
        return m_game;
    }

    filesystem::ResourceManager *Scene::getResourceManager() {
        return m_game->getResourceManager();
    }

    graphics::Camera *Scene::createCamera(const std::string &name) {
        if (m_cameras.find(name) != m_cameras.end())
            return &m_cameras[name];
        m_cameras.insert({name, {}});
        return &m_cameras[name];
    }

    graphics::Camera *Scene::getCamera(const std::string &name) {
        if (m_cameras.find(name) != m_cameras.end())
            return &m_cameras[name];
        return nullptr;
    }

    void Scene::removeCamera(const std::string &name) {
        m_cameras.erase(name);
    }

    void Scene::setCurrentCamera(const std::string &name) {
        if (m_cameras.find(name) != m_cameras.end())
            m_currentCamera = name;
        else Console::Fail("Scene", "Camera does not exist.");
    }

    void Scene::addChild(Entity *entity) {
        // Ensure the child has no parent
        if (entity->m_parent != nullptr)
            Console::Fail("Scene", "Entity already has a parent elsewhere.");

        // Check the child is not a member of a scene
        if (entity->m_scene != nullptr)
            Console::Fail("Scene", "Entity is already a member of a scene.");

        // Add to our list.
        m_entities.push_back(entity);

        // Fire event
        entity->initialize(this);
    }

    void Scene::removeChild(Entity *entity) {
        // Check we actually own the entity.
        if (std::find(m_entities.begin(), m_entities.end(), entity) == m_entities.end())
            Console::Fail("Scene", "This entity is not parented by this scene.");

        // Remove
        m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());

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

    Entity *Scene::getChildByName(const std::string &name) {
        auto nHash = std::hash<std::string>{}(name);
        for (const auto &e : m_entities) {
            if (e->m_nameHash == nHash && e->m_name == name)
                return e;
        }
        return nullptr;
    }

    std::vector<Entity *> Scene::getChildren() {
        return m_entities;
    }

    std::vector<Entity *> Scene::getChildrenByName(const std::string &name) {
        std::vector<Entity *> ents;
        auto nHash = std::hash<std::string>{}(name);
        for (const auto &e : m_entities) {
            if (e->m_nameHash == nHash && e->m_name == name)
                ents.push_back(e);
        }
        return ents;
    }

    void Scene::preRender(graphics::Renderer *renderer) {}

    void Scene::onRender(graphics::Renderer *renderer) {
        // Default render actons.
        preRender(renderer);
        render(renderer);
        postRender(renderer);
        renderUI(renderer);
    }

    void Scene::postRender(graphics::Renderer *renderer) {}

    void Scene::render(graphics::Renderer *renderer) {
        renderWith(renderer, m_currentCamera);
    }

    void Scene::renderWith(graphics::Renderer *renderer, const std::string &camera) {
        // Exit if empty
        if (m_entities.empty())
            return;

        // Sort children
        _sortEntities();

        // Get the current camera
        auto cam = getCamera(camera);
        if (cam == nullptr)
            Console::Fail("Scene", "Failed to get camera.");

        // Get camera translation
        auto view = cam->getViewMatrix();

        // Render all entities
        for (const auto &e : m_entities) e->render(renderer, view, cam);
    }

    void Scene::renderUI(graphics::Renderer *renderer) {
        // TODO: A UI system.
    }

    void Scene::update() {
        // Update entities
        for (const auto &e : m_entities) {
            if (!e->m_asleep)
                e->update();
        }

        // Physics step
        if (isPhysicsEnabled()) {
            auto timestep = 1/getGame()->getTargetFPS();
            m_space->step(timestep);
        }
    }

    void Scene::updateUI() {
        // TODO: A UI system.
    }
}