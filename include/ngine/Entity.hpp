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

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Config.hpp"

#include "filesystem/ResourceManager.hpp"
#include "graphics/Renderer.hpp"
#include "input/Mouse.hpp"
#include "input/Keyboard.hpp"
#include "physics/PhysicsBody.hpp"
#include "Scene.hpp"

namespace ngine {
    class Component;

    /**
     * This is the basic unit for the entity-component system.
     */
    class NEAPI Entity {
        friend class Scene;

        /**
         * Weak ref to parent entity.
         */
        Entity *m_parent = nullptr;

        /**
         * The scene that the entity is a member of.
         */
        Scene *m_scene = nullptr;

        /**
         * The child entities.
         */
        std::vector<Entity *> m_children;

        /**
         * Entity components.
         */
        std::unordered_map<std::string, Component *> m_components;

        /**
         * Entity Z-index in parent.
         */
        int m_zIndex = 0;

        /**
         * The entity name.
         */
        std::string m_name = "";

        /**
         * The entity name hash (for quick searching).
         */
        size_t m_nameHash = 0;

        /**
         * Whether or not the entity is updating.
         */
        bool m_asleep = false;

        /**
         * Whether or not the entity should render.
         */
        bool m_visible = true;

        /**
         * Whether or not the entity has been destroyed yet.
         */
        bool m_destroyed = false;

        /**
         * Entity position relative to parent.
         */
        Vector2 m_position = Vector2::Zero;

        /**
         * Entity scale.
         */
        Vector2 m_scale = {1, 1};

        /**
         * Entity rotation.
         */
        float m_rotation = 0;

        /**
         * The model view matrix to render with.
         */
        Matrix m_modelView = Matrix::Identity;

        /**
         * Whether or not the model view is dirty.
         */
        bool m_modelViewDirty = true;

        // TODO: Track what coordinate system was used

        /**
         * Whether or not the entity is ready to be used.
         */
        bool m_initialized = false;

        /**
         * The physics body.
         */
        physics::PhysicsBody *m_physicsBody = nullptr;

        /**
         * Sort entities by z-index.
         *
         * @param a The first entity.
         * @param b The second entity.
         * @return Whether or not a should be before b.
         */
        static bool _SortChildrenPredicate(Entity *a, Entity *b);

        /**
         * Sort all children into Z order for rendering.
         */
        void _sortChildren();

        /**
         * Update our model view.
         */
        void _updateModelView(graphics::CoordinateSystem coordSys);
    public:
        /**
         * Create a new entity.
         */
        Entity();

        virtual ~Entity();

        /**
         * Initialize the entity.
         *
         * @warning If you override this, you **MUST** call to base first.
         * @param scene The scene we were added to.
         */
        virtual void initialize(Scene *scene);

        /**
         * Cleanup the entity.
         *
         * @warning If you override this, you **MUST** call to base first.
         */
        virtual void cleanup();

        /**
         * Destroy this entity.
         */
        void destroy();

        /**
         * Get the parent entity.
         *
         * @return Our parent.
         */
        Entity *getParent() const;

        /**
         * Get the scene we are a member of.
         *
         * @return The scene.
         */
        Scene *getScene() const;

        /**
         * Get the game our scene is a member of.
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
         * Get the game resource manager.
         *
         * @return The resource manager.
         */
        filesystem::ResourceManager *getResourceManager() const;

        /**
         * Get the entity center position taking into account its parents.
         *
         * @param Position relative to (0, 0).
         */
        Vector2 getPosition() const;

        /**
         * Set the entity center position relative to the parent.
         *
         * @param position Desired position relative to parent.
         */
        void setPosition(Vector2 position);

        /**
         * Get the current entity rotation.
         *
         * @return The entity's rotation.
         */
        float getRotation() const;

        /**
         * Set the entity's rotation.
         *
         * @param rotation The new rotation.
         */
        void setRotation(float rotation);

