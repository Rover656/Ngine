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

#include "Entity.hpp"
#include "Game.hpp"
#include "Console.hpp"

namespace ngine {
    void Scene::_addEntity(Entity *ent) {
        // When an entity is added, mark as active
        m_entityActivities.insert({ent, true});

        // Add entity and mark it's depth
        if (m_entityDepths.find(ent->m_depth) == m_entityDepths.end())
            m_entityDepths.insert({ent->m_depth, {}});
        m_entityDepths[ent->m_depth].push_back(ent);

        // Fire scene add event
        ent->OnInit();
    }

    void Scene::_removeEntity(Entity *ent) {
        for (auto vec : m_entityDepths) {
            for (auto ent : vec.second) {
                if (ent == ent) {
                    m_entityDepths[vec.first].erase(
                            std::remove(m_entityDepths[vec.first].begin(), m_entityDepths[vec.first].end(), ent),
                            m_entityDepths[vec.first].end());
                }
            }
        }
    }

    void Scene::_updateEntityDepth(int newDepth, Entity *ent) {
        if (ent->m_depth == newDepth)
            return; // Short circuit if depth's are the same because we don't want to remove and re-add
        m_entityDepths[ent->m_depth].erase(
                std::remove(m_entityDepths[ent->m_depth].begin(), m_entityDepths[ent->m_depth].end(), ent),
                m_entityDepths[ent->m_depth].end());

        if (m_entityDepths.find(newDepth) == m_entityDepths.end())
            m_entityDepths.insert({newDepth, {}});
        m_entityDepths[newDepth].push_back(ent);
    }

    Scene::Scene(Game *parentGame)
            : m_parentGame(parentGame), EntityContainer(EntityContainer::SCENE) {
        // Check game
        if (parentGame == nullptr)
            Console::Fail("Scene", "Scene cannot have a null parent game.");
    }

    Scene::Scene(Game *parentGame, Vector2 grav, float ppm)
            : Scene(parentGame) {
        // Physics setup
        m_physicsEnabled = true;
        m_physicsWorld = new physics::PhysicsWorld(grav, ppm);
    }

    Scene::~Scene() {
        Console::Notice("Scene", "Deleting scene.");

        // Delete physics world
        delete m_physicsWorld;

        // Clear vectors
        m_entityActivities.clear();
        m_entityDepths.clear();
    }

    graphics::Camera *Scene::getActiveCamera() const {
        return m_activeCamera;
    }

    void Scene::setActiveCamera(graphics::Camera *camera) {
        m_activeCamera = camera;
    }

    Rectangle Scene::getCullArea() const {
        return {getCullAreaPosition(), getCullAreaWidth(), getCullAreaHeight()};
    }

    Vector2 Scene::getCullAreaPosition() const {
        // Get viewport position
        auto pos = getViewportPosition();

        // Get center of viewport
        pos.X += getViewportWidth() / 2.0f;
        pos.Y += getViewportHeight() / 2.0f;

        if (m_cullAreaCenterInViewport) {
            // Offset from cull area size
            pos.X -= getCullAreaWidth() / 2.0f;
            pos.Y -= getCullAreaHeight() / 2.0f;
        }

        return pos;
    }

    Vector2 Scene::getCullAreaEndPosition() const {
        auto pos = getCullAreaPosition();
        pos.X += m_cullAreaWidth;
        pos.Y += m_cullAreaHeight;
        return pos;
    }

    float Scene::getCullAreaWidth() const {
        float scale = 1;
        if (m_activeCamera != nullptr) {
            scale = m_activeCamera->Zoom;
        }

        return m_cullAreaWidth / scale;
    }

    float Scene::getCullAreaHeight() const {
        float scale = 1;
        if (m_activeCamera != nullptr) {
            scale = m_activeCamera->Zoom;
        }

        return m_cullAreaHeight / scale;
    }

    void Scene::setCullArea(float width, float height, bool centerInViewport) {
        m_cullAreaWidth = width;
        m_cullAreaHeight = height;
        m_cullAreaCenterInViewport = centerInViewport;
    }

