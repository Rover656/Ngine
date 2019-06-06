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

namespace NerdThings::Ngine::Core {
    // Private Methods

    // The following two functions do nothing
    // This method is here for adding an entity parent
    void Scene::RemoveEntityParent(BaseEntity *ent_) {}

    void Scene::SetEntityParent(BaseEntity *ent_) {}

    // Public Constructor(s)

    Scene::Scene() = default;

    // Public Methods

    void Scene::Draw() {
        // Invoke draw calls
        OnDraw({});

        // Draw entities
        for (auto pair : _EntityDepths) {
            auto vec = pair.second;
            for (auto ent : vec) {
                ent->Draw();
            }
        }

        // Draw with camera
        if (_ActiveCamera != nullptr)
            _ActiveCamera->BeginCamera();

        OnDrawCamera({});

        // Draw entities with camera
        for (auto pair : _EntityDepths) {
            auto vec = pair.second;
            for (auto ent : vec) {
                ent->DrawCamera();
            }
        }

        if (_ActiveCamera != nullptr)
            _ActiveCamera->EndCamera();
    }

    Graphics::TCamera *Scene::GetActiveCamera() const {
        return _ActiveCamera;
    }

    void Scene::InternalSetEntityDepth(int depth_, BaseEntity *ent_) {
        if (_EntityDepths.find(depth_) == _EntityDepths.end())
            _EntityDepths.insert({ depth_, {} });
        _EntityDepths[depth_].push_back(ent_);
    }

    void Scene::InternalUpdateEntityDepth(int oldDepth_, int newDepth_, BaseEntity *ent_) {
        _EntityDepths[oldDepth_].erase(std::remove(_EntityDepths[oldDepth_].begin(), _EntityDepths[oldDepth_].end(), ent_), _EntityDepths[oldDepth_].end());

        if (_EntityDepths.find(newDepth_) == _EntityDepths.end())
            _EntityDepths.insert({ newDepth_, {} });
        _EntityDepths[newDepth_].push_back(ent_);
    }

    void Scene::SetActiveCamera(Graphics::TCamera *camera_) {
        _ActiveCamera = camera_;
    }

    void Scene::Update() {
        // TODO: Entity Auto Culling system??

        // Invoke updates
        OnUpdate({});
    }
}
