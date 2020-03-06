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

#include "ngine/input/Gamepad.hpp"

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#endif

namespace ngine::input {
    float Gamepad::m_currentAxisValue[4][AXIS_COUNT];
    bool Gamepad::m_currentButtonState[4][BUTTON_COUNT];
    bool Gamepad::m_previousButtonState[4][BUTTON_COUNT];
    bool Gamepad::m_ready[4];

#if defined(PLATFORM_UWP)
    Windows::Gaming::Input::Gamepad ^Gamepad::m_UWPGamepads[4] = {nullptr, nullptr, nullptr, nullptr};
#endif

    GamepadAxis Gamepad::_getAxis(int axis) {
        GamepadAxis axisEnum = GamepadAxis::Unknown;
#if defined(PLATFORM_DESKTOP)
        switch (axis) {
            case GLFW_GAMEPAD_AXIS_LEFT_X: axisEnum = GamepadAxis::LeftX; break;
            case GLFW_GAMEPAD_AXIS_LEFT_Y: axisEnum = GamepadAxis::LeftY; break;
            case GLFW_GAMEPAD_AXIS_RIGHT_X: axisEnum = GamepadAxis::RightX; break;
            case GLFW_GAMEPAD_AXIS_RIGHT_Y: axisEnum = GamepadAxis::RightY; break;
            case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER: axisEnum = GamepadAxis::LeftTrigger; break;
            case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER: axisEnum = GamepadAxis::RightTrigger; break;
        }
#elif defined(PLATFORM_UWP)
        axisEnum = (GamepadAxis) axis;     // UWP will provide the correct axis
#endif
        return axisEnum;
    }

    GamepadButton Gamepad::_getButton(int button) {
        GamepadButton btn = GamepadButton::Unknown;
#if defined(PLATFORM_DESKTOP)
        switch (button) {
            case GLFW_GAMEPAD_BUTTON_Y: btn = GamepadButton::RightFaceUp; break;
            case GLFW_GAMEPAD_BUTTON_B: btn = GamepadButton::RightFaceRight; break;
            case GLFW_GAMEPAD_BUTTON_A: btn = GamepadButton::RightFaceDown; break;
            case GLFW_GAMEPAD_BUTTON_X: btn = GamepadButton::RightFaceLeft; break;

            case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER: btn = GamepadButton::LeftTrigger1; break;
            case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER: btn = GamepadButton::RightTrigger1; break;

            // Trigger 2 set by axis test in poll

            case GLFW_GAMEPAD_BUTTON_BACK: btn = GamepadButton::MiddleLeft; break;
            case GLFW_GAMEPAD_BUTTON_GUIDE: btn = GamepadButton::Middle; break;
            case GLFW_GAMEPAD_BUTTON_START: btn = GamepadButton::MiddleRight; break;

            case GLFW_GAMEPAD_BUTTON_DPAD_UP: btn = GamepadButton::LeftFaceUp; break;
            case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT: btn = GamepadButton::LeftFaceRight; break;
            case GLFW_GAMEPAD_BUTTON_DPAD_DOWN: btn = GamepadButton::LeftFaceDown; break;
            case GLFW_GAMEPAD_BUTTON_DPAD_LEFT: btn = GamepadButton::LeftFaceLeft; break;

            case GLFW_GAMEPAD_BUTTON_LEFT_THUMB: btn = GamepadButton::LeftThumb; break;
            case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB: btn = GamepadButton::RightThumb; break;
        }
#elif defined(PLATFORM_UWP)
        // Provided correctly
        btn = (GamepadButton) button;
#endif
        return btn;
    }

#if defined(PLATFORM_UWP)
    void Gamepad::_UWPGamepadAdded(Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^args) {
        for (auto i = 0; i < 4; i++) {
            if (m_UWPGamepads[i] == nullptr) {
                m_UWPGamepads[i] = args;
                m_ready[i] = true;
                break;
            }
        }
    }


    void Gamepad::_UWPGamepadRemoved(Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^args) {
        for (auto i = 0; i < 4; i++) {
            if (m_UWPGamepads[i] == args) {
                m_UWPGamepads[i] = nullptr;
                m_ready[i] = false;
                break;
            }
        }
    }
#endif

    void Gamepad::initialize() {
#if defined(PLATFORM_UWP)
        Windows::Gaming::Input::Gamepad::GamepadAdded += ref new Windows::Foundation::EventHandler<Windows::Gaming::Input::Gamepad ^>(&_UWPGamepadAdded);
        Windows::Gaming::Input::Gamepad::GamepadRemoved += ref new Windows::Foundation::EventHandler<Windows::Gaming::Input::Gamepad ^>(&_UWPGamepadRemoved);
#endif
    }

    float Gamepad::getAxisValue(GamepadNumber pad, GamepadAxis axis) {
        return m_currentAxisValue[(int) pad][(int) axis];
    }

    bool Gamepad::isAvailable(GamepadNumber pad) {
        return m_ready[(int) pad];
    }

    bool Gamepad::isButtonDown(GamepadNumber pad, GamepadButton button) {
        return m_currentButtonState[(int) pad][(int) button];
    }

    bool Gamepad::isButtonPressed(GamepadNumber pad, GamepadButton button) {
        return m_currentButtonState[(int) pad][(int) button] != m_previousButtonState[(int) pad][(int) button] && m_currentButtonState[(int) pad][(int) button];
    }

