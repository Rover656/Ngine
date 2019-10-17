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

#include "VerticalPanel.h"

namespace NerdThings::Ngine::UI::Controls {
    // Public Methods

    void VerticalPanel::FocusNext() {
        // TODO: Add this functionality in feature/advanced-ui for controller/tabbing support.
    }

    void VerticalPanel::FocusPrev() {
        // TODO: Add this functionality in feature/advanced-ui for controller/tabbing support.
    }

    float VerticalPanel::GetOffsetAbove(UIControl *control_) {
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
            offset += childStyle.GetBorderRect(child->GetRenderRectangle()).Height;
        }

        return offset;
    }

    float VerticalPanel::GetOffsetBeside(UIControl *control_) {
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
}