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

#ifndef BASICBUTTON_H
#define BASICBUTTON_H

#include "../../Config.hpp"

#include "../UIControlInteractible.h"
#include "../UIControlSized.h"
#include "Label.h"
#include "CentrePanel.h"

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

#endif // BASICBUTTON_H
