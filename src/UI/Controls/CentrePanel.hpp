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
