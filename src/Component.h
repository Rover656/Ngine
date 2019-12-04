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

#ifndef COMPONENT_H
#define COMPONENT_H

#include "Ngine.h"

#include "Events.h"
#include "Scene.h"

namespace NerdThings::Ngine {
    class BaseEntity;

    /**
     * A component is an object that is attached to an entity and manipulates its behaviour.
     */
    class NEAPI Component {
        /**
         * Entity OnDraw reference.
         */
        EventAttachment<> m_onDrawRef;

        /**
         * Entity OnUpdate reference.
         */
        EventAttachment<> m_onUpdateRef;

        /**
         * The parent entity.
         */
        BaseEntity *m_parentEntity = nullptr;
    public:
        /**
         * On detached from an entity.
         */
        Event<> OnDetached;

        virtual ~Component();

        /**
         * Get the parent entity as a type
         *
         * @tparam The type to get the entity as.
         * @return The entity casted to the provided type.
         */
        template <typename EntityType = BaseEntity>
        EntityType *GetParent() const {
            return dynamic_cast<EntityType*>(m_parentEntity);
        }

        /**
         * Whether or not the component has a parent.
         *
         * @return Whether or not this component is parented by an entity.
         */
        bool HasParent() const;

        // TODO: Functions like this also need templates.

        /**
         * Get the parent scene
         *
         * @return The parent entity's parent scene.
         */
        Scene *GetScene() const;

        /**
         * Get the parent game.
         *
         * @return The game that we are a part of.
         */
        Game *GetGame() const;

        /**
         * Unsubscribe from the entity's draw event.
         */
        void UnsubscribeFromDraw();

        /**
         * Subscribe to the entity's draw event.
         */
        void SubscribeToDraw();

        /**
         * Subscribe to the entity's update event.
         */
        void SubscribeToUpdate();

        /**
         * Unsubscribe from the entity's update event.
         */
        void UnsubscribeFromUpdate();

        /**
         * Component draw event.
         */
        virtual void Draw();

        /**
         * Component update logic
         */
        virtual void Update();

    protected:
        /**
         * Initialise component
         *
         * @param The parent entity.
         */
        Component(BaseEntity *parent_);
    };
}

#endif //COMPONENT_H
