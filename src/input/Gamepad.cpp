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

#include "input/Gamepad.hpp"

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#elif defined(PLATFORM_UWP)
#endif

namespace ngine::input {
    float Gamepad::m_currentAxisValue[4][GAMEPAD_AXIS_RIGHT_TRIGGER + 1];
    bool Gamepad::m_currentButtonState[4][GAMEPAD_BUTTON_RIGHT_THUMB + 1];
    bool Gamepad::m_previousButtonState[4][GAMEPAD_BUTTON_RIGHT_THUMB + 1];
    bool Gamepad::m_ready[4];

#if defined(PLATFORM_UWP)
    Windows::Gaming::Input::Gamepad ^Gamepad::m_UWPGamepads[4] = {nullptr, nullptr, nullptr, nullptr};
#endif

    GamepadAxis Gamepad::_getAxis(int axis) {
        GamepadAxis axisEnum = GAMEPAD_AXIS_UNKNOWN;
#if defined(PLATFORM_DESKTOP)
        switch (axis) {
            case GLFW_GAMEPAD_AXIS_LEFT_X: axisEnum = GAMEPAD_AXIS_LEFT_X; break;
            case GLFW_GAMEPAD_AXIS_LEFT_Y: axisEnum = GAMEPAD_AXIS_LEFT_Y; break;
            case GLFW_GAMEPAD_AXIS_RIGHT_X: axisEnum = GAMEPAD_AXIS_RIGHT_X; break;
            case GLFW_GAMEPAD_AXIS_RIGHT_Y: axisEnum = GAMEPAD_AXIS_RIGHT_Y; break;
            case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER: axisEnum = GAMEPAD_AXIS_LEFT_TRIGGER; break;
            case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER: axisEnum = GAMEPAD_AXIS_RIGHT_TRIGGER; break;
        }
#elif defined(PLATFORM_UWP)
        axis = (GamepadAxis) axis;     // UWP will provide the correct axis
#endif
        return axisEnum;
    }

