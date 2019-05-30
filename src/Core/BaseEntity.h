/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
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
#include "../ngine.h"

#include "EntityContainer.h"
#include "Scene.h"

namespace NerdThings::Ngine::Core {
    class Component;

    /*
     * The root class for an entity within a scene
     */
    class NEAPI BaseEntity : public EntityContainer {
        // Private fields

        /*
         * The list of all components.
         * All components are given a name for easy identification.
         */
        std::map<std::string, Component*> _Components;

        /*
         * On draw event reference
         */
        EventHandleRef<EventArgs> _OnDrawRef;

        /*
         * On draw camera event reference
         */
        EventHandleRef<EventArgs> _OnDrawCameraRef;

        /*
         * On update event reference
         */
        EventHandleRef<EventArgs> _OnUpdateRef;

        /*
         * The entity position
         */
        TVector2 _Position;
    public:
        // Public Fields

        /*
         * On draw event
         */
        EventHandler<EventArgs> OnDraw;

        /*
         * On draw event with camera translation
         */
        EventHandler<EventArgs> OnDrawCamera;

        /*
         * On position changed event
         */
        EventHandler<EntityPositionChangedEventArgs> OnPositionChanged;

        /*
         * On update event
         */
        EventHandler<EventArgs> OnUpdate;

        /*
         * Parent Scene
         */
        Scene *ParentScene;

        // Public Constructor(s)

        /*
         * Create a BaseEntity.
         */
        BaseEntity(Scene *parentScene_, TVector2 position_);

        // Destructor

        virtual ~BaseEntity();

        // Public Methods

        /*
         * Add a component to the entity.
         * component_ must be derived from class Component.
         * Returns the component if successful and null if not
         */
        template <typename ComponentType>
        ComponentType* AddComponent(const std::string &name_, ComponentType *component_) {
            // Check the name is not taken
            if (HasComponent(name_))
                return nullptr;

            // Cast to component to ensure this is valid
            auto comp = dynamic_cast<Component*>(component_);

            if (comp != nullptr) {
                _Components.insert({ name_, comp });
                return component_;
            }

            return nullptr;
        }

        /*
         * Draw code for the entity
         */
        virtual void Draw(EventArgs &e);

        /*
         * Draw code for the entity with camera translation
         */
        virtual void DrawCamera(EventArgs &e);

        /*
         * Get a component by name.
         */
        template <typename ComponentType>
        ComponentType *GetComponent(const std::string &name_) {
            // Try to find the component
            if (HasComponent(name_)) {
                return dynamic_cast<ComponentType*>(_Components.at(name_)); // Will return null if its the wrong type
            }

            return nullptr;
        }

        /*
         * Get all components
         */
        std::vector<Component*> GetComponents();

        /*
         * Get the entity position
         */
        [[nodiscard]] TVector2 GetPosition() const;

        /*
         * Test if a component exists by a name.
         */
        bool HasComponent(const std::string &name_);

        /*
         * Move an entity
         */
        void MoveBy(TVector2 moveBy_);

        /*
         * Removes the entity from the component.
         * Returns success or fail.
         */
        bool RemoveComponent(const std::string &name_);

        /*
         * Set entity position
         */
        void SetPosition(TVector2 position);

        /*
         * Subscribe to draw events in the scene with camera translation
         */
        bool SubscribeToCameraDraw();

        /*
         * Subscribe to draw events in the scene
         */
        bool SubscribeToDraw();

        /*
         * Subscribe to update events in the scene
         */
        bool SubscribeToUpdate();

        /*
         * Unsubscribe from update events in the scene with camera translation
         */
        void UnsubscribeFromCameraDraw();

        /*
         * Unsubscribe from update events in the scene
         */
        void UnsubscribeFromDraw();

        /*
         * Unsubscribe from update events in the scene
         */
        void UnsubscribeFromUpdate();

        /*
         * Update the entity
         */
        virtual void Update(EventArgs &e);
    };
}

#endif //BASEENTITY_H
