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
//
//        for (auto i = 0; i < 3; i++) {
//            state.ButtonsDown[i] = IsButtonDown(static_cast<EMouseButton>(i));
//            state.ButtonsPressed[i] = IsButtonPressed(static_cast<EMouseButton>(i));
//            state.ButtonsReleased[i] = IsButtonReleased(static_cast<EMouseButton>(i));
//        }
//
//        state.MouseWheelMovementY = GetMouseWheelMovement();
//        state.Position = GetMousePosition();
//
        return state;
    }

    // Public Fields

    EventHandler<MouseButtonEventArgs> Mouse::OnMouseButtonPressed;
    EventHandler<MouseButtonEventArgs> Mouse::OnMouseButtonReleased;
    EventHandler<MouseMovedEventArgs> Mouse::OnMouseMoved;
    EventHandler<MouseScrollChangedEventArgs> Mouse::OnMouseScrollYChanged;

    // Public Methods

    void Mouse::CancelButton(EMouseButton button_) {
        _CurrentMouseState.ButtonsDown[button_] = false;
        _CurrentMouseState.ButtonsPressed[button_] = false;
        _CurrentMouseState.ButtonsReleased[button_] = true;
    }

    TVector2 Mouse::GetMousePosition() {
        //return TVector2::FromRaylibVec(::GetMousePosition());
        return {0, 0};
    }

    MouseState Mouse::GetMouseState() {
        return _CurrentMouseState;
    }

    int Mouse::GetMouseWheelMovement() {
        //return GetMouseWheelMove();
        return 0;
    }

    bool Mouse::IsButtonDown(EMouseButton button_) {
        //return IsMouseButtonDown(button_);
        return false;
    }

    bool Mouse::IsButtonPressed(EMouseButton button_) {
        //return IsMouseButtonPressed(button_);
        return false;
    }

    bool Mouse::IsButtonReleased(EMouseButton button_) {
        //return IsMouseButtonReleased(button_);
        return false;
    }

    // Public Event Handles

    void Mouse::OnGameRun(EventArgs &e_) {
        _LastMouseState = FetchState();
    }

    void Mouse::OnGameUpdate(EventArgs &e_) {
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
        //SetMouseOffset(ox_, oy_);
    }

    void Mouse::SetScale(float sx_, float sy_) {
        //SetMouseScale(sx_, sy_);
    }
}
