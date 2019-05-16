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

#ifndef SCENE2D_H
#define SCENE2D_H

// Include ngine
#include "../ngine.h"
#include "BaseScene.h"

namespace Ngine {
    namespace Core {
        /*
         * A 2D Game Scene
         */
        class NEAPI Scene2D : public BaseScene {
            // Private Fields

            /*
             * The currently active camera
             */
            Types::TCamera2D* _ActiveCamera = nullptr;

            /*
             * The default camera for when there is no camera
             */
            Types::TCamera2D _DefaultCamera = Types::TCamera2D();

        public:

            // Public Constructor(s)

            Scene2D();

            // Public Methods

            /*
             * Get the current active camera
             */
            Types::TCamera2D* GetActiveCamera() const;

            /*
             * Set the current active camera
             */
            void SetActiveCamera(Types::TCamera2D* camera_);
        };
    }
}

#endif //SCENE2D_H
