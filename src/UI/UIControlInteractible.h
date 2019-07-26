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

#ifndef UICONTROLINTERACTIBLE_H
#define UICONTROLINTERACTIBLE_H

#include "../ngine.h"

#include "../EventArgs.h"
#include "../EventHandler.h"
#include "UIControl.h"

namespace NerdThings::Ngine::UI {
    /*
     * An interactible UI Control
     */
    class NEAPI UIControlInteractible : virtual public UIControl {
        // Private Fields

        /*
         * Whether or not the control is hovered
         */
        bool _Hovered = false;
    public:
        // Public Fields

        /*
         * On click
         */
        EventHandler<UIControlEventArgs> OnClick;

        /*
         * On hover
         */
        EventHandler<UIControlEventArgs> OnHover;

        /*
         * On un hover/mouse out
         */
        EventHandler<UIControlEventArgs> OnUnHover;

        // Public Methods

        /*
         * Update checks for events
         */
        void Update() override;
    };
}

#endif // UICONTROLINTERACTIBLE_H
