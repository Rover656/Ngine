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

        static float _CurrentAxisValue[][GAMEPAD_AXIS_RIGHT_TRIGGER+1];
        static bool _CurrentButtonState[][GAMEPAD_BUTTON_RIGHT_THUMB+1];
        static bool _PreviousButtonState[][GAMEPAD_BUTTON_RIGHT_THUMB+1];
        static bool _Ready[];

#if defined(PLATFORM_UWP)
        static Windows::Gaming::Input::Gamepad ^ _UWPGamepads[];
#endif

        // Private Methods

        static EGamepadAxis GetAxis(int axis_);
        static EGamepadButton GetButton(int button_);

#if defined(PLATFORM_UWP)
        static void UWPGamepadAdded(Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^args);
        static void UWPGamepadRemoved(Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^args);
#endif
    public:
        // Public Methods

        /*
         * Get the value for an axis
         */
        static float GetAxisValue(EGamepadNumber pad_, EGamepadAxis axis_);

        static void Init();

        /*
         * Test if a the gamepad is available
         */
        static bool IsAvailable(EGamepadNumber pad_);

        /*
         * Test if a button is down
         */
        static bool IsButtonDown(EGamepadNumber pad_, EGamepadButton button_);

        /*
         * Test if a button was pressed this frame
         */
        static bool IsButtonPressed(EGamepadNumber pad_, EGamepadButton button_);

        /*
         * Test if a button was released this frame
         */
        static bool IsButtonReleased(EGamepadNumber pad_, EGamepadButton button_);

        /*
         * Poll inputs
         */
        static void PollInputs();
    };
}

#endif //GAMEPAD_H
