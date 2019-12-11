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

#ifndef ENTITY_H
#define ENTITY_H

// Include ngine
#include "Ngine.h"

#include "Physics/PhysicsBody.h"
#include "Vector2.h"
#include "EntityContainer.h"
#include "Scene.h"

namespace NerdThings::Ngine {
    // Forward declare
    class Component;

    /**
     * Event arguments for when an entity changes position and/or rotation.
     */
    struct EntityTransformChangedEventArgs : EventArgs {
        /**
         * The new entity position
         */
        Vector2 EntityPosition;

        /**
         * The new rotation
         */
        float EntityRotation;

        /**
         * Create a new entity transform event argument.
         *
         * @param pos_ The new entity position.
         * @param rot_ The new entity rotation.
         */
        EntityTransformChangedEventArgs(Vector2 pos_, float rot_)
                : EntityPosition(pos_), EntityRotation(rot_) {}
    };

    /**
     * An entity within a scene.
     */
    class NEAPI Entity : public EntityContainer {
        // Make the EntityContainer a friend so parenting can be done
        friend class EntityContainer;
        friend class Scene;

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
         * The entity position
         */
        Vector2 m_position = Vector2::Zero;

        /**
         * The entity rotation (in radians)
         */
        float m_rotation = 0;

        /**
         * The current physics body.
         */
        Physics::PhysicsBody *m_physicsBody = nullptr;

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
         * Fired when the entity draws.
         * This should be used by components.
         */
        Event<> OnDraw;

        /**
         * Fired when the entity updates.
         * This should be used by components.
         */
        Event<> OnUpdate;

        /**
         * Create a new entity.
         *
         * @param parentScene_ The parent scene.
         * @param position_ The initial position
         * @param depth_ The depth to be rendered at
         * @param canCull_ Whether or not this can be culled.
         */
        Entity(Vector2 position_, int depth_ = 0, bool canCull_ = false);
        virtual ~Entity();

        /**
         * Remove from our parent and delete ourselves.
         *
         * @warning No more calls must be made to the entity following this.
         */
        void Destroy();

