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

#include "../Ngine.h"

namespace NerdThings::Ngine::Input {
    /*
     * Gamepad axis
     */
    enum GamepadAxis {
        /*
         * Error checking for unknown axis used
         */
        GAMEPAD_AXIS_UNKNOWN = 0,

        /*
         * Left stick X
         */
        GAMEPAD_AXIS_LEFT_X,

        /*
         * Left stick Y
         */
        GAMEPAD_AXIS_LEFT_Y,

        /*
         * Right stick X
         */
        GAMEPAD_AXIS_RIGHT_X,

        /*
         * Right stick Y
         */
        GAMEPAD_AXIS_RIGHT_Y,

        /*
         * Left trigger
         */
        GAMEPAD_AXIS_LEFT_TRIGGER,

        /*
         * Right trigger
         */
        GAMEPAD_AXIS_RIGHT_TRIGGER
    };

    /*
     * Gamepad buttons
     */
    enum EGamepadButton {
        /*
         * Error checking for unknown buttons
         */
        GAMEPAD_BUTTON_UNKNOWN = 0,

        /*
         * Left collection of buttons up
         */
        GAMEPAD_BUTTON_LEFT_FACE_UP,

        /*
         * Left collection of buttons right
         */
        GAMEPAD_BUTTON_LEFT_FACE_RIGHT,

        /*
         * Left collection of buttons down
         */
        GAMEPAD_BUTTON_LEFT_FACE_DOWN,

        /*
         * Left collection of buttons left
         */
        GAMEPAD_BUTTON_LEFT_FACE_LEFT,

        /*
         * Right collection of buttons up
         */
        GAMEPAD_BUTTON_RIGHT_FACE_UP,

        /*
         * Right collection of buttons right
         */
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,

        /*
         * Right collection of buttons down
         */
        GAMEPAD_BUTTON_RIGHT_FACE_DOWN,

        /*
         * Right collection of buttons left
         */
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT,

        /*
         * Left trigger 1
         */
        GAMEPAD_BUTTON_LEFT_TRIGGER_1,

        /*
         * Left trigger 2
         */
        GAMEPAD_BUTTON_LEFT_TRIGGER_2,

        /*
         * Right trigger 1
         */
        GAMEPAD_BUTTON_RIGHT_TRIGGER_1,

        /*
         * Right trigger 2
         */
        GAMEPAD_BUTTON_RIGHT_TRIGGER_2,

        /*
         * Middle left button
         */
        GAMEPAD_BUTTON_MIDDLE_LEFT,

        /*
         * Middle button
         */
        GAMEPAD_BUTTON_MIDDLE,

        /*
         * Middle right button
         */
        GAMEPAD_BUTTON_MIDDLE_RIGHT,

        /*
         * Left thumb clicked in
         */
        GAMEPAD_BUTTON_LEFT_THUMB,

        /*
         * Right thumb clicked in
         */
        GAMEPAD_BUTTON_RIGHT_THUMB
    };

    /*
     * Gamepad numbers
     */
    enum EGamepadNumber {
        /*
         * Gamepad 1
         */
        GAMEPAD_1 = 0,

        /*
         * Gamepad 2
         */
        GAMEPAD_2 = 1,

        /*
         * Gamepad 3
         */
        GAMEPAD_3 = 2,

        /*
         * Gamepad 4
         */
        GAMEPAD_4 = 3
    };

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

        static GamepadAxis GetAxis(int axis_);
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
        static float GetAxisValue(EGamepadNumber pad_, GamepadAxis axis_);

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