    Rectangle Scene::getViewport() const {
        return {getViewportPosition(), getViewportWidth(), getViewportHeight()};
    }

    Vector2 Scene::getViewportPosition() const {
        if (m_activeCamera != nullptr) {
            // Top left coordinate of camera is screen 0,0
            auto pos = m_activeCamera->screenToWorld({0, 0});
            return pos;
        }
        return {0, 0};
    }

    Vector2 Scene::getViewportEndPosition() const {
        auto pos = getViewportPosition();
        pos.X += getViewportWidth();
        pos.Y += getViewportHeight();
        return pos;
    }

    float Scene::getViewportWidth() const {
        float scale = 1;
        if (m_activeCamera != nullptr) {
            scale = m_activeCamera->Zoom;
        }

        if (m_parentGame->Config.MaintainResolution) {
            // Virtual size
            return m_parentGame->Config.TargetWidth / scale;
        } else {
            // Window size
            return m_parentGame->getGameWindow()->getWidth() / scale;
        }
    }

    float Scene::getViewportHeight() const {
        float scale = 1;
        if (m_activeCamera != nullptr) {
            scale = m_activeCamera->Zoom;
        }

        if (m_parentGame->Config.MaintainResolution) {
            // Virtual size
            return m_parentGame->Config.TargetHeight / scale;
        } else {
            // Window size
            return m_parentGame->getGameWindow()->getHeight() / scale;
        }
    }

    Game *Scene::getGame() {
        return m_parentGame;
    }

    filesystem::ResourceManager *Scene::getResourceManager() const {
        return m_parentGame->getResourceManager();
    }

    physics::PhysicsWorld *Scene::getPhysicsWorld() {
        return m_physicsWorld;
    }

    const physics::PhysicsWorld *Scene::getPhysicsWorld() const {
        return m_physicsWorld;
    }

    void Scene::pause() {
        m_paused = true;
    }

    void Scene::resume() {
        m_paused = false;
    }

    bool Scene::isPaused() {
        return m_paused;
    }

    void Scene::draw(graphics::Renderer *renderer) {
        // Invoke draw calls
        OnDraw(renderer);

        // Draw with camera
        if (m_activeCamera != nullptr)
            m_activeCamera->beginCamera(renderer->getGraphicsDevice());

        OnDrawCamera(renderer);

        // Draw entities with camera
        for (auto pair : m_entityDepths) {
            auto vec = pair.second;
            for (auto ent : vec) {
                if (ent != nullptr) {
                    if (m_entityActivities[ent] && ent->DrawWithCamera)
                        ent->draw(renderer);
                }
            }
        }

        if (m_activeCamera != nullptr)
            m_activeCamera->endCamera(renderer->getGraphicsDevice());

        // Draw entities
        for (auto pair : m_entityDepths) {
            auto vec = pair.second;
            for (auto ent : vec) {
                if (ent != nullptr) {
                    if (m_entityActivities.find(ent) == m_entityActivities.end())
                        m_entityActivities.insert({ent, true});

                    if (m_entityActivities[ent] && !ent->DrawWithCamera)
                        ent->draw(renderer);
                }
            }
        }
    }

    void Scene::update() {
        if (m_paused) {
            OnPersistentUpdate();
            return;
        }

        auto fps = m_parentGame->getTargetFPS();

        m_updateCounter++;

        // Every quarter second
        if (m_updateCounter % fps / 4 == 0) {
            // Check culling

            for (auto ent : getChildren()) {
                if (ent != nullptr) {
                    // Check if we can cull
                    if (ent->canCull()) {
                        auto area = getCullArea();
                        if (m_entityActivities.find(ent) == m_entityActivities.end())
                            m_entityActivities.insert({ent, true});
                        m_entityActivities[ent] = !ent->checkForCulling(area);
                    }
                }
            }
        }

        if (m_updateCounter > fps)
            m_updateCounter -= fps;

        // Invoke updates
        OnUpdate();
        OnPersistentUpdate();

        // Physics
        if (m_physicsWorld != nullptr) {
            m_physicsWorld->step(1.0f / m_parentGame->getTargetFPS(), 6, 2); // TODO: Proper values
        }
    }
}