        /**
         * Add a component to the entity.
         *
         * @tparam ComponentType The component type. This must be a class derived from `NerdThings::Ngine::Component`
         * @param name_ The name of the component.
         * @param component_ The component.
         * @return The component, so you may chain calls.
         */
        template <typename ComponentType = Component>
        ComponentType *AddComponent(const std::string &name_, ComponentType *component_) {
            // Check the name is not taken
            if (HasComponent(name_))
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
        bool RemoveComponent(const std::string &name_);

        /**
         * Get all components.
         *
         * @return An vector containing all components.
         */
        std::vector<Component*> GetComponents();

        /**
         * Test if a component exists by a name.
         *
         * @param name_ The name of the component to check for.
         * @return Whether or not the component exists.
         */
        bool HasComponent(const std::string &name_) const;

        /**
         * Get a component by name.
         *
         * @tparam ComponentType The component type to be returned.
         */
        template <typename ComponentType = Component>
        ComponentType *GetComponent(const std::string &name_) {
            // Try to find the component
            if (HasComponent(name_)) {
                return (ComponentType *) m_components.at(name_);
            }

            return nullptr;
        }

        /**
         * Check if this entity can be culled
         *
         * @return Whether or not this entity can be culled.
         */
        bool CanCull() const;

        /**
         * Set whether or not this entity can be culled
         *
         * @param canCull_ Whether or not the entity can be culled.
         */
        void SetCanCull(bool canCull_);

        /**
         * This is used to determine if this entity should be culled.
         * This can be overridden for more accurate results.
         *
         * @param cullArea_ The scene cull area.
         * @return Whether or not this entity should be culled.
         */
        virtual bool CheckForCulling(Rectangle cullArea_);

        /**
         * Get our parent container.
         * This will either be another entity or the scene
         *
         * @return The parent container.
         */
        EntityContainer *GetContainer() const;

        /**
         * Get the parent entity.
         *
         * @tparam EntityType The type to cast the parent to (Default: `Entity`)
         * @return The parent entity, null if none.
         */
        template <typename EntityType = Entity>
        Entity *GetParentEntity() const {
            return (EntityType *)(m_parentEntity);
        }

        /**
         * Get the parent scene.
         *
         * @tparam SceneType The type we want the scene as.
         * @return The parent scene.
         */
        template <class SceneType = Scene>
        SceneType *GetScene() const {
            return (SceneType *) m_parentScene;
        }

        /**
         * Get the parent game.
         *
         * @return The parent game.
         */
        Game *GetGame() const;

        /**
         * Get the entity depth.
         *
         * @return The depth of the entity.
         */
        int GetDepth() const;

        /**
         * Set the entity depth
         *
         * @param depth_ The depth to set the entity to.
         */
        void SetDepth(int depth_);

        /**
         * Get the entity position
         *
         * @return The position of the entity in world coordinates.
         */
        Vector2 GetPosition() const;

        /**
         * Set entity position
         *
         * @param position_ The position to move to.
         */
        void SetPosition(Vector2 position_);

        /**
         * Move an entity by a vector.
         *
         * @param moveBy_ The number of pixels to move in each direction.
         * @warning If this is a physics entity, use the physics body to apply a velocity instead.
         */
        void MoveBy(Vector2 moveBy_);

        /**
         * Get the entity rotation
         *
         * @return The entity rotation, in degrees.
         */
        float GetRotation() const;

        /**
         * Set entity rotation.
         *
         * @param rotation_ The rotation, in degrees, to set our entity to.
         */
        void SetRotation(float rotation_);

        /**
         * Determine if this entity is affected by physics.
         *
         * @return Whether or not this entity has a physics body attached.
         */
        bool IsPhysicsEntity() const;

        /**
         * Get the attached physics body.
         *
         * @return The physics body. If none, null.
         */
        Physics::PhysicsBody *GetPhysicsBody();

        /**
         * Get the attached physics body.
         *
         * @return The physics body. If none, null.
         */
        const Physics::PhysicsBody *GetPhysicsBody() const;

        /**
         * Set the entity physics body.
         *
         * @warning You cannot add a body until the entity has been added to a `Scene`.
         * @param body_ The body to add to this entity.
         * @note The lifecycle of the body will now be managed by this entity, even if another body is set.
         */
        void SetPhysicsBody(Physics::PhysicsBody *body_);

        /**
         * Get the world our body is a member of.
         *
         * @return The world the physics body is attached to, if any.
         */
        Physics::PhysicsWorld *GetPhysicsWorld();

        /**
         * Get the world our body is a member of.
         *
         * @return The world the physics body is attached to, if any.
         */
        const Physics::PhysicsWorld *GetPhysicsWorld() const;

        /**
         * Test if we update when the scene is paused.
         *
         * @return Whether or not we will update even when the scene is paused.
         */
        bool GetDoPersistentUpdates() const;

        /**
         * Set whether or not we update when the `Scene` is paused
         *
         * @note This will unsubscribe and resubscribe if the events are already bound.
         * @param persistentUpdates_ Whether or not the entity can update during pauses.
         */
        void SetDoPersistentUpdates(bool persistentUpdates_);

        /**
         * Subscribe to update events in the scene.
         *
         * @warning If the entity changes parent this will need to be called again.
         * @return Whether or the entity now listens to updates.
         */
        bool SubscribeToUpdate();

        /**
         * Determine if the entity is subscribed to the `Scene` update events.
         *
         * @return Whether or not the entity is subscribed to `Scene` updates.
         */
        bool SubscribedToUpdate() const;

        /**
         * Unsubscribe from update events in the scene.
         */
        void UnsubscribeFromUpdate();

        /**
         * Draw code for the entity
         */
        virtual void Draw();

        /**
         * Update the entity.
         */
        virtual void Update();
    };
}

#endif //ENTITY_H
