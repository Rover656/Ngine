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

#include "Physics/PhysicsWorld.h"
#include "Rectangle.h"
#include "Graphics/Camera.h"
#include "EntityContainer.h"
#include "EventHandler.h"

namespace NerdThings::Ngine {
    class Game;

    /**
     * Scene loaded event args.
     */
    struct SceneLoadEventArgs : EventArgs {
        /**
         * The game that the scene has been loaded into.
         */
        Game *ParentGame;

        /**
         * Create a new scene loaded event arg.
         */
        SceneLoadEventArgs(Game *game_)
                : ParentGame(game_) {}
    };

    /**
     * A scene controls every entity.
     * This also contains the active camera, cull areas and manages the overall lifecycle.
     */
    class NEAPI Scene : public EntityContainer {
        /**
         * Currently active camera which controls the viewport
         */
        Graphics::Camera *_ActiveCamera = nullptr;

        /**
         * The culling area width
         */
        float _CullAreaWidth;

        /**
         * The culling area height
         */
        float _CullAreaHeight;

        /**
         * Whether or not to center the cull area in the viewport.
         */
        bool _CullAreaCenterInViewport = true;

        /**
         * Whether or not an entity is active
         */
        std::unordered_map<BaseEntity *, bool> _EntityActivities;

        /**
         * Depth key list containing entities.
         * This is used for drawing.
         */
        std::map<int, std::vector<BaseEntity *>> _EntityDepths;

        /**
         * The parent game.
         */
        Game* _ParentGame = nullptr;

        /**
         * Whether or not the scene is paused.
         */
        bool _Paused = false;

        /**
         * Whether or not physics is enabled.
         */
        bool _PhysicsEnabled = false;

        /**
         * The physics world, if enabled.
         */
        Physics::PhysicsWorld *_PhysicsWorld;

        /**
         * The update counter.
         */
        int _UpdateCounter = 0;

        void ProcessChildAdded(BaseEntity *ent_) override;
        void ProcessChildRemoved(BaseEntity *ent_) override;
    public:
        /**
         * On draw event.
         */
        Event<> OnDraw;

        /**
         * On draw with the camera.
         */
        Event<> OnDrawCamera;

        /**
         * On scene load.
         */
        Event<SceneLoadEventArgs> OnLoad;

        /**
         * On persistent update event.
         * This means updates will run through pauses.
         */
        Event<> OnPersistentUpdate;

        /**
         * On scene unload.
         */
        Event<SceneLoadEventArgs> OnUnLoad;

        /**
         * On update event.
         */
        Event<> OnUpdate;

        /**
         * Create a new Scene.
         *
         * @param parentGame_ The current game.
         * @param physicsEnabled_ Whether or not the game has physics enabled.
         * @param grav_ The physics gravity vector.
         * @param ppm_ The physics pixel to meter ratio.
         */
        Scene(Game* parentGame_, bool physicsEnabled_ = false, Vector2 grav_ = Vector2::Zero, float ppm_ = 1);
        virtual ~Scene();

        /**
         * Draw the scene.
         * Called by the Game.
         */
        void Draw();

        /**
         * Get the currently active camera.
         *
         * @return A pointer to the currently active camera
         */
        Graphics::Camera *GetActiveCamera() const;

        /**
         * Get the culling area.
         *
         * @return The cull area rectangle.
         */
        Rectangle GetCullArea() const;

        /**
         * Get the bottom-right coordinate of the cull area.
         *
         * @return The bottom-right coordinate of the cull area.
         */
        Vector2 GetCullAreaEndPosition() const;

        /**
         * Get cull area height.
         *
         * @return The height of the cull area.
         */
        float GetCullAreaHeight() const;

        /**
         * Get the top-left coordinate of the cull area. (Same as viewport).
         *
         * @return The top-left coordinate of the cull area.
         */
        Vector2 GetCullAreaPosition() const;

        /**
         * Get cull area width.
         *
         * @return The width of the cull area.
         */
        float GetCullAreaWidth() const;

        /**
         * Get the parent game.
         */
        Game *GetParentGame();

        /**
         * Get the current physics world.
         *
         * @return The current physics world.
         */
        Physics::PhysicsWorld *GetPhysicsWorld();

        /**
         * Get the current physics world (read-only).
         *
         * @return (Read-only) The current physics world.
         */
        const Physics::PhysicsWorld *GetPhysicsWorld() const;

        /**
         * Get the current viewport.
         *
         * @return The current viewport taking into account the camera.
         */
        Rectangle GetViewport() const;

        /**
         * Get the bottom right coordinate of the viewport.
         *
         * @return The bottom right coordinate of the viewport.
         */
        Vector2 GetViewportEndPosition() const;

        /**
         * Get the height of the current viewport.
         *
         * @return The height of the viewport.
         */
        float GetViewportHeight() const;

        /**
         * Get the current viewport position.
         *
         * @return The top left coordinate of the viewport.
         */
        Vector2 GetViewportPosition() const;

        /**
         * Get the width of the current viewport.
         *
         * @return The width of the viewport.
         */
        float GetViewportWidth() const;

        /**
         * Set the entity depth in the scene (internally used).
         *
         * @note This is for internal use only.
         * @todo Handle this in a new renderer.
         * @param depth_ The new depth.
         * @param ent_ The entity.
         */
        void InternalSetEntityDepth(int depth_, BaseEntity *ent_);

        /**
         * Update the entity depth in the scene.
         *
         * @note This is for internal use only.
         * @todo Handle this in a new renderer.
         * @param oldDepth_ The old depth.
         * @param newDepth_ The new depth.
         * @param ent_ Entity that has changed.
         */
        void InternalUpdateEntityDepth(int oldDepth_, int newDepth_, BaseEntity *ent_);

        /**
         * Is the scene paused.
         *
         * @return Whether or not the scene is paused.
         */
        bool IsPaused();

        /***
         * Pause the scene.
         */
        void Pause();

        /**
         * Unpause the scene.
         */
        void Resume();

        /**
         * Set the currently active camera.
         *
         * @param camera_ The new camera.
         */
        void SetActiveCamera(Graphics::Camera *camera_);

        /**
         * Set the entity culling area.
         *
         * @param width_ The cull area width.
         * @param height_ The cull area height.
         * @param centerInViewport_ Whether or not to center the cull area in the viewport.
         */
        void SetCullArea(float width_, float height_, bool centerInViewport_);

        /**
         * Update the scene
         */
        void Update();
    };
}

#endif //BASESCENE_H
