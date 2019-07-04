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


#ifndef CENTREPANEL_H
#define CENTREPANEL_H

#include "../../ngine.h"

#include "../UIPanel.h"

namespace NerdThings::Ngine::UI::Controls {
    /*
     * A panel that holds a single child centered both vertically and horizontally
     */
    class CentrePanel : public UIPanel {
    public:

        // Public Constructor(s)

        CentrePanel(float width_, float height_)
                : UIPanel(width_, height_) {}

        // Public Methods

        void FocusNext() override {

        }

        void FocusPrev() override {

        }

        float GetOffsetAbove(UIControl *control_) override {
            if (control_ == nullptr || control_ == this)
                return 0;

            auto style = control_->GetStyle();
            auto dims = style.GetBorderDimensions({control_->GetWidth(), control_->GetHeight()});

            return (GetHeight() / 2.0f) - (dims.Y / 2.0f);
        }

        float GetOffsetBeside(UIControl *control_) override {
            if (control_ == nullptr || control_ == this)
                return 0;

            auto style = control_->GetStyle();
            auto dims = style.GetBorderDimensions({control_->GetWidth(), control_->GetHeight()});

            return (GetWidth() / 2.0f) - (dims.X / 2.0f);
        }
    };
}

#endif // CENTREPANEL_H
