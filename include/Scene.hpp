/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Config.hpp"

#include "graphics/Camera.hpp"
#include "filesystem/ResourceManager.hpp"
#include "physics/PhysicsWorld.hpp"
#include "EntityContainer.hpp"
#include "Events.hpp"
#include "Rectangle.hpp"

namespace ngine {
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
        friend class EntityContainer;
        friend class Entity;

        /**
         * Currently active camera which controls the viewport
         */
        graphics::Camera *m_activeCamera = nullptr;

        /**
         * The culling area width.
         */
        float m_cullAreaWidth;

        /**
         * The culling area height.
         */
        float m_cullAreaHeight;

        /**
         * Whether or not to center the cull area in the viewport.
         */
        bool m_cullAreaCenterInViewport = true;

        /**
         * Whether or not an entity is active.
         */
        std::unordered_map<Entity *, bool> m_entityActivities;

        /**
         * Depth key list containing entities.
         * This is used for drawing.
         */
        std::map<int, std::vector<Entity *>> m_entityDepths;

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
        physics::PhysicsWorld *m_physicsWorld;

        /**
         * The update counter.
         */
        int m_updateCounter = 0;

        /**
         * Add an entity for tracking.
         *
         * @param ent_ The entity to track.
         */
        void _addEntity(Entity *ent_);

        /**
         * Remove an entity from tracking.
         *
         * @param ent_ Entity to stop tracking.
         */
        void _removeEntity(Entity *ent_);

        /**
         * Track an entity depth value update.
         *
         * @param newDepth_ The entitiy's new depth.
         * @param ent_ The entity.
         */
        void _updateEntityDepth(int newDepth_, Entity *ent_);
    public:
        /**
         * Fired when the game loads the scene.
         * All scene setup should be done here.
         */
        Event<SceneLoadEventArgs> OnInit;

        /**
         * Fired when the game unloads the scene.
         * All scene cleanup should be done here.
         */
        Event<SceneLoadEventArgs> OnUnload;

        /**
         * On draw event.
         */
        Event<graphics::Renderer *> OnDraw;

        /**
         * On draw with the camera.
         */
        Event<graphics::Renderer *> OnDrawCamera;

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
         * Create a standard scene.
         *
         * @param parentGame_ The current game.
         */
        explicit Scene(Game *parentGame_);

        /**
         * Create a new Scene for physics use.
         *
         * @param parentGame_ The current game.
         * @param physicsEnabled_ Whether or not the game has physics enabled.
         * @param grav_ The physics gravity vector.
         * @param ppm_ The physics pixel to meter ratio.
         */
        Scene(Game* parentGame_, Vector2 grav_, float ppm_ = 1);
        virtual ~Scene();

        /**
         * Get the currently active camera.
         *
         * @return A pointer to the currently active camera
         */
        graphics::Camera *getActiveCamera() const;

        /**
         * Set the currently active camera.
         *
         * @param camera_ The new camera.
         */
        void setActiveCamera(graphics::Camera *camera_);

        /**
         * Get the culling area.
         *
         * @return The cull area rectangle.
         */
        Rectangle getCullArea() const;

        /**
         * Get the top-left coordinate of the cull area. (Same as viewport).
         *
         * @return The top-left coordinate of the cull area.
         */
        Vector2 getCullAreaPosition() const;

        /**
         * Get the bottom-right coordinate of the cull area.
         *
         * @return The bottom-right coordinate of the cull area.
         */
        Vector2 getCullAreaEndPosition() const;

        /**
         * Get cull area width.
         *
         * @return The width of the cull area.
         */
        float getCullAreaWidth() const;

        /**
         * Get cull area height.
         *
         * @return The height of the cull area.
         */
        float getCullAreaHeight() const;

        /**
         * Set the entity culling area.
         *
         * @param width_ The cull area width.
         * @param height_ The cull area height.
         * @param centerInViewport_ Whether or not to center the cull area in the viewport.
         */
        void setCullArea(float width_, float height_, bool centerInViewport_);

        /**
         * Get the current viewport.
         *
         * @return The current viewport taking into account the camera.
         */
        Rectangle getViewport() const;

        /**
         * Get the current viewport position.
         *
         * @return The top left coordinate of the viewport.
         */
        Vector2 getViewportPosition() const;

        /**
         * Get the bottom right coordinate of the viewport.
         *
         * @return The bottom right coordinate of the viewport.
         */
        Vector2 getViewportEndPosition() const;

        /**
         * Get the width of the current viewport.
         *
         * @return The width of the viewport.
         */
        float getViewportWidth() const;

        /**
         * Get the height of the current viewport.
         *
         * @return The height of the viewport.
         */
        float getViewportHeight() const;

        /**
         * Get the parent game.
         */
        Game *getGame();

        /**
         * Get the game resource manager.
         *
         * @return The game resource manager.
         */
        filesystem::ResourceManager *getResourceManager() const;

        /**
         * Get the current physics world.
         *
         * @return The current physics world.
         */
        physics::PhysicsWorld *getPhysicsWorld();

        /**
         * Get the current physics world (read-only).
         *
         * @return (Read-only) The current physics world.
         */
        const physics::PhysicsWorld *getPhysicsWorld() const;

        /**
         * Pause the scene.
         */
        void pause();

        /**
         * Unpause the scene.
         */
        void resume();

        /**
         * Is the scene paused.
         *
         * @return Whether or not the scene is paused.
         */
        bool isPaused();

        /**
         * Draw the scene.
         *
         * @warning Should only be called by the game.
         * @param renderer_ The game renderer.
         */
        void draw(graphics::Renderer *renderer_);

        /**
         * Update the scene.
         *
         * @warning Should only be called by the game.
         */
        void update();
    };
}

#endif //SCENE_HPP
