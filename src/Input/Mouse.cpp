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

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#endif

#include "../Window.h"

namespace NerdThings::Ngine::Input {
    // Private Fields

    MouseState Mouse::_CurrentMouseState;
    MouseState Mouse::_LastMouseState;
    TVector2 Mouse::_MouseOffset;
    TVector2 Mouse::_MouseScale;
    MouseState Mouse::_NextMouseState;

    // Private Methods

    TVector2 Mouse::InternalGetMousePosition() {
        auto pos = TVector2::Zero;
#if defined(PLATFORM_DESKTOP)
        double x, y;
        glfwGetCursorPos((GLFWwindow*)Window::WindowPtr, &x, &y);
        pos.X = static_cast<float>(x);
        pos.Y = static_cast<float>(y);
#elif defined(PLATFORM_UWP)
        auto window = CoreWindow::GetForCurrentThread();
        auto pointerPos = window->PointerPosition;
        pos.X = pointerPos.X - window->Bounds.X;
        pos.Y = pointerPos.Y - window->Bounds.Y;
#endif

        return pos;
    }

#if defined(PLATFORM_DESKTOP)

    void Mouse::GLFWMouseButtonCallback(GLFWwindow *window_, int button_, int action_, int mods_) {
        if (button_ <= MOUSE_BUTTON_RIGHT) { // GLFW supports more than our 3 buttons
            _NextMouseState.ButtonsDown[button_] = (action_ == GLFW_PRESS);
        }
    }

    void Mouse::GLFWScrollCallback(GLFWwindow *window, double x_, double y_) {
        _NextMouseState.MouseWheelXDelta = static_cast<int>(x_);
        _NextMouseState.MouseWheelYDelta = static_cast<int>(y_);
    }

#elif defined(PLATFORM_UWP)

    void Mouse::UWPPointerWheelChanged(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args) {
        if (args->CurrentPoint->Properties->IsHorizontalMouseWheel)
            _NextMouseState.MouseWheelXDelta = args->CurrentPoint->Properties->MouseWheelDelta / WHEEL_DELTA;
        else _NextMouseState.MouseWheelYDelta = args->CurrentPoint->Properties->MouseWheelDelta / WHEEL_DELTA;
    }

    void Mouse::UWPPointerButtonEvent(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args) {
        auto ptrPoint = args->CurrentPoint;

        if (ptrPoint->PointerDevice->PointerDeviceType == Windows::Devices::Input::PointerDeviceType::Mouse) {
            auto ptrPointProps = ptrPoint->Properties;

            _NextMouseState.ButtonsDown[MOUSE_BUTTON_LEFT] = ptrPointProps->IsLeftButtonPressed;
            _NextMouseState.ButtonsDown[MOUSE_BUTTON_MIDDLE] = ptrPointProps->IsMiddleButtonPressed;
            _NextMouseState.ButtonsDown[MOUSE_BUTTON_RIGHT] = ptrPointProps->IsRightButtonPressed;
        }
    }

#endif

    // Public Fields

    EventHandler<MouseButtonEventArgs> Mouse::OnMouseButtonPressed;
    EventHandler<MouseButtonEventArgs> Mouse::OnMouseButtonReleased;
    EventHandler<MouseMovedEventArgs> Mouse::OnMouseMoved;
    EventHandler<MouseScrollChangedEventArgs> Mouse::OnMouseScrollXChanged;
    EventHandler<MouseScrollChangedEventArgs> Mouse::OnMouseScrollYChanged;

    // Public Methods

    void Mouse::CancelButton(EMouseButton button_) {
        _CurrentMouseState.ButtonsDown[button_] = false;
        _NextMouseState.ButtonsDown[button_] = false;
    }

    TVector2 Mouse::GetMousePosition() {
        auto pos = _CurrentMouseState.Position;

        // Apply translation
        pos.X = (pos.X + _MouseOffset.X) * _MouseScale.X;
        pos.Y = (pos.Y + _MouseOffset.Y) * _MouseScale.Y;

        return pos;
    }

    int Mouse::GetMouseWheelXDelta() {
        return _CurrentMouseState.MouseWheelXDelta;
    }

    int Mouse::GetMouseWheelYDelta() {
        return _CurrentMouseState.MouseWheelYDelta;
    }

    void Mouse::Init() {
#if defined(PLATFORM_DESKTOP)

        // Register glfw callbacks
        glfwSetMouseButtonCallback((GLFWwindow *)Window::WindowPtr, Mouse::GLFWMouseButtonCallback);
        glfwSetScrollCallback((GLFWwindow *)Window::WindowPtr, Mouse::GLFWScrollCallback);

#elif defined(PLATFORM_UWP)

        // Register UWP callbacks
        auto window = CoreWindow::GetForCurrentThread();

        window->PointerWheelChanged += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::PointerEventArgs ^>(&UWPPointerWheelChanged);
        window->PointerPressed += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::PointerEventArgs ^>(&UWPPointerButtonEvent);
        window->PointerReleased += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::PointerEventArgs ^>(&UWPPointerButtonEvent);

#endif
    }

    bool Mouse::IsButtonDown(EMouseButton button_) {
        return _CurrentMouseState.ButtonsDown[button_];
    }

    bool Mouse::IsButtonPressed(EMouseButton button_) {
        return _CurrentMouseState.ButtonsDown[button_] != _LastMouseState.ButtonsDown[button_] && _CurrentMouseState.ButtonsDown[button_] == true;
    }

    bool Mouse::IsButtonReleased(EMouseButton button_) {
        return _CurrentMouseState.ButtonsDown[button_] != _LastMouseState.ButtonsDown[button_] && _CurrentMouseState.ButtonsDown[button_] == false;
    }

    void Mouse::PollInputs() {
        // Mouse position events
        if (_LastMouseState.Position != _CurrentMouseState.Position) {
            OnMouseMoved({_CurrentMouseState.Position, _CurrentMouseState.Position - _LastMouseState.Position});
        }

        // Mouse button events
        for (auto i = 0; i < 3; i++) {
            if (IsButtonPressed((EMouseButton)i)) {
                OnMouseButtonPressed({static_cast<EMouseButton>(i), true});
            } else if (IsButtonReleased((EMouseButton)i)) {
                OnMouseButtonReleased({static_cast<EMouseButton>(i), false});
            }
        }

        // Scroll wheel events
        if (_CurrentMouseState.MouseWheelXDelta != _LastMouseState.MouseWheelXDelta) {
            OnMouseScrollXChanged({_CurrentMouseState.MouseWheelXDelta});
        }
        if (_CurrentMouseState.MouseWheelYDelta != _LastMouseState.MouseWheelYDelta) {
            OnMouseScrollYChanged({_CurrentMouseState.MouseWheelYDelta});
        }

        // Update last frame state
        _LastMouseState = _CurrentMouseState;

        // Final fetch for the position before pushing it to the current stack
        _NextMouseState.Position = InternalGetMousePosition();

        // Push new data to current state
        _CurrentMouseState = _NextMouseState;

        // Reset next state's scroll wheel
        _NextMouseState.MouseWheelYDelta = 0;
        _NextMouseState.MouseWheelXDelta = 0;
    }

    void Mouse::SetOffset(float ox_, float oy_) {
        _MouseOffset = {ox_, oy_};
    }

    void Mouse::SetScale(float sx_, float sy_) {
        _MouseScale = {sx_, sy_};
    }
}
