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