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
#include "physics/PhysicsBody.hpp"
#include "EntityContainer.hpp"
#include "Math.hpp"
#include "Scene.hpp"

// TODO: Proper way to handle rotation from a components point of view. Need to have entity centers and ways to rotate both with the entity and independently.

namespace ngine {
    // Forward declare
    class Component;

    /**
     * Event arguments for when an entity changes position and/or rotation.
     */
    struct EntityTransformChangedEventArgs : EventArgs {
        /**
         * The new entity transformation.
         */
        Transform2D EntityTransformation;

        /**
         * Create a new entity transform event argument.
         *
         * @param transform_ The new entity transform.
         */
        EntityTransformChangedEventArgs(Transform2D transform_)
                : EntityTransformation(transform_) {}
    };

    /**
     * An entity within a scene.
     */
    class NEAPI Entity : public EntityContainer {
        // Make the EntityContainer a friend so parenting can be done
        friend class EntityContainer;
        friend class Scene;

        /**
         * This entity's transformation.
         */
        Transform2D m_transform;

        /**
         * Whether or not this entity can be culled
         */
        bool m_canCull = false;

        /**
         * The list of all components.
         * All components are given a name for easy identification.
         */
        std::map<std::string, Component *> m_components;

        /**
         * Depth index.
         */
        int m_depth = 0;

        /**
         * On update event reference
         */
        EventAttachment<> m_onUpdateRef;

        /**
         * The parent entity.
         */
        Entity *m_parentEntity = nullptr;

        /**
         * Parent Scene.
         */
        Scene *m_parentScene = nullptr;

        /**
         * Whether or not we update when paused
         */
        bool m_persistentUpdates = false;

        /**
         * Whether or not the entity has physics enabled.
         */
        bool m_physicsEnabled = false;

        /**
         * The current physics body.
         */
        physics::PhysicsBody *m_physicsBody = nullptr;

        /**
         * Add to a new scene.
         */
        void _addToScene(Scene *scene_);

        /**
         * Remove from our current scene.
         */
        void _removeFromScene();

        /**
         * Destroy this entity.
         */
        void _doDestroy();
    public:
        /**
         * Whether or not this entity is drawn with the camera
         */
        bool DrawWithCamera = true;

        /**
         * Fires when added to a scene.
         * Entity can be initialized once this is fired, much like Game.
         */
        Event<> OnInit;

        /**
         * Fires when the entity is destroyed.
         */
        Event<> OnDestroy;

        /**
         * On position changed event
         */
        Event<EntityTransformChangedEventArgs> OnTransformChanged;

        /**
         * Fired when the entity updates.
         * This should be used by components.
         */
        Event<> OnUpdate;

        /**
         * Create a new entity.
         *
         * @param parentScene_ The parent scene.
         * @param position_ The initial position.
         * @param rotation_ Initial rotation.
         * @param depth_ The depth to be rendered at
         * @param canCull_ Whether or not this can be culled.
         * @param physicsEnabled_ Whether or not physics should be enabled for this entity.
         */
        Entity(Vector2 position_, float rotation_ = 0, int depth_ = 0, bool canCull_ = false, bool physicsEnabled_ = false);
        virtual ~Entity();

        /**
         * Remove from our parent and delete ourselves.
         *
         * @warning No more calls must be made to the entity following this.
         */
        void destroy();

        /**
         * Add a component to the entity.
         *
         * @tparam ComponentType The component type. This must be a class derived from `ngine::Component`
         * @param name_ The name of the component.
         * @param component_ The component.
         * @return The component, so you may chain calls.
         */
        template <typename ComponentType = Component>
        ComponentType *addComponent(const std::string &name_, ComponentType *component_) {
            // Check the name is not taken
            if (hasComponent(name_))
                return nullptr;

            // Make sure the component isn't null
            if (component_ == nullptr)
                return nullptr;

            // Add to components list.
            m_components.insert({name_, (Component *) component_});

            // Send it back for call chaining.
            return component_;
        }

        /**
         * Removes the component from the entity.
         *
         * @param name_ The name of the component to remove.
         * @return Whether or not the component was removed.
         */
        bool removeComponent(const std::string &name_);

        /**
         * Get all components.
         *
         * @return An vector containing all components.
         */
        std::vector<Component*> getComponents();

        /**
         * Test if a component exists by a name.
         *
         * @param name_ The name of the component to check for.
         * @return Whether or not the component exists.
         */
        bool hasComponent(const std::string &name_) const;

        /**
         * Get a component by name.
         *
         * @tparam ComponentType The component type to be returned.
         */
        template <typename ComponentType = Component>
        ComponentType *getComponent(const std::string &name_) {
            // Try to find the component
            if (hasComponent(name_)) {
                return (ComponentType *) m_components.at(name_);
            }

            return nullptr;
        }

        /**
         * Check if this entity can be culled
         *
         * @return Whether or not this entity can be culled.
         */
        bool canCull() const;

