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

#ifndef BASEENTITY_H
#define BASEENTITY_H

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
    class NEAPI BaseEntity : public EntityContainer {
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
        std::map<std::string, std::unique_ptr<Component>> m_components;

        /**
         * Depth index.
         */
        int m_depth;

        /**
         * On update event reference
         */
        EventAttachment<> m_onUpdateRef;

        /**
         * The entity origin
         */
        Vector2 m_origin;

        /**
         * The parent entity
         */
        BaseEntity *m_parentEntity = nullptr;

        /**
         * Parent Scene
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
         * The entity scale (Used for rendering and physics)
         */
        float m_scale = 1;

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
         * On draw event
         */
        Event<> OnDraw;

        /**
         * On position changed event
         */
        Event<EntityTransformChangedEventArgs> OnTransformChanged;

        /**
         * On update event
         */
        Event<> OnUpdate;

        // TODO: Sort

        // TODO: Add physics body creation to constructor
        /**
         * Create a new entity.
         *
         * @param parentScene_ The parent scene.
         * @param position_ The initial position
         * @param depth_ The depth to be rendered at
         * @param canCull_ Whether or not this can be culled.
         */
        BaseEntity(Vector2 position_, int depth_ = 0, bool canCull_ = false);
        virtual ~BaseEntity();

        /**
         * Add a component to the entity.
         *
         * @tparam The component type. This must be a class derrived from NerdThings::Ngine::Component
         * @param name_ The name of the component.
         * @param component_ The component.
         * @return The component, so you may chain events.
         */
        template <typename ComponentType>
        ComponentType *AddComponent(const std::string &name_, ComponentType *component_) {
            // Check the name is not taken
            if (HasComponent(name_))
                return nullptr;

            // Cast to component to ensure this is valid
            auto comp = dynamic_cast<Component*>(component_);

            if (comp != nullptr) {
                m_components.insert({name_, std::unique_ptr<Component>(comp)});
                return component_;
            }

            return nullptr;
        }

        /**
         * This is used to determine if this entity should be culled.
         * This can be overridden for customisation.
         *
         * @param cullArea_ The scene cullarea.
         * @return Whether or not this entity should be culled.
         */
        virtual bool CheckForCulling(Rectangle cullArea_);

        /**
         * Remove from our parent and delete ourselves.
         *
         * @warning No more calls must be made to the entity following this.
         */
        void Destroy();

        /**
         * Draw code for the entity
         */
        virtual void Draw();

        /**
         * Get a component by name.
         *
         * @tparam ComponentType The component type to be returned.
         */
        template <typename ComponentType = Component>
        ComponentType *GetComponent(const std::string &name_) {
            // Try to find the component
            if (HasComponent(name_)) {
                return dynamic_cast<ComponentType*>(m_components.at(name_).get()); // Will return null if its the wrong type
            }

            return nullptr;
        }

        /**
         * Check if this entity can be culled
         *
         * @return Whether or not this entity can be culled.
         */
        bool GetCanCull();

        /**
         * Get all components.
         *
         * @return An vector containing all components.
         */
        std::vector<Component*> GetComponents();

        /**
         * Get the entity depth.
         *
         * @return The depth of the entity.
         */
        int GetDepth() const;

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
         * @tparam EntityType The type to cast the parent to (Default: BaseEntity)
         * @return The parent entity, null if none.
         */
        template <typename EntityType = BaseEntity>
        BaseEntity *GetParentEntity() const {
            return dynamic_cast<EntityType *>(m_parentEntity); // TODO: Choose between dynamic or straight cast
        }

        /**
         * Get the parent scene.
         *
         * @return The parent scene.
         */
        Scene *GetScene() const;

        /**
         * Get the parent game.
         *
         * @return The parent game.
         */
        Game *GetGame() const;

        /**
         * Test if we update when the scene is paused.
         *
         * @return Whether or not we will update even when the scene is paused.
         */
        bool GetDoPersistentUpdates();

        /**
         * Get the entity position
         *
         * @return The position of the entity in world coordinates.
         */
        Vector2 GetPosition() const;

        /**
         * Get the attached physics body.
         *
         * @return The physics body. If none, null.
         */
        Physics::PhysicsBody *GetPhysicsBody() const;

        /**
         * Get the world our body is a member of.
         *
         * @return The world the physics body is attached to, if any.
         */
        Physics::PhysicsWorld *GetPhysicsWorld() const;

        /**
         * Get the entity rotation
         *
         * @return The entity rotation, in degrees.
         */
        float GetRotation() const;

        /**
         * Test if a component exists by a name.
         *
         * @param name_ The name of the component to check for.
         * @return Whether or not the component exists.
         */
        bool HasComponent(const std::string &name_);

        /**
         * Determine if this entity is affected by physics.
         *
         * @return Whether or not this entity has a physics body attached.
         */
        bool IsPhysicsEntity();

        /**
         * Move an entity by a vector.
         *
         * @param moveBy_ The number of pixels to move in each direction.
         * @warning If this is a physics entity, use the physics body to apply a velocity instead.
         */
        void MoveBy(Vector2 moveBy_);

        /**
         * Removes the entity from the component.
         *
         * @param name_ The name of the component to remove.
         * @return Whether or not the component was removed.
         */
        bool RemoveComponent(const std::string &name_);

        /**
         * Set whether or not this entity can be culled
         *
         * @param canCull_ Whether or not the entity can be culled.
         */
        void SetCanCull(bool canCull_);

        /**
         * Set the entity depth
         *
         * @param depth_ The depth to set the entity to.
         */
        void SetDepth(int depth_);

        /**
         * Set whether or not we update when the scene is paused
         *
         * @param persistentUpdates_ Whether or not the entity can update during pauses.
         */
        void SetDoPersistentUpdates(bool persistentUpdates_);

        /**
         * Set entity position
         *
         * @param position_ The position to move to.
         */
        void SetPosition(Vector2 position_);

        /**
         * Set entity rotation.
         *
         * @param rotation_ The rotation, in degrees, to set our entity to.
         */
        void SetRotation(float rotation_);

        /**
         * Set the entity physics body.
         *
         * @param body_ The body to add to this entity.
         * @note The lifecycle of the body will now be managed by this entity, even if another body is set.
         */
        void SetPhysicsBody(Physics::PhysicsBody *body_);

        /**
         * Subscribe to update events in the scene.
         *
         * @return Whether or not updates were subscribed to.
         */
        bool SubscribeToUpdate();

        /**
         * Unsubscribe from update events in the scene.
         */
        void UnsubscribeFromUpdate();

        /**
         * Update the entity.
         */
        virtual void Update();
    };
}

#endif //BASEENTITY_H
