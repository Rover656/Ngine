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

namespace NerdThings::Ngine::Input {
    // Private Fields

    MouseState Mouse::_CurrentMouseState;
    MouseState Mouse::_LastMouseState;

    // Private Methods

    MouseState Mouse::FetchState() {
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

    // Public Fields

    Core::EventHandler<MouseButtonEventArgs> Mouse::OnMouseButtonPressed;
    Core::EventHandler<MouseButtonEventArgs> Mouse::OnMouseButtonReleased;
    Core::EventHandler<MouseMovedEventArgs> Mouse::OnMouseMoved;
    Core::EventHandler<MouseScrollChangedEventArgs> Mouse::OnMouseScrollYChanged;

    // Public Methods

    void Mouse::CancelButton(EMouseButton button_) {
        _CurrentMouseState.ButtonsDown[button_] = false;
        _CurrentMouseState.ButtonsPressed[button_] = false;
        _CurrentMouseState.ButtonsReleased[button_] = true;
    }

    Math::TVector2 Mouse::GetMousePosition() {
        return Math::TVector2::FromRaylibVec(::GetMousePosition());
    }

    MouseState Mouse::GetMouseState() {
        return _CurrentMouseState;
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

    void Mouse::OnGameRun(Core::EventArgs &e_) {
        _LastMouseState = FetchState();
    }

    void Mouse::OnGameUpdate(Core::EventArgs &e_) {
        // Get current state
        _CurrentMouseState = FetchState();

        // Mouse position events
        if (_LastMouseState.Position != _CurrentMouseState.Position) {
            OnMouseMoved({_CurrentMouseState.Position, _CurrentMouseState.Position - _LastMouseState.Position});
        }

        // Mouse button events
        for (auto i = 0; i < 3; i++) {
            if (_CurrentMouseState.ButtonsPressed[i]) {
                OnMouseButtonPressed({static_cast<EMouseButton>(i), true});
            } else if (_CurrentMouseState.ButtonsReleased[i]) {
                OnMouseButtonReleased({static_cast<EMouseButton>(i), false});
            }
        }

        // Scroll wheel events
        if (_CurrentMouseState.MouseWheelMovementY != _LastMouseState.MouseWheelMovementY) {
            OnMouseScrollYChanged({_CurrentMouseState.MouseWheelMovementY});
        }

        // Update last frame state
        _LastMouseState = _CurrentMouseState;
    }

    void Mouse::SetOffset(float ox_, float oy_) {
        SetMouseOffset(ox_, oy_);
    }

    void Mouse::SetScale(float sx_, float sy_) {
        SetMouseScale(sx_, sy_);
    }
}
