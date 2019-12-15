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

#ifndef VERTICALPANEL_H
#define VERTICALPANEL_H

#include "../../Config.hpp"

#include "../UIPanel.h"

namespace NerdThings::Ngine::UI::Controls {
    /*
     * A panel that displays its children vertically
     */
    class NEAPI VerticalPanel : public UIPanel {
    public:
        // Public Fields

        /*
         * Element horizontal alignment
         */
        EHorizontalAlignment HorizontalAlignment = ALIGN_LEFT;

        // Public Constructor(s)

        VerticalPanel(float width_, float height_)
                : UIPanel(width_, height_) {}

        // Public Methods

        void FocusNext() override;

        void FocusPrev() override;

        float GetOffsetAbove(UIControl *control_) override;

        float GetOffsetBeside(UIControl *control_) override;
    };
}

#endif // VERTICALPANEL_H
