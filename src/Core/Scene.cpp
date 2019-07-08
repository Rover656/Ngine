/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*   
*   File reviewed on 01/06/2019 by R.M
*       Needs a review again after adding physics engine
*
**********************************************************************************************/

#include "Scene.h"

#include "BaseEntity.h"
#include "Game.h"

namespace NerdThings::Ngine::Core {
    // Private Methods

    // The following two functions do nothing
    // This method is here for adding an entity parent
    void Scene::RemoveEntityParent(BaseEntity *ent_) {}

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
        ConsoleMessage("Deleting entities.", "NOTICE", "SCENE");
        for (auto ent : GetEntities()) {
            delete ent;
        }
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

    Graphics::TCamera *Scene::GetActiveCamera() const {
        return _ActiveCamera;
    }

    Math::TRectangle Scene::GetCullArea() const {
        auto cam = GetActiveCamera();

        if (_CullAreaCenter)
            return {
                cam->Target.X - _CullAreaWidth * 0.5f, cam->Target.Y - _CullAreaHeight * 0.5f, _CullAreaWidth,
                _CullAreaHeight
            };
        return {cam->Target.X - cam->Origin.X, cam->Target.Y - cam->Origin.Y, _CullAreaWidth, _CullAreaHeight};
    }

    Game *Scene::GetParentGame() {
        return _ParentGame;
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

    void Scene::SetActiveCamera(Graphics::TCamera *camera_) {
        _ActiveCamera = camera_;
    }

    void Scene::SetCullArea(float width_, float height_, bool centerOnCamera_) {
        _CullAreaWidth = width_;
        _CullAreaHeight = height_;
        _CullAreaCenter = centerOnCamera_;
    }

    void Scene::Update() {
        if (_Paused) {
            OnPersistentUpdate({});
            return;
        }

        auto fps = _ParentGame->GetUpdateFPS();

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
                        if (ent->CheckForCulling(area)) {
                            _EntityActivities[ent] = true;
                        } else {
                            _EntityActivities[ent] = false;
                        }
                    }
                }
            }
        }

        if (_UpdateCounter > fps)
            _UpdateCounter -= fps;

        // Invoke updates
        OnUpdate({});
        OnPersistentUpdate({});
    }
}
