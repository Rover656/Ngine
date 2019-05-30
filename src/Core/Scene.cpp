/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Scene.h"

namespace NerdThings::Ngine::Core {
    // Public Constructor(s)

    Scene::Scene() {
        // Not doing anything yet...
    }

    // Public Methods

    void Scene::Draw() {
        // Invoke draw calls
        OnDraw({});
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
