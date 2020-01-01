/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP

#include "../Config.hpp"

namespace NerdThings::Ngine::Input {
    /**
     * Gamepad axis.
     */
    enum GamepadAxis {
        /**
         * Error checking for unknown axis used.
         */
        GAMEPAD_AXIS_UNKNOWN = 0,

        /**
         * Left stick X.
         */
        GAMEPAD_AXIS_LEFT_X,

        /**
         * Left stick Y.
         */
        GAMEPAD_AXIS_LEFT_Y,

        /**
         * Right stick X.
         */
        GAMEPAD_AXIS_RIGHT_X,

        /**
         * Right stick Y.
         */
        GAMEPAD_AXIS_RIGHT_Y,

        /**
         * Left trigger.
         */
        GAMEPAD_AXIS_LEFT_TRIGGER,

        /**
         * Right trigger.
         */
        GAMEPAD_AXIS_RIGHT_TRIGGER
    };

    /**
     * Gamepad buttons.
     */
    enum GamepadButton {
        /**
         * Error checking for unknown buttons.
         */
        GAMEPAD_BUTTON_UNKNOWN = 0,

        /**
         * Left collection of buttons up.
         */
        GAMEPAD_BUTTON_LEFT_FACE_UP,

        /**
         * Left collection of buttons right.
         */
        GAMEPAD_BUTTON_LEFT_FACE_RIGHT,

        /**
         * Left collection of buttons down.
         */
        GAMEPAD_BUTTON_LEFT_FACE_DOWN,

        /**
         * Left collection of buttons left.
         */
        GAMEPAD_BUTTON_LEFT_FACE_LEFT,

        /**
         * Right collection of buttons up.
         */
        GAMEPAD_BUTTON_RIGHT_FACE_UP,

        /**
         * Right collection of buttons right.
         */
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,

        /**
         * Right collection of buttons down.
         */
        GAMEPAD_BUTTON_RIGHT_FACE_DOWN,

        /**
         * Right collection of buttons left.
         */
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT,

        /**
         * Left trigger 1.
         */
        GAMEPAD_BUTTON_LEFT_TRIGGER_1,

        /**
         * Left trigger 2.
         */
        GAMEPAD_BUTTON_LEFT_TRIGGER_2,

        /**
         * Right trigger 1.
         */
        GAMEPAD_BUTTON_RIGHT_TRIGGER_1,

        /**
         * Right trigger 2.
         */
        GAMEPAD_BUTTON_RIGHT_TRIGGER_2,

        /**
         * Middle left button.
         */
        GAMEPAD_BUTTON_MIDDLE_LEFT,

        /**
         * Middle button.
         */
        GAMEPAD_BUTTON_MIDDLE,

        /**
         * Middle right button.
         */
        GAMEPAD_BUTTON_MIDDLE_RIGHT,

        /**
         * Left thumb clicked in.
         */
        GAMEPAD_BUTTON_LEFT_THUMB,

        /**
         * Right thumb clicked in.
         */
        GAMEPAD_BUTTON_RIGHT_THUMB
    };

    /**
     * Gamepad numbers.
     */
    enum GamepadNumber {
        /**
         * Gamepad 1.
         */
        GAMEPAD_1 = 0,

        /**
         * Gamepad 2.
         */
        GAMEPAD_2 = 1,

        /**
         * Gamepad 3.
         */
        GAMEPAD_3 = 2,

        /**
         * Gamepad 4.
         */
        GAMEPAD_4 = 3
    };

    /**
     * Gamepad input.
     */
    class NEAPI Gamepad {
        /**
         * The current axis values.
         */
        static float m_currentAxisValue[][GAMEPAD_AXIS_RIGHT_TRIGGER + 1];

        /**
         * Current button states.
         */
        static bool m_currentButtonState[][GAMEPAD_BUTTON_RIGHT_THUMB + 1];

        /**
         * Previous button states.
         */
        static bool m_previousButtonState[][GAMEPAD_BUTTON_RIGHT_THUMB + 1];

        /**
         * Gamepad readiness.
         */
        static bool m_ready[];

#if defined(PLATFORM_UWP)
        /**
         * UWP Gamepad list.
         */
        static Windows::Gaming::Input::Gamepad ^ m_UWPGamepads[];
#endif

        /**
         * Get an axis from a native name.
         *
         * @param axis_ Native axis name.
         * @return Axis enum.
         */
        static GamepadAxis _getAxis(int axis_);

        /**
         * Get a button from a native name.
         *
         * @param button_ Native button name.
         * @return Button enum.
         */
        static GamepadButton _getButton(int button_);

#if defined(PLATFORM_UWP)
        static void _UWPGamepadAdded(Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^args);
        static void _UWPGamepadRemoved(Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^args);
#endif
    public:
        /**
         * Init Gamepad APIs.
         */
        static void Init();

        /**
         * Get the value for an axis.
         *
         * @param pad_ Gamepad to use.
         * @param axis_ Axis to get.
         * @return The axis value.
         */
        static float GetAxisValue(GamepadNumber pad_, GamepadAxis axis_);

        /**
         * Test if a the gamepad is available.
         *
         * @param pad_ Gamepad to check.
         * @return Whether the gamepad is available or not.
         */
        static bool IsAvailable(GamepadNumber pad_);

        /**
         * Test if a button is down.
         *
         * @param pad_ Gamepad to use.
         * @param button_ Button to check
         * @return Whether the button is down or not.
         */
        static bool IsButtonDown(GamepadNumber pad_, GamepadButton button_);

        /**
         * Test if a button was pressed this frame.
         *
         * @param pad_ Gamepad to use.
         * @param button_ Button to check.
         * @return Whether the button was pressed this frame.
         */
        static bool IsButtonPressed(GamepadNumber pad_, GamepadButton button_);

        /**
         * Test if a button was released this frame.
         *
         * @param pad_ Gamepad to use.
         * @param button_ Button to check.
         * @return Whether the button was released this frame.
         */
        static bool IsButtonReleased(GamepadNumber pad_, GamepadButton button_);

        /**
         * Poll inputs.
         */
        static void PollInputs();
    };
}

#endif //GAMEPAD_HPP
