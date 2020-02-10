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
#include "graphics/Renderer.hpp"
#include "filesystem/ResourceManager.hpp"
#include "physics/PhysicsSpace.hpp"

namespace ngine {
    class Entity;
    class Game;

    class NEAPI Scene {
        /**
         * The entities within the scene.
         */
        std::vector<Entity *> m_entities;

        /**
         * The game we are a part of.
         */
        Game *m_game;

        /**
         * Cameras that are a part of the scene.
         */
        std::unordered_map<std::string, graphics::Camera> m_cameras;

        /**
         * The currently enabled camera
         */
        std::string m_currentCamera = "default";

        /**
         * The scene physics space.
         */
        physics::PhysicsSpace *m_space = nullptr;

        /**
         * Sort the entities.
         */
        void _sortEntities();
    public:
        /**
         * Create a new scene.
         * Adds a camera named `default`.
         *
         * @param game Parent game.
         * @param physicsEnabled Whether or not physics is enabled.
         */
        Scene(Game *game, bool physicsEnabled = false);
        virtual ~Scene();

        /**
         * Determine if physics is enabled
         */
        bool isPhysicsEnabled() const;

        /**
         * Get the scene physics space.
         *
         * @return The physics space.
         */
        physics::PhysicsSpace *getSpace();

        /**
         * Get the scene physics space.
         *
         * @return The physics space.
         */
        const physics::PhysicsSpace *getSpace() const;

        /**
         * Get the game we are a member of.
         *
         * @return The game.
         */
        Game *getGame() const;

        /**
         * Get the game's resource manager.
         *
         * @return The game resource manager.
         */
        filesystem::ResourceManager *getResourceManager();

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
         * @return A weak reference to the camera or null if the camera doesn't exist.
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
         * Pre-render event.
         * This enables the developer to use render targets and have them drawn before the scene is, allowing entities to use these targets in game.
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
         * By default this will call `preRender()`, `render()`, `postRender()` and `renderUI()`.
         * This could be overridden and instead render two render targets created in preRender which constitute split screen.
         *
         * @param renderer Renderer to use.
         */
        virtual void onRender(graphics::Renderer *renderer);

        /**
         * Post-render event.
         * This enables the developer to apply post-processing effects or something else.
         *
         * This is called (in the default `onRender`) after `render()` and before `postRender()`.
         *
         * @param renderer Renderer to use.
         */
        virtual void postRender(graphics::Renderer *renderer);

        /**
         * Render the scene contents in the current/main camera.
         *
         * This is called (in the default `onRender`) after `preRender()` and before `postRender()`.
         *
         * @param renderer The renderer to use.
         */
        void render(graphics::Renderer *renderer);

        /**
         * Render the scene contents with the given camera.
         * This is useful for rendering to rendertargets.
         *
         * @param renderer The renderer to use.
         * @param camera The name of the camera to use.
         */
        void renderWith(graphics::Renderer *renderer, const std::string &camera);

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
}

#endif //SCENE_HPP
