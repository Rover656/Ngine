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

#include "Scene.hpp"

#include "Entity.hpp"
#include "Game.hpp"
#include "Logger.hpp"

namespace NerdThings::Ngine {
    void Scene::_addEntity(Entity *ent_) {
        // When an entity is added, mark as active
        m_entityActivities.insert({ent_, true});

        // Add entity and mark it's depth
        if (m_entityDepths.find(ent_->m_depth) == m_entityDepths.end())
            m_entityDepths.insert({ent_->m_depth, {}});
        m_entityDepths[ent_->m_depth].push_back(ent_);

        // Fire scene add event
        ent_->OnInit();
    }

    void Scene::_removeEntity(Entity *ent_) {
        for (auto vec : m_entityDepths) {
            for (auto ent : vec.second) {
                if (ent == ent_) {
                    m_entityDepths[vec.first].erase(
                            std::remove(m_entityDepths[vec.first].begin(), m_entityDepths[vec.first].end(), ent_),
                            m_entityDepths[vec.first].end());
                }
            }
        }
    }

    void Scene::_updateEntityDepth(int newDepth_, Entity *ent_) {
        if (ent_->m_depth == newDepth_)
            return; // Short circuit if depth's are the same because we don't want to remove and re-add
        m_entityDepths[ent_->m_depth].erase(
                std::remove(m_entityDepths[ent_->m_depth].begin(), m_entityDepths[ent_->m_depth].end(), ent_),
                m_entityDepths[ent_->m_depth].end());

        if (m_entityDepths.find(newDepth_) == m_entityDepths.end())
            m_entityDepths.insert({newDepth_, {}});
        m_entityDepths[newDepth_].push_back(ent_);
    }

    Scene::Scene(Game *parentGame_)
            : m_parentGame(parentGame_), EntityContainer(EntityContainer::SCENE) {
        // Check game
        if (parentGame_ == nullptr)
            Logger::Fail("Scene", "Scene cannot have a null parent game.");
    }

    Scene::Scene(Game *parentGame_, Vector2 grav_, float ppm_)
            : Scene(parentGame_) {
        // Physics setup
        m_physicsEnabled = true;
        m_physicsWorld = new Physics::PhysicsWorld(grav_, ppm_);
    }

    Scene::~Scene() {
        Logger::Notice("Scene", "Deleting scene.");

        // Delete physics world
        delete m_physicsWorld;

        // Clear vectors
        m_entityActivities.clear();
        m_entityDepths.clear();
    }

    Graphics::Camera *Scene::GetActiveCamera() const {
        return m_activeCamera;
    }

    void Scene::SetActiveCamera(Graphics::Camera *camera_) {
        m_activeCamera = camera_;
    }

    Rectangle Scene::GetCullArea() const {
        return {GetCullAreaPosition(), GetCullAreaWidth(), GetCullAreaHeight()};
    }

    Vector2 Scene::GetCullAreaPosition() const {
        // Get viewport position
        auto pos = GetViewportPosition();

        // Get center of viewport
        pos.X += GetViewportWidth() / 2.0f;
        pos.Y += GetViewportHeight() / 2.0f;

        if (m_cullAreaCenterInViewport) {
            // Offset from cull area size
            pos.X -= GetCullAreaWidth() / 2.0f;
            pos.Y -= GetCullAreaHeight() / 2.0f;
        }

        return pos;
    }

    Vector2 Scene::GetCullAreaEndPosition() const {
        auto pos = GetCullAreaPosition();
        pos.X += m_cullAreaWidth;
        pos.Y += m_cullAreaHeight;
        return pos;
    }

    float Scene::GetCullAreaWidth() const {
        float scale = 1;
        if (m_activeCamera != nullptr) {
            scale = m_activeCamera->Zoom;
        }

        return m_cullAreaWidth / scale;
    }

    float Scene::GetCullAreaHeight() const {
        float scale = 1;
        if (m_activeCamera != nullptr) {
            scale = m_activeCamera->Zoom;
        }

        return m_cullAreaHeight / scale;
    }

