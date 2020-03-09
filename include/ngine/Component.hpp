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

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Config.hpp"

#include "filesystem/ResourceManager.hpp"
#include "graphics/Renderer.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"

namespace ngine {
    class Entity;
    class Game;
    class Scene;

    /**
     * A component attaches to an Entity and defines/manipulates its behaviour
     * and appearance.
     */
    class NEAPI Component {
        friend class Entity;

        /**
         * The entity we are attached to.
         */
        Entity *m_parent = nullptr;

        /**
         * Whether or not we are initializsed.
         */
        bool m_initialized = false;

        /**
         * Whether or not delete has been called yet.
         */
        bool m_destroyed = false;

    protected:
        /**
         * Create a new component
         */
        Component();

    public:
        virtual ~Component();

        /**
         * Initialize the component
         *
         * @warning If you override this, you **MUST** call to base first.
         * @param parent The parent entity.
         */
        virtual void initialize(Entity *parent);

        /**
         * Cleanup/deinitialize the component.
         *
         * @warning If you override this, you **MUST** call to base first.
         */
        virtual void cleanup();

        /**
         * Get the component's parent entity.
         *
         * @return The parent entity.
         */
        Entity *getParent() const;

        /**
         * Get the scene our parent is a member of.
         *
         * @return The scene.
         */
        Scene *getScene() const;

        /**
         * Get the game our member's scene is a member of.
         *
         * @return The scene.
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
         * Return the game's resource manager.
         *
         * @return The resource manager.
         */
        filesystem::ResourceManager *getResourceManager() const;

        /**
         * Draw the component
         *
         * @note Remember, drawing happens relative to the entity, so drawing a
         * circle at (0, 0) draws it right on the entity!
         * @param renderer Renderer to draw with.
         */
        virtual void draw(graphics::Renderer *renderer);

        /**
         * Run update logic for the component.
         */
        virtual void update();
    };
} // namespace ngine

#endif // COMPONENT_HPP
