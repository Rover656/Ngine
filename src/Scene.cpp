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

#include "Scene.h"

#include "BaseEntity.h"
#include "Game.h"
#include "Logger.h"

namespace NerdThings::Ngine {
    // Private Methods

    // The following two functions do nothing
    // This method is here for adding an entity parent
    void Scene::ProcessChildRemoved(BaseEntity *ent_) {
        for (auto vec : _EntityDepths) {
            for (auto ent : vec.second) {
                if (ent == ent_) {
                    _EntityDepths[vec.first].erase(std::remove(_EntityDepths[vec.first].begin(), _EntityDepths[vec.first].end(), ent_), _EntityDepths[vec.first].end());
                }
            }
        }
    }

    void Scene::ProcessChildAdded(BaseEntity *ent_) {
        // When an entity is added, mark as active
        _EntityActivities.insert({ent_, true});
    }

    // Public Constructor(s)

    Scene::Scene(Game *parentGame_, bool physicsEnabled_, Vector2 grav, float ppm)
            : _ParentGame(parentGame_) {
        // Check game
        if (parentGame_ == nullptr) {
            Logger::Fail("Scene", "Scene cannot have a null parent game.");
        }

        // Physics setup
        _PhysicsEnabled = physicsEnabled_;
        if (_PhysicsEnabled) {
            _PhysicsWorld = new Physics::PhysicsWorld(grav, ppm);
        }
    }

    // Destructor

    Scene::~Scene() {
        Logger::Notice("Scene", "Deleting scene.");

        // Delete physics world
        if (_PhysicsWorld != nullptr) delete _PhysicsWorld;

        // Clear vectors
        _EntityActivities.clear();
        _EntityDepths.clear();
    }

    // Public Methods

    void Scene::Draw() {
        // Invoke draw calls
        OnDraw();

        // Draw with camera
        if (_ActiveCamera != nullptr)
            _ActiveCamera->BeginCamera();

        OnDrawCamera();

        // Draw entities with camera
        for (auto pair : _EntityDepths) {
            auto vec = pair.second;
            for (auto ent : vec) {
                if (ent != nullptr) {
                    if (_EntityActivities[ent] && ent->DrawWithCamera)
                        ent->Draw();
                }
            }
        }

        if (_ActiveCamera != nullptr)
            _ActiveCamera->EndCamera();

        // Draw entities
        for (auto pair : _EntityDepths) {
            auto vec = pair.second;
            for (auto ent : vec) {
                if (ent != nullptr) {
                    if (_EntityActivities.find(ent) == _EntityActivities.end())
                        _EntityActivities.insert({ent, true});

                    if (_EntityActivities[ent] && !ent->DrawWithCamera)
                        ent->Draw();
                }
            }
        }
    }

    Graphics::Camera *Scene::GetActiveCamera() const {
        return _ActiveCamera;
    }

    Rectangle Scene::GetCullArea() const {
        return {GetCullAreaPosition(), GetCullAreaWidth(), GetCullAreaHeight()};
    }

    Vector2 Scene::GetCullAreaEndPosition() const {
        auto pos = GetCullAreaPosition();
        pos.X += _CullAreaWidth;
        pos.Y += _CullAreaHeight;
        return pos;
    }

    float Scene::GetCullAreaHeight() const {
        float scale = 1;
        if (_ActiveCamera != nullptr) {
            scale = _ActiveCamera->Zoom;
        }

        return _CullAreaHeight / scale;
    }

    Vector2 Scene::GetCullAreaPosition() const {
        // Get viewport position
        auto pos = GetViewportPosition();

        // Get center of viewport
        pos.X += GetViewportWidth() / 2.0f;
        pos.Y += GetViewportHeight() / 2.0f;

        if (_CullAreaCenterInViewport) {
            // Offset from cull area size
            pos.X -= GetCullAreaWidth() / 2.0f;
            pos.Y -= GetCullAreaHeight() / 2.0f;
        }

        return pos;
    }

    float Scene::GetCullAreaWidth() const {
        float scale = 1;
        if (_ActiveCamera != nullptr) {
            scale = _ActiveCamera->Zoom;
        }

        return _CullAreaWidth / scale;
    }

