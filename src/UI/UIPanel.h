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

#ifndef UIPANEL_H
#define UIPANEL_H

#include "../ngine.h"

#include "../Math/Vector2.h"
#include "UIControl.h"

namespace NerdThings::Ngine::UI {
    /*
     * A UI Panel base, contains entities
     */
    class NEAPI UIPanel : public UIControl {
        // Private Fields

        /*
         * Panel children
         */
        std::vector<UIControl *> _Children;
    public:
        // Public Constructor(s)

        // Public Methods

        /*
         * Add a child to the panel
         */
        void AddChild(UIControl *);
    };
}

#endif // UIPANEL_H