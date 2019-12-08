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
    class Entity;

    /**
     * A component is an object that is attached to an entity and manipulates its behaviour.
     */
    class NEAPI Component {
        /**
         * The parent entity.
         *
         * @note Should *NEVER* be null, if it is something is very wrong.
         */
        Entity *m_parentEntity = nullptr;

        /**
         * Entity OnDraw reference.
         */
        EventAttachment<> m_onDrawRef;

        /**
         * Entity OnUpdate reference.
         */
        EventAttachment<> m_onUpdateRef;
    public:
        /**
         * Fired when an entity destroys this component.
         */
        Event<> OnDestroy;

        virtual ~Component();

        /**
         * Get the parent entity as a type
         *
         * @tparam EntityType The type to get the entity as.
         * @return The entity casted to the provided type.
         */
        template <typename EntityType = Entity>
        EntityType *GetParent() const {
            return (EntityType *) m_parentEntity;
        }

        /**
         * Get the parent scene
         *
         * @tparam SceneType The type we want the scene as.
         * @return The parent entity's parent scene.
         */
        template <class SceneType = Scene>
        SceneType *GetScene() const {
            return (SceneType *) m_parentEntity->GetScene();
        }

        /**
         * Get the parent game.
         *
         * @return The game that we are a part of.
         */
        Game *GetGame() const;

        /**
         * Subscribe to the entity's draw event.
         */
        void SubscribeToDraw();

        /**
         * Determine if the component is subscribed to the draw event.
         *
         * @return Whether or not the component is subscribed to draw.
         */
        bool SubscribedToDraw() const;

        /**
         * Unsubscribe from the entity's draw event.
         */
        void UnsubscribeFromDraw();

        /**
         * Subscribe to the entity's update event.
         */
        void SubscribeToUpdate();

        /**
         * Determine if the component is subscribed to the update event.
         *
         * @return Whether or not the component is subscribed to update.
         */
        bool SubscribedToUpdate() const;

        /**
         * Unsubscribe from the entity's update event.
         */
        void UnsubscribeFromUpdate();

        /**
         * Component draw code.
         */
        virtual void Draw();

        /**
         * Component update code.
         */
        virtual void Update();

    protected:
        /**
         * Initialise component
         *
         * @note This is protected as `Component` MUST be used as a base class only.
         * @param parent_ The parent entity.
         */
        Component(Entity *parent_);
    };
}

#endif //COMPONENT_H
