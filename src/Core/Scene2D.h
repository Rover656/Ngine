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

#include "../ngine.h"
#include "BaseScene.h"

#include "EventHandler.h"

namespace Ngine::Core {
    /*
     * A 2D Game Scene
     */
    class NEAPI Scene2D : public BaseScene {
        // Private Fields

        /*
         * The currently active camera
         */
        TCamera2D *_ActiveCamera = nullptr;

        /*
         * The default camera for when there is no camera
         */
        TCamera2D _DefaultCamera = TCamera2D();

    public:
        // Public Fields


        // Public Constructor(s)

        Scene2D();

        // Public Methods

        /*
         * Get the current active camera
         */
        TCamera2D *GetActiveCamera() const;

        /*
         * Set the current active camera
         */
        void SetActiveCamera(TCamera2D *camera_);
    };
}

#endif //SCENE2D_H
