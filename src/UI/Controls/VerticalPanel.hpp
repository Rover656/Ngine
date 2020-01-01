/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef VERTICALPANEL_HPP
#define VERTICALPANEL_HPP

#include "../../Config.hpp"

#include "../UIPanel.hpp"

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

#endif //VERTICALPANEL_HPP