    GamepadButton Gamepad::_getButton(int button) {
        GamepadButton btn = GAMEPAD_BUTTON_UNKNOWN;
#if defined(PLATFORM_DESKTOP)
        switch (button) {
            case GLFW_GAMEPAD_BUTTON_Y: btn = GAMEPAD_BUTTON_RIGHT_FACE_UP; break;
            case GLFW_GAMEPAD_BUTTON_B: btn = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT; break;
            case GLFW_GAMEPAD_BUTTON_A: btn = GAMEPAD_BUTTON_RIGHT_FACE_DOWN; break;
            case GLFW_GAMEPAD_BUTTON_X: btn = GAMEPAD_BUTTON_RIGHT_FACE_LEFT; break;

            case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER: btn = GAMEPAD_BUTTON_LEFT_TRIGGER_1; break;
            case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER: btn = GAMEPAD_BUTTON_RIGHT_TRIGGER_1; break;

            case GLFW_GAMEPAD_BUTTON_BACK: btn = GAMEPAD_BUTTON_MIDDLE_LEFT; break;
            case GLFW_GAMEPAD_BUTTON_GUIDE: btn = GAMEPAD_BUTTON_MIDDLE; break;
            case GLFW_GAMEPAD_BUTTON_START: btn = GAMEPAD_BUTTON_MIDDLE_RIGHT; break;

            case GLFW_GAMEPAD_BUTTON_DPAD_UP: btn = GAMEPAD_BUTTON_LEFT_FACE_UP; break;
            case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT: btn = GAMEPAD_BUTTON_LEFT_FACE_RIGHT; break;
            case GLFW_GAMEPAD_BUTTON_DPAD_DOWN: btn = GAMEPAD_BUTTON_LEFT_FACE_DOWN; break;
            case GLFW_GAMEPAD_BUTTON_DPAD_LEFT: btn = GAMEPAD_BUTTON_LEFT_FACE_LEFT; break;

            case GLFW_GAMEPAD_BUTTON_LEFT_THUMB: btn = GAMEPAD_BUTTON_LEFT_THUMB; break;
            case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB: btn = GAMEPAD_BUTTON_RIGHT_THUMB; break;
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
        return m_currentAxisValue[pad][axis];
    }

    bool Gamepad::isAvailable(GamepadNumber pad) {
        return m_ready[pad];
    }

    bool Gamepad::isButtonDown(GamepadNumber pad, GamepadButton button) {
        return m_currentButtonState[pad][button];
    }

    bool Gamepad::isButtonPressed(GamepadNumber pad, GamepadButton button) {
        return m_currentButtonState[pad][button] != m_previousButtonState[pad][button] && m_currentButtonState[pad][button];
    }

    bool Gamepad::isButtonReleased(GamepadNumber pad, GamepadButton button) {
        return m_currentButtonState[pad][button] != m_previousButtonState[pad][button] && !m_currentButtonState[pad][button];
    }

    void Gamepad::pollInputs() {
#if defined(PLATFORM_DESKTOP)
        // Check for ready controllers
        for (auto i = 0; i < 4; i++) {
            m_ready[i] = glfwJoystickPresent(i) == GLFW_TRUE;
        }

        // Check for inputs
        for (auto i = 0; i < 4; i++) {
            if (isAvailable((GamepadNumber) i)) {
                for (auto j = 0; j <= GAMEPAD_BUTTON_RIGHT_THUMB; j++) m_previousButtonState[i][j] = m_currentButtonState[i][j];

                GLFWgamepadstate state;
                glfwGetGamepadState(i, &state);
                auto buttons = state.buttons;

                for (auto j = 0; (buttons != nullptr) && (j <= GLFW_GAMEPAD_BUTTON_DPAD_LEFT) && (j <= GAMEPAD_BUTTON_RIGHT_THUMB); j++) {
                    GamepadButton b = _getButton(j);

                    m_currentButtonState[i][b] = buttons[j] == GLFW_PRESS;
                }

                // Get axis values
                auto axes = state.axes;

                for (auto j = 0; (axes != nullptr) && (j <= GLFW_GAMEPAD_AXIS_LAST) && (j <= GAMEPAD_AXIS_RIGHT_TRIGGER); j++){
                    GamepadAxis a = _getAxis(j);

                    m_currentAxisValue[i][a] = axes[j];
                }

                // Register buttons for pressure triggers
                m_currentButtonState[i][GAMEPAD_BUTTON_LEFT_TRIGGER_2] = m_currentAxisValue[i][GAMEPAD_AXIS_LEFT_TRIGGER] > 0.1;
                m_currentButtonState[i][GAMEPAD_BUTTON_RIGHT_TRIGGER_2] = m_currentAxisValue[i][GAMEPAD_AXIS_RIGHT_TRIGGER] > 0.1;
            }
        }
#elif defined(PLATFORM_UWP)
        // Get gamepad states
        for (auto i = 0; i < 4; i++) {
            if (IsAvailable((GamepadNumber)i)) {
                auto gamepad = m_UWPGamepads[i];
                auto reading = gamepad->GetCurrentReading();

                for (auto j = 0; j <= GAMEPAD_BUTTON_RIGHT_THUMB; j++) m_previousButtonState[i][j] = m_currentButtonState[i][j];

                // Get buttons
                m_currentButtonState[i][GAMEPAD_BUTTON_RIGHT_FACE_DOWN] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::A) == Windows::Gaming::Input::GamepadButtons::A);
                m_currentButtonState[i][GAMEPAD_BUTTON_RIGHT_FACE_RIGHT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::B) == Windows::Gaming::Input::GamepadButtons::B);
                m_currentButtonState[i][GAMEPAD_BUTTON_RIGHT_FACE_LEFT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::X) == Windows::Gaming::Input::GamepadButtons::X);
                m_currentButtonState[i][GAMEPAD_BUTTON_RIGHT_FACE_UP] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::Y) == Windows::Gaming::Input::GamepadButtons::Y);

                m_currentButtonState[i][GAMEPAD_BUTTON_LEFT_TRIGGER_1] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::LeftShoulder) == Windows::Gaming::Input::GamepadButtons::LeftShoulder);
                m_currentButtonState[i][GAMEPAD_BUTTON_RIGHT_TRIGGER_1] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::RightShoulder) == Windows::Gaming::Input::GamepadButtons::RightShoulder);

                m_currentButtonState[i][GAMEPAD_BUTTON_MIDDLE_LEFT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::View) == Windows::Gaming::Input::GamepadButtons::View);
                m_currentButtonState[i][GAMEPAD_BUTTON_MIDDLE_RIGHT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::Menu) == Windows::Gaming::Input::GamepadButtons::Menu);

                m_currentButtonState[i][GAMEPAD_BUTTON_LEFT_FACE_UP] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadUp) == Windows::Gaming::Input::GamepadButtons::DPadUp);
                m_currentButtonState[i][GAMEPAD_BUTTON_LEFT_FACE_RIGHT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadRight) == Windows::Gaming::Input::GamepadButtons::DPadRight);
                m_currentButtonState[i][GAMEPAD_BUTTON_LEFT_FACE_DOWN] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadDown) == Windows::Gaming::Input::GamepadButtons::DPadDown);
                m_currentButtonState[i][GAMEPAD_BUTTON_LEFT_FACE_LEFT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadLeft) == Windows::Gaming::Input::GamepadButtons::DPadLeft);

                m_currentButtonState[i][GAMEPAD_BUTTON_MIDDLE] = false; // Not supported

                // Get axis
                m_currentAxisValue[i][GAMEPAD_AXIS_LEFT_X] = (float)reading.LeftThumbstickX;
                m_currentAxisValue[i][GAMEPAD_AXIS_LEFT_Y] = (float)-reading.LeftThumbstickY;
                m_currentAxisValue[i][GAMEPAD_AXIS_RIGHT_X] = (float)reading.RightThumbstickX;
                m_currentAxisValue[i][GAMEPAD_AXIS_RIGHT_Y] = (float)-reading.RightThumbstickY;
                m_currentAxisValue[i][GAMEPAD_AXIS_LEFT_TRIGGER] = (float)reading.LeftTrigger;
                m_currentAxisValue[i][GAMEPAD_AXIS_RIGHT_TRIGGER] = (float)reading.RightTrigger;

                // Register buttons for pressure triggers
                m_currentButtonState[i][GAMEPAD_BUTTON_LEFT_TRIGGER_2] = m_currentAxisValue[i][GAMEPAD_AXIS_LEFT_TRIGGER] > 0.1;
                m_currentButtonState[i][GAMEPAD_BUTTON_RIGHT_TRIGGER_2] = m_currentAxisValue[i][GAMEPAD_AXIS_RIGHT_TRIGGER] > 0.1;
            }
        }
#endif
    }
}
