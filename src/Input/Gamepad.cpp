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

#include "Gamepad.h"

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#elif defined(PLATFORM_UWP)
#endif

#include "../Window.h"

namespace NerdThings::Ngine::Input {
    // Private Fields

    float Gamepad::_CurrentAxisValue[4][GAMEPAD_AXIS_RIGHT_TRIGGER+1];
    bool Gamepad::_CurrentButtonState[4][GAMEPAD_BUTTON_RIGHT_THUMB+1];
    bool Gamepad::_PreviousButtonState[4][GAMEPAD_BUTTON_RIGHT_THUMB+1];
    bool Gamepad::_Ready[4];

#if defined(PLATFORM_UWP)
    Windows::Gaming::Input::Gamepad ^Gamepad::_UWPGamepads[4] = {nullptr, nullptr, nullptr, nullptr};
#endif

    // Private Methods

    EGamepadAxis Gamepad::GetAxis(int axis_) {
        EGamepadAxis axis = GAMEPAD_AXIS_UNKNOWN;
#if defined(PLATFORM_DESKTOP)
        switch (axis_) {
            case GLFW_GAMEPAD_AXIS_LEFT_X: axis = GAMEPAD_AXIS_LEFT_X; break;
            case GLFW_GAMEPAD_AXIS_LEFT_Y: axis = GAMEPAD_AXIS_LEFT_Y; break;
            case GLFW_GAMEPAD_AXIS_RIGHT_X: axis = GAMEPAD_AXIS_RIGHT_X; break;
            case GLFW_GAMEPAD_AXIS_RIGHT_Y: axis = GAMEPAD_AXIS_RIGHT_Y; break;
            case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER: axis = GAMEPAD_AXIS_LEFT_TRIGGER; break;
            case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER: axis = GAMEPAD_AXIS_RIGHT_TRIGGER; break;
        }
#elif defined(PLATFORM_UWP)
        axis = (EGamepadAxis) axis_;     // UWP will provide the correct axis
#endif
        return axis;
    }