    bool Gamepad::isButtonReleased(GamepadNumber pad, GamepadButton button) {
        return m_currentButtonState[(int) pad][(int) button] != m_previousButtonState[(int) pad][(int) button] && !m_currentButtonState[(int) pad][(int) button];
    }

    void Gamepad::pollInputs() {
        // TODO: Check axis value parity.

#if defined(PLATFORM_DESKTOP)
        // Check for ready controllers
        for (auto i = 0; i < 4; i++) {
            m_ready[i] = glfwJoystickPresent(i) == GLFW_TRUE;
        }

        // Check for inputs
        for (auto i = 0; i < 4; i++) {
            if (isAvailable((GamepadNumber) i)) {
                for (auto j = 0; j < BUTTON_COUNT; j++)
                    m_previousButtonState[i][j] = m_currentButtonState[i][j];

                GLFWgamepadstate state;
                glfwGetGamepadState(i, &state);
                auto buttons = state.buttons;

                for (auto j = 0; (buttons != nullptr) && (j <= GLFW_GAMEPAD_BUTTON_DPAD_LEFT) && (j < BUTTON_COUNT); j++) {
                    GamepadButton b = _getButton(j);

                    m_currentButtonState[i][(int) b] = buttons[j] == GLFW_PRESS;
                }

                // Get axis values
                auto axes = state.axes;

                for (auto j = 0; (axes != nullptr) && (j <= GLFW_GAMEPAD_AXIS_LAST) && (j < AXIS_COUNT); j++){
                    GamepadAxis a = _getAxis(j);

                    m_currentAxisValue[i][(int) a] = axes[j];
                }

                // Register buttons for pressure triggers
                m_currentButtonState[i][(int) GamepadButton::LeftTrigger2] = m_currentAxisValue[i][(int) GamepadAxis::LeftTrigger] > 0.1;
                m_currentButtonState[i][(int) GamepadButton::RightTrigger2] = m_currentAxisValue[i][(int) GamepadAxis::RightTrigger] > 0.1;
            }
        }
#elif defined(PLATFORM_UWP)
        // Get gamepad states
        for (auto i = 0; i < 4; i++) {
            if (isAvailable((GamepadNumber)i)) {
                auto gamepad = m_UWPGamepads[i];
                auto reading = gamepad->GetCurrentReading();

                for (auto j = 0; j < BUTTON_COUNT; j++) m_previousButtonState[i][j] = m_currentButtonState[i][j];

                // Get buttons
                m_currentButtonState[i][(int) GamepadButton::RightFaceDown] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::A) == Windows::Gaming::Input::GamepadButtons::A);
                m_currentButtonState[i][(int)GamepadButton::RightFaceRight] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::B) == Windows::Gaming::Input::GamepadButtons::B);
                m_currentButtonState[i][(int)GamepadButton::RightFaceLeft] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::X) == Windows::Gaming::Input::GamepadButtons::X);
                m_currentButtonState[i][(int) GamepadButton::RightFaceUp] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::Y) == Windows::Gaming::Input::GamepadButtons::Y);

                m_currentButtonState[i][(int)GamepadButton::LeftTrigger1] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::LeftShoulder) == Windows::Gaming::Input::GamepadButtons::LeftShoulder);
                m_currentButtonState[i][(int)GamepadButton::RightTrigger1] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::RightShoulder) == Windows::Gaming::Input::GamepadButtons::RightShoulder);

                m_currentButtonState[i][(int)GamepadButton::MiddleLeft] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::View) == Windows::Gaming::Input::GamepadButtons::View);
                m_currentButtonState[i][(int)GamepadButton::MiddleRight] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::Menu) == Windows::Gaming::Input::GamepadButtons::Menu);

                m_currentButtonState[i][(int)GamepadButton::LeftFaceUp] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadUp) == Windows::Gaming::Input::GamepadButtons::DPadUp);
                m_currentButtonState[i][(int)GamepadButton::LeftFaceRight] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadRight) == Windows::Gaming::Input::GamepadButtons::DPadRight);
                m_currentButtonState[i][(int)GamepadButton::LeftFaceDown] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadDown) == Windows::Gaming::Input::GamepadButtons::DPadDown);
                m_currentButtonState[i][(int)GamepadButton::LeftFaceLeft] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadLeft) == Windows::Gaming::Input::GamepadButtons::DPadLeft);

                m_currentButtonState[i][(int)GamepadButton::Middle] = false; // Not supported

                // Get axis
                m_currentAxisValue[i][(int) GamepadAxis::LeftX] = (float)reading.LeftThumbstickX;
                m_currentAxisValue[i][(int)GamepadAxis::LeftY] = (float)-reading.LeftThumbstickY;
                m_currentAxisValue[i][(int)GamepadAxis::RightX] = (float)reading.RightThumbstickX;
                m_currentAxisValue[i][(int)GamepadAxis::RightY] = (float)-reading.RightThumbstickY;
                m_currentAxisValue[i][(int)GamepadAxis::LeftTrigger] = (float)reading.LeftTrigger;
                m_currentAxisValue[i][(int)GamepadAxis::RightTrigger] = (float)reading.RightTrigger;

                // Register buttons for pressure triggers
                m_currentButtonState[i][(int)GamepadButton::LeftTrigger2] = m_currentAxisValue[i][(int)GamepadAxis::LeftTrigger] > 0.1;
                m_currentButtonState[i][(int)GamepadButton::RightTrigger2] = m_currentAxisValue[i][(int)GamepadAxis::RightTrigger] > 0.1;
            }
        }
#endif
    }
}
