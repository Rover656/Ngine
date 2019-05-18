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

#include "Scene2D.h"

namespace NerdThings::Ngine::Core {
    // Public Constructor(s)

    Scene2D::Scene2D() : BaseScene() {
        // Will we need stuff here??
    }

    // Public Methods

    TCamera2D *Scene2D::GetActiveCamera() const {
        return _ActiveCamera;
    }

    void Scene2D::SetActiveCamera(TCamera2D *camera_) {
        _ActiveCamera = camera_;
    }
}
