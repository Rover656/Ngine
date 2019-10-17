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

namespace NerdThings::Ngine {
    // Private Methods

    // The following two functions do nothing
    // This method is here for adding an entity parent
    void Scene::RemoveEntityParent(BaseEntity *ent_) {
        for (auto vec : _EntityDepths) {
            for (auto ent : vec.second) {
                if (ent == ent_) {
                    _EntityDepths[vec.first].erase(std::remove(_EntityDepths[vec.first].begin(), _EntityDepths[vec.first].end(), ent_), _EntityDepths[vec.first].end());
                }
            }
        }
    }

    void Scene::SetEntityParent(BaseEntity *ent_) {
        // When an entity is added, mark as active
        _EntityActivities.insert({ent_, true});
    }

    // Public Constructor(s)

    Scene::Scene(Game *parentGame_)
        : _ParentGame(parentGame_) {
        if (parentGame_ == nullptr) {
            ConsoleMessage("A scene has been fed null for the parent game.", "FATAL", "SCENE");
            throw std::runtime_error("Cannot have a null game.");
        }
    }

    // Destructor

    Scene::~Scene() {
        ConsoleMessage("Deleting Scene.", "NOTICE", "SCENE");
        CollisionMap.clear();
        _EntityActivities.clear();
        _EntityDepths.clear();
    }

    // Public Methods

    void Scene::Draw() {
        // Invoke draw calls
        OnDraw({});

        // Draw with camera
        if (_ActiveCamera != nullptr)
            _ActiveCamera->BeginCamera();

        OnDrawCamera({});

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
        return {GetCullAreaPosition(), _CullAreaWidth, _CullAreaHeight};
    }

    Vector2 Scene::GetCullAreaEndPosition() const {
        auto pos = GetCullAreaPosition();
        pos.X += _CullAreaWidth;
        pos.Y += _CullAreaHeight;
        return pos;
    }

    float Scene::GetCullAreaHeight() const {
        return _CullAreaHeight;
    }

    Vector2 Scene::GetCullAreaPosition() const {
        if (_CullAreaCenterInViewport) {
            // Get viewport position
            auto pos = GetViewportPosition();

            // Get center of viewport
            pos.X += GetViewportWidth() / 2.0f;
            pos.Y += GetViewportHeight() / 2.0f;

            // Offset from cull area size
            pos.X -= _CullAreaWidth / 2.0f;
            pos.Y -= _CullAreaHeight / 2.0f;

            return pos;
        }
        return GetViewportPosition();
    }

    float Scene::GetCullAreaWidth() const {
        return _CullAreaWidth;
    }

    Game *Scene::GetParentGame() {
        return _ParentGame;
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
        if (_ActiveCamera != nullptr) {
            if (_ParentGame->Config.MaintainResolution) {
                return _ParentGame->Config.TargetHeight / _ActiveCamera->Zoom;
            } else {
                return Window::GetHeight() / _ActiveCamera->Zoom;
            }
        } else if (_ParentGame->Config.MaintainResolution) {
            // Virtual size
            return _ParentGame->Config.TargetHeight;
        } else {
            // Window size
            return Window::GetHeight();
        }
    }

    Vector2 Scene::GetViewportPosition() const {
        if (_ActiveCamera != nullptr) {
            return _ActiveCamera->Position - _ActiveCamera->Origin;
        }
        return {0, 0}; // If we add a viewport system, this will have different values
    }

    float Scene::GetViewportWidth() const {
        if (_ActiveCamera != nullptr) {
            if (_ParentGame->Config.MaintainResolution) {
                return _ParentGame->Config.TargetWidth / _ActiveCamera->Zoom;
            } else {
                return Window::GetHeight() / _ActiveCamera->Zoom;
            }
        } else if (_ParentGame->Config.MaintainResolution) {
            // Virtual size
            return _ParentGame->Config.TargetWidth;
        } else {
            // Window size
            return Window::GetHeight();
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
            OnPersistentUpdate({});
            return;
        }

        auto fps = _ParentGame->GetTargetFPS();

        _UpdateCounter++;

        // Every half second
        if (_UpdateCounter % fps / 2 == 0) {
            // Check culling

            for (auto ent : GetEntities()) {
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
    }
}
