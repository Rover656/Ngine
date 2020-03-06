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

#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP

#include "../Config.hpp"

// Counts for arrays (include unknowns)
#define AXIS_COUNT 7
#define BUTTON_COUNT 18

namespace ngine::input {
    /**
     * Gamepad axis.
     */
    enum class GamepadAxis : int {
        /**
         * Error checking for unknown axis used.
         */
        Unknown = 0,

        /**
         * Left stick X.
         */
        LeftX,

        /**
         * Left stick Y.
         */
        LeftY,

        /**
         * Right stick X.
         */
        RightX,

        /**
         * Right stick Y.
         */
        RightY,

        /**
         * Left trigger.
         */
        LeftTrigger,

        /**
         * Right trigger.
         */
        RightTrigger
    };

    /**
     * Gamepad buttons.
     */
    enum class GamepadButton : int {
        /**
         * Error checking for unknown buttons.
         */
        Unknown = 0,

        /**
         * Left collection of buttons up.
         */
        LeftFaceUp,

        /**
         * Left collection of buttons right.
         */
        LeftFaceRight,

        /**
         * Left collection of buttons down.
         */
        LeftFaceDown,

        /**
         * Left collection of buttons left.
         */
        LeftFaceLeft,

        /**
         * Right collection of buttons up.
         */
        RightFaceUp,

        /**
         * Right collection of buttons right.
         */
        RightFaceRight,

        /**
         * Right collection of buttons down.
         */
        RightFaceDown,

        /**
         * Right collection of buttons left.
         */
        RightFaceLeft,

        /**
         * Left trigger 1.
         */
        LeftTrigger1,

        /**
         * Left trigger 2.
         */
        LeftTrigger2,

        /**
         * Right trigger 1.
         */
        RightTrigger1,

        /**
         * Right trigger 2.
         */
        RightTrigger2,

        /**
         * Middle left button.
         */
        MiddleLeft,

        /**
         * Middle button.
         */
        Middle,

        /**
         * Middle right button.
         */
        MiddleRight,

        /**
         * Left thumb clicked in.
         */
        LeftThumb,

        /**
         * Right thumb clicked in.
         */
        RightThumb
    };

    /**
     * Gamepad numbers.
     */
    enum class GamepadNumber : int {
        /**
         * Gamepad 1.
         */
        One = 0,

        /**
         * Gamepad 2.
         */
        Two = 1,

        /**
         * Gamepad 3.
         */
        Three = 2,

        /**
         * Gamepad 4.
         */
        Four = 3
    };

    /**
     * Gamepad input.
     */
    class NEAPI Gamepad {
        /**
         * The current axis values.
         */
        static float m_currentAxisValue[][AXIS_COUNT];

        /**
         * Current button states.
         */
        static bool m_currentButtonState[][BUTTON_COUNT];

        /**
         * Previous button states.
         */
        static bool m_previousButtonState[][BUTTON_COUNT];

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
         * @param axis Native axis name.
         * @return Axis enum.
         */
        static GamepadAxis _getAxis(int axis);

        /**
         * Get a button from a native name.
         *
         * @param button Native button name.
         * @return Button enum.
         */
        static GamepadButton _getButton(int button);

#if defined(PLATFORM_UWP)
        static void _UWPGamepadAdded(Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^args);
        static void _UWPGamepadRemoved(Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^args);
#endif
    public:
        /**
         * Init Gamepad APIs.
         */
        static void initialize();

        /**
         * Get the value for an axis.
         *
         * @param pad Gamepad to use.
         * @param axis Axis to get.
         * @return The axis value.
         */
        static float getAxisValue(GamepadNumber pad, GamepadAxis axis);

        /**
         * Test if a the gamepad is available.
         *
         * @param pad Gamepad to check.
         * @return Whether the gamepad is available or not.
         */
        static bool isAvailable(GamepadNumber pad);

        /**
         * Test if a button is down.
         *
         * @param pad Gamepad to use.
         * @param button Button to check
         * @return Whether the button is down or not.
         */
        static bool isButtonDown(GamepadNumber pad, GamepadButton button);

        /**
         * Test if a button was pressed this frame.
         *
         * @param pad Gamepad to use.
         * @param button Button to check.
         * @return Whether the button was pressed this frame.
         */
        static bool isButtonPressed(GamepadNumber pad, GamepadButton button);

        /**
         * Test if a button was released this frame.
         *
         * @param pad Gamepad to use.
         * @param button Button to check.
         * @return Whether the button was released this frame.
         */
        static bool isButtonReleased(GamepadNumber pad, GamepadButton button);

        /**
         * Poll inputs.
         */
        static void pollInputs();
    };
}

#endif //GAMEPAD_HPP