    void Scene::SetCullArea(float width_, float height_, bool centerInViewport_) {
        m_cullAreaWidth = width_;
        m_cullAreaHeight = height_;
        m_cullAreaCenterInViewport = centerInViewport_;
    }

    Rectangle Scene::GetViewport() const {
        return {GetViewportPosition(), GetViewportWidth(), GetViewportHeight()};
    }

    Vector2 Scene::GetViewportPosition() const {
        if (m_activeCamera != nullptr) {
            // Top left coordinate of camera is screen 0,0
            auto pos = m_activeCamera->ScreenToWorld({0, 0});
            return pos;
        }
        return {0, 0};
    }

    Vector2 Scene::GetViewportEndPosition() const {
        auto pos = GetViewportPosition();
        pos.X += GetViewportWidth();
        pos.Y += GetViewportHeight();
        return pos;
    }

    float Scene::GetViewportWidth() const {
        float scale = 1;
        if (m_activeCamera != nullptr) {
            scale = m_activeCamera->Zoom;
        }

        if (m_parentGame->Config.MaintainResolution) {
            // Virtual size
            return m_parentGame->Config.TargetWidth / scale;
        } else {
            // Window size
            return m_parentGame->GetGameWindow()->GetWidth() / scale;
        }
    }

    float Scene::GetViewportHeight() const {
        float scale = 1;
        if (m_activeCamera != nullptr) {
            scale = m_activeCamera->Zoom;
        }

        if (m_parentGame->Config.MaintainResolution) {
            // Virtual size
            return m_parentGame->Config.TargetHeight / scale;
        } else {
            // Window size
            return m_parentGame->GetGameWindow()->GetHeight() / scale;
        }
    }

    Game *Scene::GetGame() {
        return m_parentGame;
    }

    Filesystem::ResourceManager *Scene::GetResourceManager() const {
        return m_parentGame->GetResourceManager();
    }

    Physics::PhysicsWorld *Scene::GetPhysicsWorld() {
        return m_physicsWorld;
    }

    const Physics::PhysicsWorld *Scene::GetPhysicsWorld() const {
        return m_physicsWorld;
    }

    void Scene::Pause() {
        m_paused = true;
    }

    void Scene::Resume() {
        m_paused = false;
    }

    bool Scene::IsPaused() {
        return m_paused;
    }

    void Scene::Draw() {
        // Invoke draw calls
        OnDraw();

        // Draw with camera
        if (m_activeCamera != nullptr)
            m_activeCamera->BeginCamera();

        OnDrawCamera();

        // Draw entities with camera
        for (auto pair : m_entityDepths) {
            auto vec = pair.second;
            for (auto ent : vec) {
                if (ent != nullptr) {
                    if (m_entityActivities[ent] && ent->DrawWithCamera)
                        ent->Draw();
                }
            }
        }

        if (m_activeCamera != nullptr)
            m_activeCamera->EndCamera();

        // Draw entities
        for (auto pair : m_entityDepths) {
            auto vec = pair.second;
            for (auto ent : vec) {
                if (ent != nullptr) {
                    if (m_entityActivities.find(ent) == m_entityActivities.end())
                        m_entityActivities.insert({ent, true});

                    if (m_entityActivities[ent] && !ent->DrawWithCamera)
                        ent->Draw();
                }
            }
        }
    }

    void Scene::Update() {
        if (m_paused) {
            OnPersistentUpdate();
            return;
        }

        auto fps = m_parentGame->GetTargetFPS();

        m_updateCounter++;

        // Every quarter second
        if (m_updateCounter % fps / 4 == 0) {
            // Check culling

            for (auto ent : GetChildren()) {
                if (ent != nullptr) {
                    // Check if we can cull
                    if (ent->CanCull()) {
                        auto area = GetCullArea();
                        if (m_entityActivities.find(ent) == m_entityActivities.end())
                            m_entityActivities.insert({ent, true});
                        m_entityActivities[ent] = !ent->CheckForCulling(area);
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
            m_physicsWorld->Step(1.0f / m_parentGame->GetTargetFPS(), 6, 2); // TODO: Proper values
        }
    }
}
