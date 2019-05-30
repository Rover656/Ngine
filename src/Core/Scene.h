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

#include "EntityContainer.h"    // Required for: EntityContainer
#include "EventHandler.h"

namespace NerdThings::Ngine::Core {
    /*
     * A container for entities
     */
    class NEAPI Scene : public EntityContainer {
        // Private Fields

        /*
         * Currently active camera
         */
        TCamera *_ActiveCamera = nullptr;

    public:
        // Public Fields

        /*
         * On draw event
         */
        EventHandler<EventArgs> OnDraw;

        /*
         * On scene load
         */
        EventHandler<SceneLoadEventArgs> OnLoad;

        /*
         * On scene unload
         */
        EventHandler<SceneLoadEventArgs> OnUnLoad;

        /*
         * On update event
         */
        EventHandler<EventArgs> OnUpdate;

        // Public Constructor(s)

        /*
         * Create a new Scene
         */
        Scene();

        // Public Destructor

        virtual ~Scene() = default;

        // Public Methods

        /*
         * Draw the scene
         */
        void Draw();

        /*
         * Get the currently active camera
         */
        [[nodiscard]] TCamera *GetActiveCamera() const;

        /*
         * Set the currently active camera
         */
        void SetActiveCamera(TCamera *camera_);

        /*
         * Update the scene
         */
        void Update();
    };
}

#endif //BASESCENE_H
