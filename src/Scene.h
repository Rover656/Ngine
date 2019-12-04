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

#include "Graphics/Camera.h"
#include "Physics/PhysicsWorld.h"
#include "EntityContainer.h"
#include "Events.h"
#include "Rectangle.h"

namespace NerdThings::Ngine {
    // Forward declare
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
        // EntityContainer needs private access
        friend class EntityContainer;
        friend class BaseEntity;

        /**
         * Currently active camera which controls the viewport
         */
        Graphics::Camera *m_activeCamera = nullptr;

        /**
         * The culling area width
         */
        float m_cullAreaWidth;

        /**
         * The culling area height
         */
        float m_cullAreaHeight;

        /**
         * Whether or not to center the cull area in the viewport.
         */
        bool m_cullAreaCenterInViewport = true;

        /**
         * Whether or not an entity is active.
         *
         * @todo Make this a part of `EntityContainer`?
         */
        std::unordered_map<BaseEntity *, bool> m_entityActivities;

        /**
         * Depth key list containing entities.
         * This is used for drawing.
         *
         * @todo Make this a part of `EntityContainer`?
         */
        std::map<int, std::vector<BaseEntity *>> m_entityDepths;

        /**
         * The parent game.
         */
        Game* m_parentGame = nullptr;

        /**
         * Whether or not the scene is paused.
         */
        bool m_paused = false;

        /**
         * Whether or not physics is enabled.
         */
        bool m_physicsEnabled = false;

        /**
         * The physics world, if enabled.
         */
        Physics::PhysicsWorld *m_physicsWorld;

        /**
         * The update counter.
         */
        int m_updateCounter = 0;

        /**
         * Add an entity for tracking.
         *
         * @param ent_ The entity to track.
         */
        void _addEntity(BaseEntity *ent_);

        /**
         * Remove an entity from tracking.
         *
         * @param ent_ Entity to stop tracking.
         */
        void _removeEntity(BaseEntity *ent_);

        /**
         * Track an entity depth value update.
         *
         * @param newDepth_ The entitiy's new depth.
         * @param ent_ The entity.
         */
        void _updateEntityDepth(int newDepth_, BaseEntity *ent_);
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
         * On update event.
         */
        Event<> OnUpdate;

        /**
         * On persistent update event.
         * This means updates will run through pauses.
         */
        Event<> OnPersistentUpdate;

        /**
         * On scene load.
         */
        Event<SceneLoadEventArgs> OnLoad;

        /**
         * On scene unload.
         */
        Event<SceneLoadEventArgs> OnUnLoad;

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
         * Get the currently active camera.
         *
         * @return A pointer to the currently active camera
         */
        Graphics::Camera *GetActiveCamera() const;

        /**
         * Set the currently active camera.
         *
         * @param camera_ The new camera.
         */
        void SetActiveCamera(Graphics::Camera *camera_);

        /**
         * Get the culling area.
         *
         * @return The cull area rectangle.
         */
        Rectangle GetCullArea() const;

        /**
         * Get the top-left coordinate of the cull area. (Same as viewport).
         *
         * @return The top-left coordinate of the cull area.
         */
        Vector2 GetCullAreaPosition() const;

        /**
         * Get the bottom-right coordinate of the cull area.
         *
         * @return The bottom-right coordinate of the cull area.
         */
        Vector2 GetCullAreaEndPosition() const;

        /**
         * Get cull area width.
         *
         * @return The width of the cull area.
         */
        float GetCullAreaWidth() const;

        /**
         * Get cull area height.
         *
         * @return The height of the cull area.
         */
        float GetCullAreaHeight() const;

        /**
         * Set the entity culling area.
         *
         * @param width_ The cull area width.
         * @param height_ The cull area height.
         * @param centerInViewport_ Whether or not to center the cull area in the viewport.
         */
        void SetCullArea(float width_, float height_, bool centerInViewport_);

        /**
         * Get the current viewport.
         *
         * @return The current viewport taking into account the camera.
         */
        Rectangle GetViewport() const;

        /**
         * Get the current viewport position.
         *
         * @return The top left coordinate of the viewport.
         */
        Vector2 GetViewportPosition() const;

        /**
         * Get the bottom right coordinate of the viewport.
         *
         * @return The bottom right coordinate of the viewport.
         */
        Vector2 GetViewportEndPosition() const;

        /**
         * Get the width of the current viewport.
         *
         * @return The width of the viewport.
         */
        float GetViewportWidth() const;

        /**
         * Get the height of the current viewport.
         *
         * @return The height of the viewport.
         */
        float GetViewportHeight() const;

        /**
         * Get the parent game.
         */
        Game *GetGame();

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
         * Pause the scene.
         */
        void Pause();

        /**
         * Unpause the scene.
         */
        void Resume();

        /**
         * Is the scene paused.
         *
         * @return Whether or not the scene is paused.
         */
        bool IsPaused();

        /**
         * Draw the scene.
         *
         * @warning Should only be called by the game.
         */
        void Draw();

        /**
         * Update the scene.
         *
         * @warning Should only be called by the game.
         */
        void Update();
    };
}

#endif //BASESCENE_H
