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

#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "../ngine.h"

namespace NerdThings::Ngine::Input {
    /*
     * Gamepad input
     */
    class NEAPI Gamepad {
        // Private Fields

        /*
         * The current gamepad number
         */
        EGamepadNumber _GamepadNumber;

    public:
        // Public Constructor(s)

        /*
         * Create a gamepad accessor for gamepad number
         */
        Gamepad(EGamepadNumber number_);

        // Public Methods

        /*
         * Get the value for an axis
         */
        [[nodiscard]] float GetAxisValue(EGamepadAxis axis_) const;

        /*
         * Get a gamepad by number
         */
        static Gamepad GetGamepad(EGamepadNumber gamepadNumber_);

        /*
         * Test if a the gamepad is available
         */
        [[nodiscard]] bool IsAvailable() const;

        /*
         * Test if a button is down
         */
        [[nodiscard]] bool IsButtonDown(EGamepadButton button_) const;

        /*
         * Test if a button was pressed this frame
         */
        [[nodiscard]] bool IsButtonPressed(EGamepadButton button_) const;

        /*
         * Test if a button was released this frame
         */
        [[nodiscard]] bool IsButtonReleased(EGamepadButton button_) const;

        /*
         * Test if a button is up
         */
        [[nodiscard]] bool IsButtonUp(EGamepadButton button_) const;
    };
}

#endif //GAMEPAD_H
