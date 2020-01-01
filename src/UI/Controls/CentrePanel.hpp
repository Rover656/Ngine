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


#ifndef CENTREPANEL_HPP
#define CENTREPANEL_HPP

#include "../../Config.hpp"

#include "UI/UIPanel.hpp"

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

#endif //CENTREPANEL_HPP
