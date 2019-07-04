#include <utility>

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

#ifndef BASICBUTTON_H
#define BASICBUTTON_H

#include "../../ngine.h"

#include "../UIControlInteractible.h"
#include "../UIControlSized.h"
#include "Label.h"

namespace NerdThings::Ngine::UI::Controls {
    /*
     * A basic button that will run an action when clicked.
     * It has a simple label within it
     */
    class BasicButton : public UIControlSized, public UIControlInteractible {
    public:

        // Public Constructor(s)

        BasicButton(std::string buttonText_, Graphics::TFont *font_, float fontSize_, float width_, float height_, float fontSpacing_ = 2) {
            // Set dimensions
            UIControlSized::SetWidth(width_);
            UIControlSized::SetHeight(height_);

            // Allow one child
            _ChildrenConfig = 1;

            // Add label
            auto label = new Label(std::move(buttonText_), font_, UIControlSized::GetWidth(), UIControlSized::GetHeight());
            label->SetFontSize(fontSize_);
            label->SetFontSpacing(fontSpacing_);
            UIControlSized::AddChild("label", label);
        }

        // Public Methods

        float GetHeight() override {
            return UIControlSized::GetHeight();
        }

        Label *GetLabel() {
            return GetChild<Label>("label");
        }

        float GetWidth() override {
            return UIControlSized::GetWidth();
        }

        // Major TODO: Account for styling in the below code

        void SetHeight(float height_) override {
            auto l = UIControlSized::GetChild<Label>("label");
            l->SetHeight(height_);
            UIControlSized::SetHeight(height_);
        }

        void SetMaxHeight(float height_) override {
            auto l = UIControlSized::GetChild<Label>("label");
            l->SetMaxHeight(height_);
            UIControlSized::SetMaxHeight(height_);
        }

        void SetMaxWidth(float width_) override {
            auto l = UIControlSized::GetChild<Label>("label");
            l->SetMaxWidth(width_);
            UIControlSized::SetMaxWidth(width_);
        }

        void SetMinHeight(float height_) override {
            auto l = UIControlSized::GetChild<Label>("label");
            l->SetMinHeight(height_);
            UIControlSized::SetMinHeight(height_);
        }

        void SetMinWidth(float width_) override {
            auto l = UIControlSized::GetChild<Label>("label");
            l->SetMinWidth(width_);
            UIControlSized::SetMinWidth(width_);
        }

        void SetWidth(float width_) override {
            auto l = UIControlSized::GetChild<Label>("label");
            l->SetWidth(width_);
            UIControlSized::SetWidth(width_);
        }
    };
}

#endif // BASICBUTTON_H
