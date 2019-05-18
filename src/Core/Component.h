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

#ifndef COMPONENT_H
#define COMPONENT_H

// Include ngine
#include "../ngine.h"

namespace NerdThings::Ngine::Core {
    class NEAPI BaseEntity;

    /*
     * A component that can be attached to an entity
     */
    class NEAPI Component {
        // Private Fields

        /*
         * The parent entity
         */
        BaseEntity *_ParentEntity = nullptr;

    public:
        // Public Destructor

        /*
         * Destruct component
         */
        virtual ~Component() = default;

        // Public Methods

        /*
         * Get the component parent
         */
        BaseEntity *GetParent() const;

        /*
         * Whether or not the component has a parent
         */
        bool HasParent() const;

        /*
         * Called when the component is added to an entity
         */
        void OnAttach(BaseEntity *_attachedEntity);

        /*
         * Called when the component is removed from an entity
         */
        void OnDetach();

        /*
         * Called when the entity draws
         */
        void OnDraw();

        /*
         * Called when the entity updates
         */
        void OnUpdate();

    protected:

        // Protected Constructor(s)

        /*
         * Initialise component
         */
        Component();
    };
}

#endif //COMPONENT_H
