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
         * On update event reference
         */
        EventHandleRef<EventArgs> _OnUpdateRef;

    public:
        // Public Fields

        /*
         * Parent Scene
         */
        Scene *ParentScene;

        // Public Destructor

        virtual ~BaseEntity() = default;

        // Public Methods

        /*
         * Add a component to the entity.
         * component_ must be derived from class Component.
         * Returns whether or not the component was added
         */
        template <typename ComponentType>
        bool AddComponent(const std::string &name_, ComponentType *component_);

        /*
         * Draw code for the entity
         */
        virtual void Draw(EventArgs &e);

        /*
         * Get a component by name.
         */
        template <typename ComponentType>
        ComponentType *GetComponent(const std::string &name_);

        /*
         * Get all components
         */
        std::vector<Component*> GetComponents();

        /*
         * Test if a component exists by a name.
         */
        bool HasComponent(const std::string &name_);

        /*
         * Removes the entity from the component.
         * Returns success or fail.
         */
        bool RemoveComponent(const std::string &name_);

        /*
         * Subscribe to draw events in the 
         */
        bool SubscribeToDraw();

        /*
         * Subscribe to update events in the scene
         */
        bool SubscribeToUpdate();

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
    protected:
        // Protected Constructor(s)

        /*
         * Create a BaseEntity.
         */
        explicit BaseEntity(Scene *parentScene_);
    };
}

#endif //BASEENTITY_H
