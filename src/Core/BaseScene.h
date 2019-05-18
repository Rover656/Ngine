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

#ifndef BASESCENE_H
#define BASESCENE_H

// Include ngine
#include "../ngine.h"

#include "BaseEntity.h"         // Required for: BaseEntity
#include "EntityContainer.h"    // Required for: EntityContainer

namespace Ngine::Core {
    /*
     * A container for entities
     */
    class NEAPI BaseScene : public EntityContainer {
    public:
        // Public Constructor(s)

        /*
         * Create a new Scene
         */
        BaseScene();

        // Public Destructor

        virtual ~BaseScene() = default;

        // Public Methods

        /*
         * Draw the scene
         */
        virtual void Draw();

        /*
         * Update the scene
         */
        virtual void Update();
    };
}

#endif //BASESCENE_H
