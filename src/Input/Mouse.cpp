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
    // Public Fields

    MouseState Mouse::LastMouseState;
    Core::EventHandler<MouseButtonEventArgs> Mouse::OnMouseButtonPressed;
    Core::EventHandler<MouseButtonEventArgs> Mouse::OnMouseButtonReleased;
    Core::EventHandler<MouseMovedEventArgs> Mouse::OnMouseMoved;
    Core::EventHandler<MouseScrollChangedEventArgs> Mouse::OnMouseScrollYChanged;

    // Public Methods

    TVector2 Mouse::GetMousePosition() {
        return TVector2::FromRaylibVec(::GetMousePosition());
    }

    MouseState Mouse::GetMouseState() {
        MouseState state;

        for (auto i = 0; i < 3; i++) {
            state.ButtonsDown[i] = IsButtonDown(static_cast<EMouseButton>(i));
            state.ButtonsPressed[i] = IsButtonPressed(static_cast<EMouseButton>(i));
            state.ButtonsReleased[i] = IsButtonReleased(static_cast<EMouseButton>(i));
        }

        state.MouseWheelMovementY = GetMouseWheelMovement();
        state.Position = GetMousePosition();

        return state;
    }

    int Mouse::GetMouseWheelMovement() {
        return GetMouseWheelMove();
    }

    bool Mouse::IsButtonDown(EMouseButton button_) {
        return IsMouseButtonDown(button_);
    }

    bool Mouse::IsButtonPressed(EMouseButton button_) {
        return IsMouseButtonPressed(button_);
    }

    bool Mouse::IsButtonReleased(EMouseButton button_) {
        return IsMouseButtonReleased(button_);
    }

    // Public Event Handles

    void Mouse::OnGameRun(EventArgs& e_) {
        LastMouseState = GetMouseState();
    }

    void Mouse::OnGameUpdate(EventArgs& e_) {
        // Get current state
        auto state = GetMouseState();

        // Mouse position events
        if (LastMouseState.Position != state.Position) {
            OnMouseMoved.Invoke(MouseMovedEventArgs(state.Position, state.Position - LastMouseState.Position));
        }

        // Mouse button events
        for (auto i = 0; i < 3; i++) {
            if (state.ButtonsPressed[i]) {
                OnMouseButtonPressed.Invoke({static_cast<EMouseButton>(i), true});
            } else if (state.ButtonsReleased[i]) {
                OnMouseButtonReleased.Invoke({static_cast<EMouseButton>(i), false});
            }
        }

        // Scroll wheel events
        if (state.MouseWheelMovementY != LastMouseState.MouseWheelMovementY) {
            OnMouseScrollYChanged.Invoke(MouseScrollChangedEventArgs(state.MouseWheelMovementY));
        }

        // Update last frame state
        LastMouseState = state;
    }
}
