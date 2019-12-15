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

#ifndef UICONTROLINTERACTIBLE_H
#define UICONTROLINTERACTIBLE_H

#include "../Config.hpp"

#include "../Events.hpp"
#include "UIControl.h"

namespace NerdThings::Ngine::UI {
    /*
     * UI Control event args
     */
    struct UIControlEventArgs : EventArgs {
        // Public Fields

        /*
         * The control this belongs to
         */
        UI::UIControl *Control;

        // Public Constructor(s)

        UIControlEventArgs(UI::UIControl *control_)
                : Control(control_) {}
    };

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
        Event<UIControlEventArgs> OnClick;

        /*
         * On hover
         */
        Event<UIControlEventArgs> OnHover;

        /*
         * On un hover/mouse out
         */
        Event<UIControlEventArgs> OnUnHover;

        // Public Methods

        /*
         * Update checks for events
         */
        void Update() override;
    };
}

#endif // UICONTROLINTERACTIBLE_H