        /**
         * Set whether or not this entity can be culled
         *
         * @param canCull_ Whether or not the entity can be culled.
         */
        void setCanCull(bool canCull_);

        /**
         * This is used to determine if this entity should be culled.
         * This can be overridden for more accurate results.
         *
         * @param cullArea_ The scene cull area.
         * @return Whether or not this entity should be culled.
         */
        virtual bool checkForCulling(Rectangle cullArea_);

        /**
         * Get our parent container.
         * This will either be another entity or the scene
         *
         * @return The parent container.
         */
        EntityContainer *getContainer() const;

        /**
         * Get the parent entity.
         *
         * @tparam EntityType The type to cast the parent to (Default: `Entity`)
         * @return The parent entity, null if none.
         */
        template <typename EntityType = Entity>
        Entity *getParentEntity() const {
            return (EntityType *)(m_parentEntity);
        }

        /**
         * Get the parent scene.
         *
         * @tparam SceneType The type we want the scene as.
         * @return The parent scene.
         */
        template <class SceneType = Scene>
        SceneType *getScene() const {
            return (SceneType *) m_parentScene;
        }

        /**
         * Get the parent game.
         *
         * @return The parent game.
         */
        Game *getGame() const;

        /**
         * Get the game resource manager.
         *
         * @return The game resource manager.
         */
        filesystem::ResourceManager *getResourceManager() const;

        /**
         * Get the entity depth.
         *
         * @return The depth of the entity.
         */
        int getDepth() const;

        /**
         * Set the entity depth
         *
         * @param depth_ The depth to set the entity to.
         */
        void setDepth(int depth_);

        // MAJOR TODO: Add system for entity origin when not attached to physics body.

        /**
         * Get the entity transform (position and rotation).
         *
         * @return The entity transform.
         */
        Transform2D getTransform() const;

        /**
         * Set the entity transform (position and rotation).
         */
        void setTransform(const Transform2D &transform_);

        /**
         * Get the entity position
         *
         * @return The position of the entity in world coordinates.
         */
        Vector2 getPosition() const;

        /**
         * Set entity position
         *
         * @param position_ The position to move to.
         */
        void setPosition(Vector2 position_);

        /**
         * Move an entity by a vector.
         *
         * @warning Use of this in a physics enabled entity is not recommended, add a velocity instead.
         * @param moveBy_ The number of pixels to move in each direction.
         */
        void moveBy(Vector2 moveBy_);

        /**
         * Get the entity rotation
         *
         * @return The entity rotation, in degrees.
         */
        Angle getRotation() const;

        /**
         * Set entity rotation.
         *
         * @param rotation_ The rotation, in degrees, to set our entity to.
         */
        void setRotation(Angle rotation_);

        /**
         * Determine if this entity is affected by physics.
         * This is determined by the physicsEnabled_ parameter in the entity constructor.
         *
         * @note Once an entity is created, this property *can not* be changed.
         * @return Whether or not this entity has a physics body attached.
         */
        bool isPhysicsEnabled() const;

        /**
         * Get the attached physics body.
         *
         * @return The physics body. If none, null.
         */
        physics::PhysicsBody *getPhysicsBody();

        /**
         * Get the attached physics body.
         *
         * @return The physics body. If none, null.
         */
        const physics::PhysicsBody *getPhysicsBody() const;

        /**
         * Get the world our body is a member of.
         *
         * @return The world the physics body is attached to, if any.
         */
        physics::PhysicsWorld *getPhysicsWorld();

        /**
         * Get the world our body is a member of.
         *
         * @return The world the physics body is attached to, if any.
         */
        const physics::PhysicsWorld *getPhysicsWorld() const;

        /**
         * Test if we update when the scene is paused.
         *
         * @return Whether or not we will update even when the scene is paused.
         */
        bool getDoPersistentUpdates() const;

        /**
         * Set whether or not we update when the `Scene` is paused
         *
         * @note This will unsubscribe and resubscribe if the events are already bound.
         * @param persistentUpdates_ Whether or not the entity can update during pauses.
         */
        void setDoPersistentUpdates(bool persistentUpdates_);

        /**
         * Subscribe to update events in the scene.
         *
         * @warning If the entity changes parent this will need to be called again.
         * @return Whether or the entity now listens to updates.
         */
        bool subscribeToUpdate();

        /**
         * Determine if the entity is subscribed to the `Scene` update events.
         *
         * @return Whether or not the entity is subscribed to `Scene` updates.
         */
        bool subscribedToUpdate() const;

        /**
         * Unsubscribe from update events in the scene.
         */
        void unsubscribeFromUpdate();

        /**
         * Draw code for the entity.
         *
         * @param renderer_ The game renderer.
         */
        virtual void draw(graphics::Renderer *renderer_);

        /**
         * Update the entity.
         */
        virtual void update();
    };
}

#endif //ENTITY_HPP
