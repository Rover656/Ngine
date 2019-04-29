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

namespace Ngine {
    namespace Core {
        // Public Constructor(s)

        Scene::Scene() {
            // Will we need stuff here??
        }

        // Public Methods

        void Scene::Draw() {
            // TODO: Culling system??
            for (auto entity : GetEntities()) {
				entity->Draw();
            }
        }

        void Scene::Update() {
            // TODO: Culling system??
			for (auto entity : GetEntities()) {
				entity->Update();
			}
		}

    }
}