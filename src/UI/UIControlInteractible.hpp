/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#ifndef UICONTROLINTERACTIBLE_HPP
#define UICONTROLINTERACTIBLE_HPP

#include "../Config.hpp"

#include "../Events.hpp"
#include "UIControl.hpp"

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

#endif //UICONTROLINTERACTIBLE_HPP
