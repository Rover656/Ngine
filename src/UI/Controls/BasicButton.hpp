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

#ifndef BASICBUTTON_HPP
#define BASICBUTTON_HPP

#include "../../Config.hpp"

#include "UI/UIControlInteractible.hpp"
#include "UI/UIControlSized.hpp"
#include "Label.hpp"
#include "CentrePanel.hpp"

namespace NerdThings::Ngine::UI::Controls {
    /*
     * A basic button that will run an action when clicked.
     * It has a simple label within it
     */
    class BasicButton : public UIControlSized, public UIControlInteractible {
    public:

        // Public Constructor(s)

        BasicButton(std::string buttonText_, Graphics::Font *font_, float fontSize_, float width_, float height_, float fontSpacing_ = 2) {
            // Set dimensions
            UIControlSized::SetWidth(width_);
            UIControlSized::SetHeight(height_);

            // Allow one child
            _ChildrenConfig = 1;

            // Add label
            auto panel = new CentrePanel(width_, height_);

            auto label = new Label(std::move(buttonText_), font_);
            label->SetFontSize(fontSize_);
            label->SetFontSpacing(fontSpacing_);
            panel->AddChild("label", label);

            UIControlSized::AddChild("panel", panel);
        }

        // Public Methods

        float GetHeight() override {
            return UIControlSized::GetHeight();
        }

        Label *GetLabel() {
            return GetChild<CentrePanel>("panel")->GetChild<Label>("label");
        }

        CentrePanel *GetLabelPanel() {
            return GetChild<CentrePanel>("panel");
        }

        float GetWidth() override {
            return UIControlSized::GetWidth();
        }

        void SetHeight(float height_) override {
            UIControlSized::SetHeight(height_);
            GetLabelPanel()->SetHeight(height_);
        }

        void SetMaxHeight(float height_) override {
            UIControlSized::SetMaxHeight(height_);
            GetLabelPanel()->SetMaxHeight(height_);
        }

        void SetMaxWidth(float width_) override {
            UIControlSized::SetMaxWidth(width_);
            GetLabelPanel()->SetMaxWidth(width_);
        }

        void SetMinHeight(float height_) override {
            UIControlSized::SetMinHeight(height_);
            GetLabelPanel()->SetMinHeight(height_);
        }

        void SetMinWidth(float width_) override {
            UIControlSized::SetMinWidth(width_);
            GetLabelPanel()->SetMinWidth(width_);
        }

        void SetWidth(float width_) override {
            UIControlSized::SetWidth(width_);
            GetLabelPanel()->SetWidth(width_);
        }

        void Draw() override {
            UIControl::Draw();
        }
    };
}

#endif //BASICBUTTON_HPP
