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

        // Draw with camera
        if (_ActiveCamera != nullptr)
            _ActiveCamera->BeginCamera();

        OnDrawCamera({});

        if (_ActiveCamera != nullptr)
            _ActiveCamera->EndCamera();
    }

    TCamera *Scene::GetActiveCamera() const {
        return _ActiveCamera;
    }

    void Scene::SetActiveCamera(TCamera *camera_) {
        _ActiveCamera = camera_;
    }

    void Scene::Update() {
        // TODO: Entity Auto Culling system??

        // Invoke updates
        OnUpdate({});
    }

}
