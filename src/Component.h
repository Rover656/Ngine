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

#include "EventHandler.h"

namespace NerdThings::Ngine {
#ifndef BASEENTITY_H
    class NEAPI BaseEntity;
#endif

    /*
     * A component that can be attached to an entity
     */
    class NEAPI Component {
        // Private Fields

        /*
         * On draw ref
         */
        EventHandleRef<EventArgs> _OnDrawRef;

        /*
         * On update ref
         */
        EventHandleRef<EventArgs> _OnUpdateRef;

        /*
         * The parent entity
         */
        BaseEntity *_ParentEntity = nullptr;
    public:
        // Public Fields

        /*
         * On detached from an entity
         */
        EventHandler<EventArgs> OnDetached;

        // Destructor

        /*
         * Destruct component
         */
        virtual ~Component();

        // Public Methods

        /*
         * Draw
         */
        virtual void Draw(EventArgs &e);

        /*
         * Get the parent entity as a type
         */
        template <typename EntityType>
        EntityType *GetParent() const {
            return dynamic_cast<EntityType*>(_ParentEntity);
        }

        /*
         * Whether or not the component has a parent
         */
        [[nodiscard]] bool HasParent() const;

        /*
         * Subscribe to entity draw
         */
        void SubscribeToDraw();

        /*
         * Subscribe to entity update
         */
        void SubscribeToUpdate();

        /*
         * Unsubscribe from entity draw
         */
        void UnsubscribeFromDraw();

        /*
         * Unsubscribe from entity update
         */
        void UnsubscribeFromUpdate();

        /*
         * Update
         */
        virtual void Update(EventArgs &e);

    protected:

        // Protected Constructor(s)

        /*
         * Initialise component
         */
        Component(BaseEntity *parent_);
    };
}

#endif //COMPONENT_H
