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

#ifndef VERTICALPANEL_H
#define VERTICALPANEL_H

#include "../../ngine.h"

#include "../UIPanel.h"

namespace NerdThings::Ngine::UI::Controls {
    /*
     * A panel that displays its children vertically
     */
    class VerticalPanel : public UIPanel {
    public:
        // Public Fields

        /*
         * Element horizontal alignment
         */
        EUIPanelHorizontalAlignment HorizontalAlignment = ALIGN_LEFT;

        // Public Constructor(s)

        VerticalPanel(float width_, float height_)
                : UIPanel(width_, height_) {}

        // Public Methods

        void FocusNext() override {
            // TODO: Will be done in feature/advanced-ui later. This is for controller and tab support
        }

        void FocusPrev() override {
            // TODO: Will be done in feature/advanced-ui later. This is for controller and tab support
        }

        float GetOffsetAbove(UIControl *control_) override {
            // This returns the heights of everything before this control

            auto children = GetChildren();
            auto iterator = std::find(children.begin(), children.end(), control_);

            if (iterator == children.end())
                return 0;

            auto index = std::distance(children.begin(), iterator);

            auto style = GetStyle();

            auto offset = style.Padding[0];

            for (auto i = index - 1; i >= 0; i--) {
                auto child = children[i];
                auto childStyle = child->GetStyle();

                // Margins TODO: Is this right
                offset += childStyle.Margin[0];
                offset += childStyle.Margin[2];

                // Height
                offset += childStyle.GetBorderRect(child->GetScreenRectangle()).Height;
            }

            return offset;
        }

        float GetOffsetBeside(UIControl *control_) override {
            if (control_ == nullptr || control_ == this)
                return 0;

            auto style = control_->GetStyle();
            auto dims = style.GetBorderDimensions({control_->GetWidth(), control_->GetHeight()});
            switch (HorizontalAlignment) {
                case ALIGN_LEFT:
                    return 0;
                case ALIGN_CENTER:
                    return (GetWidth() / 2.0f)  - (dims.X / 2.0f);
                case ALIGN_RIGHT:
                    return GetWidth() - dims.X;
            }

            return 0; // Shut the compilers up
        }
    };
}

#endif // VERTICALPANEL_H
