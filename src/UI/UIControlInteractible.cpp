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

#include "UIControlInteractible.hpp"

#include "../Input/Mouse.hpp"

namespace NerdThings::Ngine::UI {
    // Public Methods

    void UIControlInteractible::Update() {
        // Get button bounds (from border)
        auto style = GetStyle();
        auto rect = style.GetBorderRect(GetLogicRectangle());

        // Get mouse position
        auto mPos = Input::Mouse::GetCurrent()->GetMousePosition();

        // Check the mouse is within our parent too, if not we cant be clicked
        auto par = GetParent<UIControl>();
        if (par != nullptr) {
            auto parentRect = par->GetLogicRectangle();

            if (!parentRect.Contains(mPos))
                return;
        }

        if (rect.Contains(mPos)) {
            // The mouse is over the button
            if (Input::Mouse::GetCurrent()->IsButtonPressed(Input::MOUSE_BUTTON_LEFT)) {
                // Clicked
                OnClick({this});

                // Deregister click so double events dont happen
                Input::Mouse::GetCurrent()->CancelButton(Input::MOUSE_BUTTON_LEFT);
            } else if (!_Hovered) {
                // Hovered
                OnHover({this});
                _Hovered = true;
            }
        } else if (_Hovered) {
            OnUnHover({this});
            _Hovered = false;
        }
    }
}