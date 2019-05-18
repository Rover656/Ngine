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

#include "BaseScene.h"

namespace NerdThings::Ngine::Core {
    // Public Constructor(s)

    BaseScene::BaseScene() {
        // Will we need stuff here??
    }

    // Public Methods

    void BaseScene::Draw() {
        // TODO: Culling system??
        for (auto entity : GetEntities()) {
            entity->Draw();
        }
    }

    void BaseScene::Update() {
        // TODO: Culling system??
        for (auto entity : GetEntities()) {
            entity->Update();
        }
    }

}
