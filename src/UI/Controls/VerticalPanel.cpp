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

#include "VerticalPanel.hpp"

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