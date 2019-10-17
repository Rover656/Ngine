/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef BASESCENE_H
#define BASESCENE_H

#include "Ngine.h"

#include "Rectangle.h"
#include "Graphics/Camera.h"
#include "EntityContainer.h"
#include "EventHandler.h"

namespace NerdThings::Ngine {
    class NEAPI Game;

    /*
     * Scene loaded event args.
     */
    struct SceneLoadEventArgs : EventArgs {
        // Public Fields

        /*
         * The game that the scene has been loaded into
         */
        Game *ParentGame;

        // Public Constructor(s)

        SceneLoadEventArgs(Game *game_)
                : ParentGame(game_) {}
    };

    /*
     * A container for entities
     */
    class NEAPI Scene : public EntityContainer {
        // Private Fields

        /*
         * Currently active camera
         */
        Graphics::Camera *_ActiveCamera = nullptr;

        /*
         * Whether or not the cull area centers around
         */
        bool _CullAreaCenter = false;

        /*
         * The culling area height
         */
        float _CullAreaHeight;

        /*
         * The culling area width
         */
        float _CullAreaWidth;

        /*
         * Whether or not an entity is active
         */
        std::unordered_map<BaseEntity *, bool> _EntityActivities;

        /*
         * Depth key list containing entities.
         * This is used for drawing.
         */
        std::map<int, std::vector<BaseEntity *>> _EntityDepths;

        /*
         * The parent game
         */
        Game* _ParentGame = nullptr;

        /*
         * Whether or not the scene is paused.
         */
        bool _Paused = false;

        /*
         * The update counter
         */
        int _UpdateCounter = 0;

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
         * On persistent update event.
         * This means updates will run through pauses.
         */
        EventHandler<EventArgs> OnPersistentUpdate;

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
        Scene(Game* parentGame_);

        // Public Destructor

        virtual ~Scene();

        // Public Methods

        /*
         * Draw the scene
         */
        void Draw();

        /*
         * Get the currently active camera
         */
        [[nodiscard]] Graphics::Camera *GetActiveCamera() const;

        /*
         * Get the culling area
         */
        Rectangle GetCullArea() const;

        /*
         * Get the parent game
         */
        Game *GetParentGame();

        /*
         * Set the entity depth in the scene (internally used)
         */
        void InternalSetEntityDepth(int depth_, BaseEntity *ent_);

        /*
         * Update the entity depth in the scene (internally used)
         */
        void InternalUpdateEntityDepth(int oldDepth_, int newDepth_, BaseEntity *ent_);

        /*
         * Is the scene paused
         */
        bool IsPaused();

        /*
         * Pause the scene
         */
        void Pause();

        /*
         * Unpause the scene
         */
        void Resume();

        /*
         * Set the currently active camera
         */
        void SetActiveCamera(Graphics::Camera *camera_);

        /*
         * Set the entity culling area
         */
        void SetCullArea(float width_, float height_, bool centerOnCamera_);

        /*
         * Update the scene
         */
        void Update();
    };
}

#endif //BASESCENE_H