        /**
         * Get the entity scale.
         *
         * @return The entity X and Y scales.
         */
        Vector2 getScale() const;

        /**
         * Set the entity scale.
         *
         * @note This scale also applies to children.
         * @param scale The new scale to apply.
         */
        void setScale(Vector2 scale);

        /**
         * Get the entity's physics body.
         *
         * @return The physics body or null if one is not present.
         */
        physics::PhysicsBody *getPhysicsBody();

        /**
         * Set the entity's physics body.
         *
         * @param body The new physics body.
         */
        void setPhysicsBody(physics::PhysicsBody *body);

        /**
         * Get the z-index of the entity.
         *
         * @return The z-index.
         */
        int getZIndex() const;

        /**
         * Set the z-index of the entity.
         *
         * @param zIndex The new z-index.
         */
        void setZIndex(int zIndex);

        /**
         * Add a component to the entity.
         *
         * @param name Component name.
         * @param component Component to add.
         */
        void addComponent(const std::string &name, Component *component);

        /**
         * Remove a component from the entity.
         *
         * @param name Component to remove.
         */
        void removeComponent(const std::string &name);

        /**
         * Remove a component from the entity by pointer.
         *
         * @param component Component to remove.
         */
        void removeComponent(Component *component);

        /**
         * Get a component by name.
         *
         * @param name Component to get.
         * @return The component or null.
         */
        Component *getComponent(const std::string &name);

        /**
         * Get a component by name casted to the required type.
         *
         * @tparam TComponent The type wanted.
         * @param Component to get.
         * @return The component or null.
         */
        template <class TComponent>
        TComponent *getComponentAs(const std::string &name) {
            return (TComponent *)getComponent(name);
        }

        /**
         * Whether or not the entity is asleep (not running updates).
         *
         * @note This will also make the physics body sleep.
         * @return Whether or not the entity is sleeping.
         */
        bool isAsleep();

        /**
         * Set whether or not the entity is asleep.
         *
         * @param asleep Whether or not the entity should sleep.
         */
        void setIsAsleep(bool asleep);

        /**
         * Determine if the entity is visible.
         *
         * @return Whether or not the entity is drawn.
         */
        bool isVisible();

        /**
         * Set whether or not the entity should be visible/drawn.
         *
         * @param visible Whether or not the entity should draw.
         */
        void setIsVisible(bool visible);

        /**
         * Get the entity name.
         *
         * @return Entity name.
         */
        std::string getName();

        /**
         * Set the entity name.
         *
         * @param name Entity name.
         */
        void setName(const std::string &name);

        /**
         * Add a child.
         *
         * @param entity Entity to add as a child.
         */
        void addChild(Entity *entity);

        /**
         * Remove the given child from this entity.
         *
         * @param entity The entity to remove.
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
         * Detemine if this entity is visible in the given camera.
         *
         * @note By default, this will check the size fields for a non-physics shape, and the physics body's AABB for a physics shape.
         * @param camera Camera we are checking with.
         * @return Whether or not we are visible in the camera.
         */
        virtual bool isVisibleInCamera(graphics::Camera *camera);

        /**
         * Render this entity and its children with the given renderer and model view matrix.
         *
         * @param renderer The renderer.
         * @param modelView The current modelview (if the scene calls this, it'll be the camera's view matrix).
         * @param currentCamera The current camera or null if a camera is not used.
         */
        void render(graphics::Renderer *renderer, Matrix modelView, graphics::Camera *currentCamera = nullptr);

        /**
         * Draw the contents of the entity.
         *
         * @note All rendering in this method is ***RELATIVE*** to the entity already. So rendering something at (0, 0) will render at the entity itself.
         * @param renderer The renderer.
         * @param modelView The currently applied modelview, should only be used for reference/converting of coordinates.
         */
        virtual void draw(graphics::Renderer *renderer, Matrix modelView);

        /**
         * Run update logic for the entity's components and children.
         */
        virtual void update();
    };
}

#endif //ENTITY_HPP
