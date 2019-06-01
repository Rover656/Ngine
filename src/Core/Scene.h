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
*
**********************************************************************************************/

#ifndef BASESCENE_H
#define BASESCENE_H

#include "../ngine.h"

#include "EntityContainer.h"
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

        // Private Methods

        void RemoveEntityParent(BaseEntity *ent_) override;

        void SetEntityParent(BaseEntity *ent_) override;
    public:
        // Public Fields

        /*
         * The collision map.
         * This is controlled by collision components
         */
        std::unordered_map<std::string, std::vector<BaseEntity*>> CollisionMap;

        /*
         * On draw event
         */
        EventHandler<EventArgs> OnDraw;

        /*
         * On draw with the camera
         */
        EventHandler<EventArgs> OnDrawCamera;

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
