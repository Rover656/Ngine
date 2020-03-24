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

#include "filesystem/ResourceManager.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Renderer.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"
#include "physics/PhysicsWorld.hpp"

namespace ngine {
    class Entity;
    class Game;

    /**
     * A scene contains all of your Entities and Cameras and handles the
     * sorting, updating and rendering of them. A scene can represent a level in
     * a game.
     */
    class NEAPI Scene {
        /**
         * The entities within the scene.
         */
        std::vector<Entity *> m_entities;

        /**
         * The game we are a part of.
         */
        Game *m_game = nullptr;

        /**
         * Cameras that are a part of the scene.
         */
        std::unordered_map<std::string, graphics::Camera> m_cameras;

        /**
         * The current camera for `render`
         */
        std::string m_currentCamera;

        /**
         * The internal physics context.
         */
        physics::PhysicsContext *m_physicsContext = nullptr;

        /**
         * The scene physics world.
         */
        physics::PhysicsWorld *m_physicsWorld = nullptr;

        /**
         * Sort the entities.
         */
        void _sortEntities();

    public:
        /**
         * Create a new scene.
         */
        Scene();

        /**
         * Create a new physics-enabled scene.
         *
         * @param gravity Gravity vector.
         * @param ppm Pixels per meter.
         */
        Scene(Vector2 gravity, int ppm);

        virtual ~Scene();

        /**
         * Initialize the scene as we are now active.
         *
         * @param game Game the scene is active in.
         */
        virtual void initialize(Game *game);

        /**
         * Cleanup the scene as it is no longer the active scene.
         */
        virtual void cleanup();

        /**
         * Get the game we are a member of.
         *
         * @return The game.
         */
        Game *getGame() const;

        /**
         * Get the game's keyboard input manager.
         *
         * @return Keyboard input manager
         */
        input::Keyboard *getKeyboard() const;

        /**
         * Get the game's mouse input manager.
         *
         * @return Mouse input manager
         */
        input::Mouse *getMouse() const;

        /**
         * Get the game's resource manager.
         *
         * @return The game resource manager.
         */
        filesystem::ResourceManager *getResourceManager();

        /**
         * Determine if physics is enabled.
         *
         * @return Whether or not physics is enabled in this scene.
         */
        bool isPhysicsEnabled() const;

        /**
         * Get the physics context used for unit conversions.
         *
         * @return Physics context or null if not enabled.
         */
        const physics::PhysicsContext *getPhysicsContext() const;

        /**
         * Get the physics world.
         *
         * @return The physics world or null if not enabled.
         */
        physics::PhysicsWorld *getPhysicsWorld() const;

        /**
         * Create a new camera with the given name.
         *
         * @param name Name of the new camera.
         * @return A weak reference to the new camera.
         */
        graphics::Camera *createCamera(const std::string &name);

        /**
         * Get a weak reference to the requested camera.
         *
         * @param name The camera to get.
         * @return A weak reference to the camera or null if the camera doesn't
         * exist.
         */
        graphics::Camera *getCamera(const std::string &name);

        /**
         * Remove/delete the given camera.
         *
         * @param name The camera to remove.
         */
        void removeCamera(const std::string &name);

        /**
         * Set the currently active camera.
         *
         * @param name The name of the camera to use for main render.
         */
        void setCurrentCamera(const std::string &name);

        /**
         * Add a child entity to the scene.
         *
         * @param entity Entity to add.
         */
        void addChild(Entity *entity);

        /**
         * Remove a child entity from the scene.
         */
        void removeChild(Entity *entity);

        /**
         * Get the first child of name.
         *
         * @param name The child name to find.
         * @return The first child found with the given name.
         */
        Entity *getChildByName(const std::string &name);

        /**
         * Get all children.
         *
         * @return All children.
         */
        std::vector<Entity *> getChildren();

        /**
         * Get all children with the given name.
         *
         * @param name Name to search for.
         * @return All children with the given name.
         */
        std::vector<Entity *> getChildrenByName(const std::string &name);

        /**
         * Get the current viewport.
         *
         * This will return the camera viewport, virtual viewport or finally the window viewport.
         */
        const graphics::Viewport *getViewport();

        /**
         * Pre-render event.
         * This enables the developer to use render targets and have them drawn
         * before the scene is, allowing entities to use these targets in game.
         *
         * This is called (in the default onRender) before everything.
         *
         * @param renderer The renderer to use.
         */
        virtual void preRender(graphics::Renderer *renderer);

        /**
         * On-render event.
         * This defines what happens when the scene is to be rendered.
         *
         * By default this will call `preRender()`, `render()`, `postRender()`
         * and `renderUI()`. This could be overridden and instead render two
         * render targets created in preRender which constitute split screen.
         *
         * @param renderer Renderer to use.
         */
        virtual void renderScene(graphics::Renderer *renderer);

        /**
         * Post-render event.
         * This enables the developer to apply post-processing effects or
         * something else.
         *
         * This is called (in the default `onRender`) after `render()` and
         * before `postRender()`.
         *
         * @param renderer Renderer to use.
         */
        virtual void postRender(graphics::Renderer *renderer);

        /**
         * Render the scene contents in the current/main camera.
         *
         * This is called (in the default `onRender`) after `preRender()` and
         * before `postRender()`.
         *
         * @param renderer The renderer to use.
         */
        void renderDefaultView(graphics::Renderer *renderer);

        /**
         * Render the scene contents with the given camera.
         * This is useful for rendering to rendertargets.
         *
         * @param renderer The renderer to use.
         * @param camera The name of the camera to use.
         */
        void renderUsingCamera(graphics::Renderer *renderer,
                               const std::string &camera);

        /**
         * Render all UI active in the scene.
         *
         * @param renderer The renderer to use.
         */
        void renderUI(graphics::Renderer *renderer);

        /**
         * Update all entities in the scene.
         */
        void update();

        /**
         * Run update logic for UI.
         */
        void updateUI();
    };
} // namespace ngine

#endif // SCENE_HPP
