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

#ifndef HORIZONTALPANEL_HPP
#define HORIZONTALPANEL_HPP

#include "../../Config.hpp"

#include "../UIPanel.hpp"

namespace NerdThings::Ngine::UI::Controls {
    /*
     * A panel that displays its children vertically
     */
    class NEAPI HorizontalPanel : public UIPanel {
    public:
        // Public Fields

        /*
         * Element horizontal alignment
         */
        EVerticalAlignment VerticalAlignment = ALIGN_TOP;

        // Public Constructor(s)

        HorizontalPanel(float width_, float height_)
                : UIPanel(width_, height_) {}

        // Public Methods

        void FocusNext() override;

        void FocusPrev() override;

        float GetOffsetAbove(UIControl *control_) override;

        float GetOffsetBeside(UIControl *control_) override;
    };
}

#endif //HORIZONTALPANEL_HPP
