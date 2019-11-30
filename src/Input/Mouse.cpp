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

#include "Mouse.h"

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#endif

#include "../Window.h"

namespace NerdThings::Ngine::Input {
    Vector2 Mouse::_internalGetMousePosition() {
        auto pos = Vector2::Zero;
#if defined(PLATFORM_DESKTOP)
        double x = 0, y = 0;
        glfwGetCursorPos(m_attachedWindow->m_GLFWWindow, &x, &y);
        pos.X = static_cast<float>(x);
        pos.Y = static_cast<float>(y);
#elif defined(PLATFORM_UWP)
        auto window = CoreWindow::GetForCurrentThread();
        auto pointerPos = window->PointerPosition;

        // Get display resolution scale (Fixes a big bad bug)
        auto resScale = (int)DisplayInformation::GetForCurrentView()->ResolutionScale;
        auto sf = (float)resScale / 100.0f;

        // Translate pointer position
        pointerPos.X *= sf;
        pointerPos.Y *= sf;

        // Translate bound locations
        auto wPosX = window->Bounds.X * sf;
        auto wPosY = window->Bounds.Y * sf;

        // Save position
        pos.X = pointerPos.X - wPosX;
        pos.Y = pointerPos.Y - wPosY;
#endif

        return pos;
    }

    Mouse::Mouse(Window *window_) : m_attachedWindow(window_) {
#if defined(PLATFORM_DESKTOP)
        // Register glfw callbacks
        glfwSetMouseButtonCallback(m_attachedWindow->m_GLFWWindow, Mouse::GLFWMouseButtonCallback);
        glfwSetScrollCallback(m_attachedWindow->m_GLFWWindow, Mouse::GLFWScrollCallback);
#elif defined(PLATFORM_UWP)
        // Register UWP callbacks
        auto window = CoreWindow::GetForCurrentThread();
        window->PointerWheelChanged += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::PointerEventArgs ^>(&UWPPointerWheelChanged);
        window->PointerPressed += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::PointerEventArgs ^>(&UWPPointerButtonEvent);
        window->PointerReleased += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::PointerEventArgs ^>(&UWPPointerButtonEvent);
#endif
    }

#if defined(PLATFORM_DESKTOP)

    void Mouse::GLFWMouseButtonCallback(GLFWwindow *window_, int button_, int action_, int mods_) {
        if (button_ <= MOUSE_BUTTON_RIGHT) { // GLFW supports more than our 3 buttons
            auto mouse = ((Window *)glfwGetWindowUserPointer(window_))->GetMouse();
            mouse->m_nextMouseState.ButtonsDown[button_] = (action_ == GLFW_PRESS);
        }
    }

    void Mouse::GLFWScrollCallback(GLFWwindow *window_, double x_, double y_) {
        auto mouse = ((Window *)glfwGetWindowUserPointer(window_))->GetMouse();
        mouse->m_nextMouseState.MouseWheelXDelta = static_cast<int>(x_);
        mouse->m_nextMouseState.MouseWheelYDelta = static_cast<int>(y_);
    }

#elif defined(PLATFORM_UWP)

    void Mouse::UWPPointerWheelChanged(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args) {
        if (args->CurrentPoint->Properties->IsHorizontalMouseWheel)
            m_nextMouseState.MouseWheelXDelta = args->CurrentPoint->Properties->MouseWheelDelta / WHEEL_DELTA;
        else m_nextMouseState.MouseWheelYDelta = args->CurrentPoint->Properties->MouseWheelDelta / WHEEL_DELTA;
    }

    void Mouse::UWPPointerButtonEvent(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args) {
        auto ptrPoint = args->CurrentPoint;

        if (ptrPoint->PointerDevice->PointerDeviceType == Windows::Devices::Input::PointerDeviceType::Mouse) {
            auto ptrPointProps = ptrPoint->Properties;

            m_nextMouseState.ButtonsDown[MOUSE_BUTTON_LEFT] = ptrPointProps->IsLeftButtonPressed;
            m_nextMouseState.ButtonsDown[MOUSE_BUTTON_MIDDLE] = ptrPointProps->IsMiddleButtonPressed;
            m_nextMouseState.ButtonsDown[MOUSE_BUTTON_RIGHT] = ptrPointProps->IsRightButtonPressed;
        }
    }

#endif

    Mouse *Mouse::GetCurrent() {
        auto w = Window::GetCurrent();
        return w == nullptr ? nullptr : w->GetMouse();
    }

    void Mouse::CancelButton(MouseButton button_) {
        m_currentMouseState.ButtonsDown[button_] = false;
        m_nextMouseState.ButtonsDown[button_] = false;
    }

    Vector2 Mouse::GetMousePosition() {
        auto pos = m_currentMouseState.Position;

        // Apply translation
        pos.X = (pos.X + m_mouseOffset.X) * m_mouseScale.X;
        pos.Y = (pos.Y + m_mouseOffset.Y) * m_mouseScale.Y;

        return pos;
    }

    int Mouse::GetMouseWheelXDelta() {
        return m_currentMouseState.MouseWheelXDelta;
    }

    int Mouse::GetMouseWheelYDelta() {
        return m_currentMouseState.MouseWheelYDelta;
    }

    bool Mouse::IsButtonDown(MouseButton button_) {
        return m_currentMouseState.ButtonsDown[button_];
    }

    bool Mouse::IsButtonPressed(MouseButton button_) {
        return m_currentMouseState.ButtonsDown[button_] != m_previousMouseState.ButtonsDown[button_] && m_currentMouseState.ButtonsDown[button_] == true;
    }

    bool Mouse::IsButtonReleased(MouseButton button_) {
        return m_currentMouseState.ButtonsDown[button_] != m_previousMouseState.ButtonsDown[button_] && m_currentMouseState.ButtonsDown[button_] == false;
    }

    void Mouse::PollInputs() {
        // Mouse position events
        if (m_previousMouseState.Position != m_currentMouseState.Position) {
            OnMouseMoved({m_currentMouseState.Position, m_currentMouseState.Position - m_previousMouseState.Position});
        }

        // Mouse button events
        for (auto i = 0; i < 3; i++) {
            if (IsButtonPressed((MouseButton)i)) {
                OnMouseButtonPressed({static_cast<MouseButton>(i), true});
            } else if (IsButtonReleased((MouseButton)i)) {
                OnMouseButtonReleased({static_cast<MouseButton>(i), false});
            }
        }

        // Scroll wheel events
        if (m_currentMouseState.MouseWheelXDelta != m_previousMouseState.MouseWheelXDelta) {
            OnMouseScrollXChanged({m_currentMouseState.MouseWheelXDelta});
        }
        if (m_currentMouseState.MouseWheelYDelta != m_previousMouseState.MouseWheelYDelta) {
            OnMouseScrollYChanged({m_currentMouseState.MouseWheelYDelta});
        }

        // Update last frame state
        m_previousMouseState = m_currentMouseState;

        // Final fetch for the position before pushing it to the current stack
        m_nextMouseState.Position = _internalGetMousePosition();

        // Push new data to current state
        m_currentMouseState = m_nextMouseState;

        // Reset next state's scroll wheel
        m_nextMouseState.MouseWheelYDelta = 0;
        m_nextMouseState.MouseWheelXDelta = 0;
    }

    void Mouse::SetOffset(float ox_, float oy_) {
        m_mouseOffset = {ox_, oy_};
    }

    void Mouse::SetScale(float sx_, float sy_) {
        m_mouseScale = {sx_, sy_};
    }
}
