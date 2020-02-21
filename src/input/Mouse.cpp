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

#include "input/Mouse.hpp"

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#endif

#include "Window.hpp"

namespace ngine::input {
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

    Mouse::Mouse(Window *window) : m_attachedWindow(window) {
#if defined(PLATFORM_DESKTOP)
        // Register glfw callbacks
        glfwSetMouseButtonCallback(m_attachedWindow->m_GLFWWindow, Mouse::_GLFWMouseButtonCallback);
        glfwSetScrollCallback(m_attachedWindow->m_GLFWWindow, Mouse::_GLFWScrollCallback);
#elif defined(PLATFORM_UWP)
        if (m_UWPMouse != nullptr)
            throw std::runtime_error("On UWP only one mouse can be created.");

        // Save
        m_UWPMouse = this;

        // Register UWP callbacks
        auto UWPwindow = CoreWindow::GetForCurrentThread();
        UWPwindow->PointerWheelChanged += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::PointerEventArgs ^>(&_UWPPointerWheelChanged);
        UWPwindow->PointerPressed += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::PointerEventArgs ^>(&_UWPPointerButtonEvent);
        UWPwindow->PointerReleased += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::PointerEventArgs ^>(&_UWPPointerButtonEvent);
        UWPwindow->PointerMoved += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::PointerEventArgs ^>(&_UWPPointerMovedEvent);
#endif
    }

#if defined(PLATFORM_DESKTOP)

    void Mouse::_GLFWMouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
        if (button <= static_cast<int>(MouseButton::Middle)) { // GLFW supports more than our 3 buttons
            auto mouse = ((Window *) glfwGetWindowUserPointer(window))->getMouse();
            mouse->m_nextMouseState.ButtonsDown[button] = (action == GLFW_PRESS);
        }
    }

    void Mouse::_GLFWScrollCallback(GLFWwindow *window, double x, double y) {
        auto mouse = ((Window *) glfwGetWindowUserPointer(window))->getMouse();
        mouse->m_nextMouseState.MouseWheelXDelta = static_cast<int>(x);
        mouse->m_nextMouseState.MouseWheelYDelta = static_cast<int>(y);
    }

#elif defined(PLATFORM_UWP)

    Mouse *Mouse::m_UWPMouse = nullptr;

    void Mouse::_UWPPointerWheelChanged(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args) {
        if (args->CurrentPoint->Properties->IsHorizontalMouseWheel)
            m_UWPMouse->m_nextMouseState.MouseWheelXDelta = args->CurrentPoint->Properties->MouseWheelDelta / WHEEL_DELTA;
        else m_UWPMouse->m_nextMouseState.MouseWheelYDelta = args->CurrentPoint->Properties->MouseWheelDelta / WHEEL_DELTA;
    }

    void Mouse::_UWPPointerButtonEvent(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args) {
        auto ptrPoint = args->CurrentPoint;

        if (ptrPoint->PointerDevice->PointerDeviceType == Windows::Devices::Input::PointerDeviceType::Mouse) {
            auto ptrPointProps = ptrPoint->Properties;

            m_UWPMouse->m_nextMouseState.ButtonsDown[MOUSE_BUTTON_LEFT] = ptrPointProps->IsLeftButtonPressed;
            m_UWPMouse->m_nextMouseState.ButtonsDown[MOUSE_BUTTON_MIDDLE] = ptrPointProps->IsMiddleButtonPressed;
            m_UWPMouse->m_nextMouseState.ButtonsDown[MOUSE_BUTTON_RIGHT] = ptrPointProps->IsRightButtonPressed;
        }
    }

    void Mouse::_UWPPointerMovedEvent(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args) {
        // Fetch the new position
        m_UWPMouse->m_nextMouseState.Position = m_UWPMouse->_internalGetMousePosition();
    }

#endif

    Mouse *Mouse::GetCurrent() {
        auto w = Window::GetCurrent();
        return w == nullptr ? nullptr : w->getMouse();
    }

    void Mouse::cancelButton(MouseButton button) {
        m_currentMouseState.ButtonsDown[static_cast<int>(button)] = false;
        m_nextMouseState.ButtonsDown[static_cast<int>(button)] = false;
    }

    Vector2 Mouse::getMousePosition() {
        auto pos = m_currentMouseState.Position;

        // Apply translation
        pos.X = (pos.X + m_mouseOffset.X) * m_mouseScale.X;
        pos.Y = (pos.Y + m_mouseOffset.Y) * m_mouseScale.Y;

        return pos;
    }

    int Mouse::getMouseWheelXDelta() {
        return m_currentMouseState.MouseWheelXDelta;
    }

    int Mouse::getMouseWheelYDelta() {
        return m_currentMouseState.MouseWheelYDelta;
    }

    bool Mouse::isButtonDown(MouseButton button) {
        return m_currentMouseState.ButtonsDown[static_cast<int>(button)];
    }

    bool Mouse::isButtonPressed(MouseButton button) {
        return m_currentMouseState.ButtonsDown[static_cast<int>(button)] != m_previousMouseState.ButtonsDown[static_cast<int>(button)] && m_currentMouseState.ButtonsDown[static_cast<int>(button)] == true;
    }

    bool Mouse::isButtonReleased(MouseButton button) {
        return m_currentMouseState.ButtonsDown[static_cast<int>(button)] != m_previousMouseState.ButtonsDown[static_cast<int>(button)] && m_currentMouseState.ButtonsDown[static_cast<int>(button)] == false;
    }

    void Mouse::pollInputs() {
        // Mouse position events
        if (m_previousMouseState.Position != m_currentMouseState.Position) {
            EventDispatcher::fire(MovedEvent(this, m_attachedWindow, m_currentMouseState.Position, m_currentMouseState.Position - m_previousMouseState.Position));
        }

        // Mouse button events
        for (auto i = 0; i < 3; i++) {
            if (isButtonPressed((MouseButton) i)) {
                EventDispatcher::fire(ButtonPressedEvent(this, m_attachedWindow, (MouseButton) i));
            } else if (isButtonReleased((MouseButton) i)) {
                EventDispatcher::fire(ButtonReleasedEvent(this, m_attachedWindow, (MouseButton) i));
            }
        }

        // Scroll wheel events
        if (m_currentMouseState.MouseWheelXDelta != m_previousMouseState.MouseWheelXDelta) {
            EventDispatcher::fire(WheelXChangedEvent(this, m_attachedWindow, m_currentMouseState.MouseWheelXDelta));
        }
        if (m_currentMouseState.MouseWheelYDelta != m_previousMouseState.MouseWheelYDelta) {
            EventDispatcher::fire(WheelYChangedEvent(this, m_attachedWindow, m_currentMouseState.MouseWheelYDelta));
        }

        // Update last frame state
        m_previousMouseState = m_currentMouseState;

        // Final fetch for the position before pushing it to the current stack
#ifndef PLATFORM_UWP
        m_nextMouseState.Position = _internalGetMousePosition();
#endif

        // Push new data to current state
        m_currentMouseState = m_nextMouseState;

        // Reset next state's scroll wheel
        m_nextMouseState.MouseWheelYDelta = 0;
        m_nextMouseState.MouseWheelXDelta = 0;
    }

    void Mouse::setOffset(float xOffset, float yOffset) {
        m_mouseOffset = {xOffset, yOffset};
    }

    void Mouse::setScale(float xScale, float yScale) {
        m_mouseScale = {xScale, yScale};
    }
}
