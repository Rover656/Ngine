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

namespace NerdThings::Ngine::Input {
    // Public Constructor(s)

    Gamepad::Gamepad(EGamepadNumber number_) {
        if (number_ < 0 || number_ > 3)
            throw std::runtime_error("Gamepad number out of range.");
        _GamepadNumber = number_;
    }

    // Public Methods

    float Gamepad::GetAxisValue(const EGamepadAxis axis_) const {
        return GetGamepadAxisMovement(static_cast<int>(_GamepadNumber), static_cast<int>(axis_));
    }

    Gamepad Gamepad::GetGamepad(const EGamepadNumber gamepadNumber_) {
        return Gamepad(gamepadNumber_);
    }

    bool Gamepad::IsAvailable() const {
        return IsGamepadAvailable(static_cast<int>(_GamepadNumber));
    }

    bool Gamepad::IsButtonDown(const EGamepadButton button_) const {
        return IsGamepadButtonDown(static_cast<int>(_GamepadNumber), static_cast<int>(button_));
    }

    bool Gamepad::IsButtonPressed(const EGamepadButton button_) const {
        return IsGamepadButtonPressed(static_cast<int>(_GamepadNumber), static_cast<int>(button_));
    }

    bool Gamepad::IsButtonReleased(const EGamepadButton button_) const {
        return IsGamepadButtonReleased(static_cast<int>(_GamepadNumber), static_cast<int>(button_));
    }

    bool Gamepad::IsButtonUp(const EGamepadButton button_) const {
        return IsGamepadButtonUp(static_cast<int>(_GamepadNumber), static_cast<int>(button_));
    }

}