    EGamepadButton Gamepad::GetButton(int button_) {
        EGamepadButton btn = GAMEPAD_BUTTON_UNKNOWN;
#if defined(PLATFORM_DESKTOP)
        switch (button_) {
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
        btn = (EGamepadButton) button_;
#endif
        return btn;
    }

#if defined(PLATFORM_UWP)
    void Gamepad::UWPGamepadAdded(Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^args) {
        for (auto i = 0; i < 4; i++) {
            if (_UWPGamepads[i] == nullptr) {
                _UWPGamepads[i] = args;
                _Ready[i] = true;
                break;
            }
        }
    }


    void Gamepad::UWPGamepadRemoved(Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^args) {
        for (auto i = 0; i < 4; i++) {
            if (_UWPGamepads[i] == args) {
                _UWPGamepads[i] = nullptr;
                _Ready[i] = false;
                break;
            }
        }
    }
#endif

    // Public Methods

    float Gamepad::GetAxisValue(EGamepadNumber pad_, const EGamepadAxis axis_) {
        return _CurrentAxisValue[pad_][axis_];
    }

    void Gamepad::Init() {
#if defined(PLATFORM_UWP)
        Windows::Gaming::Input::Gamepad::GamepadAdded += ref new Windows::Foundation::EventHandler<Windows::Gaming::Input::Gamepad ^>(&UWPGamepadAdded);
        Windows::Gaming::Input::Gamepad::GamepadRemoved += ref new Windows::Foundation::EventHandler<Windows::Gaming::Input::Gamepad ^>(&UWPGamepadRemoved);
#endif
    }

    bool Gamepad::IsAvailable(EGamepadNumber pad_) {
        return _Ready[pad_];
    }

    bool Gamepad::IsButtonDown(EGamepadNumber pad_, const EGamepadButton button_) {
        return _CurrentButtonState[pad_][button_];
    }

    bool Gamepad::IsButtonPressed(EGamepadNumber pad_, const EGamepadButton button_) {
        return _CurrentButtonState[pad_][button_] != _PreviousButtonState[pad_][button_] && _CurrentButtonState[pad_][button_];
    }

    bool Gamepad::IsButtonReleased(EGamepadNumber pad_, const EGamepadButton button_) {
        return _CurrentButtonState[pad_][button_] != _PreviousButtonState[pad_][button_] && !_CurrentButtonState[pad_][button_];
    }

    void Gamepad::PollInputs() {
#if defined(PLATFORM_DESKTOP)
        // Check for ready controllers
        for (auto i = 0; i < 4; i++) {
            _Ready[i] = glfwJoystickPresent(i) == GLFW_TRUE;
        }

        // Check for inputs
        for (auto i = 0; i < 4; i++) {
            if (IsAvailable((EGamepadNumber) i)) {
                for (auto j = 0; j <= GAMEPAD_BUTTON_RIGHT_THUMB; j++) _PreviousButtonState[i][j] = _CurrentButtonState[i][j];

                GLFWgamepadstate state;
                glfwGetGamepadState(i, &state);
                auto buttons = state.buttons;

                for (auto j = 0; (buttons != nullptr) && (j <= GLFW_GAMEPAD_BUTTON_DPAD_LEFT) && (j <= GAMEPAD_BUTTON_RIGHT_THUMB); j++) {
                    EGamepadButton b = GetButton(j);

                    _CurrentButtonState[i][b] = buttons[j] == GLFW_PRESS;
                }

                // Get axis values
                auto axes = state.axes;

                for (auto j = 0; (axes != nullptr) && (j <= GLFW_GAMEPAD_AXIS_LAST) && (j <= GAMEPAD_AXIS_RIGHT_TRIGGER); j++){
                    EGamepadAxis a = GetAxis(j);

                    _CurrentAxisValue[i][a] = axes[j];
                }

                // Register buttons for pressure triggers
                _CurrentButtonState[i][GAMEPAD_BUTTON_LEFT_TRIGGER_2] = _CurrentAxisValue[i][GAMEPAD_AXIS_LEFT_TRIGGER] > 0.1;
                _CurrentButtonState[i][GAMEPAD_BUTTON_RIGHT_TRIGGER_2] = _CurrentAxisValue[i][GAMEPAD_AXIS_RIGHT_TRIGGER] > 0.1;
            }
        }
#elif defined(PLATFORM_UWP)
        // Get gamepad states
        for (auto i = 0; i < 4; i++) {
            if (IsAvailable((EGamepadNumber)i)) {
                auto gamepad = _UWPGamepads[i];
                auto reading = gamepad->GetCurrentReading();

                for (auto j = 0; j <= GAMEPAD_BUTTON_RIGHT_THUMB; j++) _PreviousButtonState[i][j] = _CurrentButtonState[i][j];

                // Get buttons
                _CurrentButtonState[i][GAMEPAD_BUTTON_RIGHT_FACE_DOWN] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::A) == Windows::Gaming::Input::GamepadButtons::A);
                _CurrentButtonState[i][GAMEPAD_BUTTON_RIGHT_FACE_RIGHT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::B) == Windows::Gaming::Input::GamepadButtons::B);
                _CurrentButtonState[i][GAMEPAD_BUTTON_RIGHT_FACE_LEFT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::X) == Windows::Gaming::Input::GamepadButtons::X);
                _CurrentButtonState[i][GAMEPAD_BUTTON_RIGHT_FACE_UP] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::Y) == Windows::Gaming::Input::GamepadButtons::Y);

                _CurrentButtonState[i][GAMEPAD_BUTTON_LEFT_TRIGGER_1] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::LeftShoulder) == Windows::Gaming::Input::GamepadButtons::LeftShoulder);
                _CurrentButtonState[i][GAMEPAD_BUTTON_RIGHT_TRIGGER_1] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::RightShoulder) == Windows::Gaming::Input::GamepadButtons::RightShoulder);

                _CurrentButtonState[i][GAMEPAD_BUTTON_MIDDLE_LEFT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::View) == Windows::Gaming::Input::GamepadButtons::View);
                _CurrentButtonState[i][GAMEPAD_BUTTON_MIDDLE_RIGHT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::Menu) == Windows::Gaming::Input::GamepadButtons::Menu);

                _CurrentButtonState[i][GAMEPAD_BUTTON_LEFT_FACE_UP] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadUp) == Windows::Gaming::Input::GamepadButtons::DPadUp);
                _CurrentButtonState[i][GAMEPAD_BUTTON_LEFT_FACE_RIGHT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadRight) == Windows::Gaming::Input::GamepadButtons::DPadRight);
                _CurrentButtonState[i][GAMEPAD_BUTTON_LEFT_FACE_DOWN] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadDown) == Windows::Gaming::Input::GamepadButtons::DPadDown);
                _CurrentButtonState[i][GAMEPAD_BUTTON_LEFT_FACE_LEFT] = ((reading.Buttons & Windows::Gaming::Input::GamepadButtons::DPadLeft) == Windows::Gaming::Input::GamepadButtons::DPadLeft);

                _CurrentButtonState[i][GAMEPAD_BUTTON_MIDDLE] = false; // Not supported

                // Get axis
                _CurrentAxisValue[i][GAMEPAD_AXIS_LEFT_X] = (float)reading.LeftThumbstickX;
                _CurrentAxisValue[i][GAMEPAD_AXIS_LEFT_Y] = (float)reading.LeftThumbstickY;
                _CurrentAxisValue[i][GAMEPAD_AXIS_RIGHT_X] = (float)reading.RightThumbstickX;
                _CurrentAxisValue[i][GAMEPAD_AXIS_RIGHT_Y] = (float)reading.RightThumbstickY;
                _CurrentAxisValue[i][GAMEPAD_AXIS_LEFT_TRIGGER] = (float)reading.LeftTrigger;
                _CurrentAxisValue[i][GAMEPAD_AXIS_RIGHT_TRIGGER] = (float)reading.RightTrigger;

                // Register buttons for pressure triggers
                _CurrentButtonState[i][GAMEPAD_BUTTON_LEFT_TRIGGER_2] = _CurrentAxisValue[i][GAMEPAD_AXIS_LEFT_TRIGGER] > 0.1;
                _CurrentButtonState[i][GAMEPAD_BUTTON_RIGHT_TRIGGER_2] = _CurrentAxisValue[i][GAMEPAD_AXIS_RIGHT_TRIGGER] > 0.1;
            }
        }
#endif
    }
}