    Game *Scene::GetGameGame() {
        return _ParentGame;
    }

    Physics::PhysicsWorld *Scene::GetPhysicsWorld() {
        return _PhysicsWorld;
    }

    Rectangle Scene::GetViewport() const {
        return {GetViewportPosition(), GetViewportWidth(), GetViewportHeight()};
    }

    Vector2 Scene::GetViewportEndPosition() const {
        auto pos = GetViewportPosition();
        pos.X += GetViewportWidth();
        pos.Y += GetViewportHeight();
        return pos;
    }

    float Scene::GetViewportHeight() const {
        float scale = 1;
        if (_ActiveCamera != nullptr) {
            scale = _ActiveCamera->Zoom;
        }

        if (_ParentGame->Config.MaintainResolution) {
            // Virtual size
            return _ParentGame->Config.TargetHeight / scale;
        } else {
            // Window size
            return _ParentGame->GetGameWindow()->GetWidth() / scale;
        }
    }

    Vector2 Scene::GetViewportPosition() const {
        if (_ActiveCamera != nullptr) {
            // Top left coordinate of camera is screen 0,0
            auto pos = _ActiveCamera->ScreenToWorld({0, 0});
            return pos;
        }
        return {0, 0};
    }

    float Scene::GetViewportWidth() const {
        float scale = 1;
        if (_ActiveCamera != nullptr) {
            scale = _ActiveCamera->Zoom;
        }

        if (_ParentGame->Config.MaintainResolution) {
            // Virtual size
            return _ParentGame->Config.TargetWidth / scale;
        } else {
            // Window size
            return _ParentGame->GetGameWindow()->GetWidth() / scale;
        }
    }

    void Scene::InternalSetEntityDepth(int depth_, BaseEntity *ent_) {
        if (_EntityDepths.find(depth_) == _EntityDepths.end())
            _EntityDepths.insert({depth_, {}});
        _EntityDepths[depth_].push_back(ent_);
    }

    void Scene::InternalUpdateEntityDepth(int oldDepth_, int newDepth_, BaseEntity *ent_) {
        if (oldDepth_ == newDepth_)
            return; // Short circuit if depth's are the same because we don't want to remove and re-add
        _EntityDepths[oldDepth_].erase(
                std::remove(_EntityDepths[oldDepth_].begin(), _EntityDepths[oldDepth_].end(), ent_),
                _EntityDepths[oldDepth_].end());

        if (_EntityDepths.find(newDepth_) == _EntityDepths.end())
            _EntityDepths.insert({newDepth_, {}});
        _EntityDepths[newDepth_].push_back(ent_);
    }

    bool Scene::IsPaused() {
        return _Paused;
    }

    void Scene::Pause() {
        _Paused = true;
    }

    void Scene::Resume() {
        _Paused = false;
    }

    void Scene::SetActiveCamera(Graphics::Camera *camera_) {
        _ActiveCamera = camera_;
    }

    void Scene::SetCullArea(float width_, float height_, bool centerInViewport_) {
        _CullAreaWidth = width_;
        _CullAreaHeight = height_;
        _CullAreaCenterInViewport = centerInViewport_;
    }

    void Scene::Update() {
        if (_Paused) {
            OnPersistentUpdate();
            return;
        }

        auto fps = _ParentGame->GetTargetFPS();

        _UpdateCounter++;

        // Every quarter second
        if (_UpdateCounter % fps / 4 == 0) {
            // Check culling

            for (auto ent : GetChildren()) {
                if (ent != nullptr) {
                    // Check if we can cull
                    if (ent->GetCanCull()) {
                        auto area = GetCullArea();
                        if (_EntityActivities.find(ent) == _EntityActivities.end())
                            _EntityActivities.insert({ent, true});
                        _EntityActivities[ent] = !ent->CheckForCulling(area);
                    }
                }
            }
        }

        if (_UpdateCounter > fps)
            _UpdateCounter -= fps;

        // Invoke updates
        OnUpdate();
        OnPersistentUpdate();

        // Physics
        if (_PhysicsWorld != nullptr) {
            _PhysicsWorld->Step(1.0f/_ParentGame->GetTargetFPS(), 6, 2); // TODO: Proper values
        }
    }
}
