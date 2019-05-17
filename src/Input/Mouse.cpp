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

#include "Mouse.h"

namespace Ngine::Input {
    // Public Methods

    TVector2 Mouse::GetMousePosition() {
        return TVector2::FromRaylibVec(::GetMousePosition());
    }

    MouseState Mouse::GetMouseState() {
        MouseState state;

        for (auto i = 0; i < 3; i++) {
            state.ButtonsDown[i] = IsButtonDown(static_cast<MouseButton>(i));
            state.ButtonsPressed[i] = IsButtonPressed(static_cast<MouseButton>(i));
            state.ButtonsReleased[i] = IsButtonReleased(static_cast<MouseButton>(i));
        }

        state.MouseWheelMovementY = GetMouseWheelMovement();
        state.Position = GetMousePosition();

        return state;
    }

    int Mouse::GetMouseWheelMovement() {
        return GetMouseWheelMove();
    }

    bool Mouse::IsButtonDown(MouseButton button_) {
        return IsMouseButtonDown(button_);
    }

    bool Mouse::IsButtonPressed(MouseButton button_) {
        return IsMouseButtonPressed(button_);
    }

    bool Mouse::IsButtonReleased(MouseButton button_) {
        return IsMouseButtonReleased(button_);
    }
}