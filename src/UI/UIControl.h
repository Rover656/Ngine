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

#ifndef UICONTROL_H
#define UICONTROL_H

#include "../ngine.h"

#include "../Math/Vector2.h"

namespace NerdThings::Ngine::UI {
    /*
     * A UI component, parented by a panel
     */
    class NEAPI UIControl {
        // Private Fields

        /*
         * Control parent, may be null
         */
        UIControl *_Parent;
    public:

        // Public Constructor(s)

        // Public Methods

        /*
         * Get parent as type
         */
        template<typename ParentType>
        ParentType* GetParent() {
            return dynamic_cast<ParentType*>(_Parent);
        }
    protected:

        // Protected Methods

        /*
         * Set parent, used by panels mostly
         */
        void SetParent(UIControl *parent_);
    };
}

#endif // UICONTROL_H