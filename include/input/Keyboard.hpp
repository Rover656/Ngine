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

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "../Config.hpp"

#include "../Events.hpp"
#include "../Window.hpp"

#if defined(PLATFORM_DESKTOP)
struct GLFWwindow;
#endif

namespace ngine::input {
    /**
     * A keyboard key
     */
    enum class Key {
        // For error checking
        None = 0,

        // Alphanumeric keys
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        Zero = 48,
        One = 49,
        Two = 50,
        Three = 51,
        Four = 52,
        Five = 53,
        Six = 54,
        Seven = 55,
        Eight = 56,
        Nine = 57,

        Semicolon = 59,
        Equals = 61,

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        // Function keys
        Space = 32,
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348,
        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        Grave = 96,

        // Keypad keys
        KeyPad0 = 320,
        KeyPad1 = 321,
        KeyPad2 = 322,
        KeyPad3 = 323,
        KeyPad4 = 324,
        KeyPad5 = 325,
        KeyPad6 = 326,
        KeyPad7 = 327,
        KeyPad8 = 328,
        KeyPad9 = 329,
        KeyPadDecimal = 330,
        KeyPadDivide = 331,
        KeyPadMultiply = 332,
        KeyPadSubtract = 333,
        KeyPadAdd = 334,
        KeyPadEnter = 335,
        KeyPadEquals = 336
    };

#define KEY_MAX 348

    /**
     * Keyboard input provider.
     *
     * @todo Allow resetting the state of keys (to prevent any actions until it is released, overriding down).
     */
    class NEAPI Keyboard {
        // Make window a friend so it can use our constructor
        friend class ngine::Window;

        /**
         * The window we are attached to.
         */
        Window *m_window;

        /**
         * Last frame's key state
         */
        bool m_previousKeyState[KEY_MAX + 1];

        /**
         * This frame's key state
         */
        bool m_currentKeyState[KEY_MAX + 1];

        /**
         * Next frame's key state.
         * This is here because inputs are handled on a separate thread to updates.
         */
        bool m_nextKeyState[KEY_MAX + 1];

        /**
         * The key used to exit the game.
         */
        Key m_exitKey = Key::None;

        /**
         * The latest key pressed.
         */
        Key m_latestKeyPress = Key::None;

#if defined(PLATFORM_DESKTOP)

        static void _GLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

#elif defined(PLATFORM_UWP)
        static Keyboard * m_UWPKeyboard;
        static Key _keyFromVirtualKey(int key);
        static void _UWPKeyDown(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args);
        static void _UWPKeyUp(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args);
#endif

        /**
         * Create a keyboard input manager.
         *
         * @param window The window.
         */
        Keyboard(Window *window);

    public:
        /**
         * Event fired whenever a key is pressed.
         */
        class KeyPressEvent : public Event {
        public:
            /**
             * The window that fired this event
             */
            ngine::Window *Window;

            /**
             * The key this event is fired for.
             */
            input::Key Key;

            /**
             * Create a key press event
             * @param sender The keyboard that fired this
             * @param window The window the keyboard is attached to
             * @param key The key that has been pressed
             */
            KeyPressEvent(Keyboard *sender, ngine::Window *window, input::Key key) : Window(window), Key(key),
                                                                                     Event(sender) {}
        };

        /**
         * Event fired whenever a key is released.
         */
        class KeyReleaseEvent : public Event {
        public:
            /**
             * The window that fired this event
             */
            ngine::Window *Window;

            /**
             * The key this event is fired for.
             */
            input::Key Key;

            /**
             * Create a key release event
             * @param sender The keyboard that fired this
             * @param window The window the keyboard is attached to
             * @param key The key that has been released
             */
            KeyReleaseEvent(Keyboard *sender, ngine::Window *window, input::Key key) : Window(window), Key(key),
                                                                                     Event(sender) {}
        };

        /**
         * Get the current window's keyboard input manager.
         *
         * @deprecated This will be removed before Ngine 1.0
         * @return The current window's keyboard input manager.
         */
        static Keyboard *getCurrent();

        /**
         * Get key enum from char.
         *
         * @param key Key to get.
         * @return Key enum.
         */
        static Key getKey(char key);

        /**
         * Get the latest keypress.
         *
         * @deprecated This is pretty much useless, use events if you want this information.
         * @return The latest key that has been pressed.
         */
        Key getLatestKeypress() const;

        /**
         * Is the key down.
         *
         * @param key Key to check
         * @return Whether the key is down or not.
         */
        bool isKeyDown(Key key) const;

        /**
         * Check if a key is down (by its character).
         *
         * @param key Key to check.
         * @return Whether or not the key was pressed.
         */
        bool isKeyDown(char key) const;

        /**
         * Has a key been pressed this tick.
         *
         * @param key Key to check
         * @return Whether the key is was pressed this frame.
         */
        bool isKeyPressed(Key key) const;

        /**
         * Has a key been pressed this tick.
         *
         * @param key Key to check
         * @return Whether the key is was pressed this frame.
         */
        bool isKeyPressed(char key) const;

        /**
         * Was the key released this frame.
         *
         * @param key Key to check
         * @return Whether the key was released this frame.
         */
        bool isKeyReleased(Key key) const;

        /**
         * Was the key released this frame.
         *
         * @param key Key to check
         * @return Whether the key was released this frame.
         */
        bool isKeyReleased(char key) const;

        /**
         * Poll keyboard inputs.
         */
        void pollInputs();

        /**
         * Set the key to close the game.
         *
         * @note This is most useful for debugging.
         * @todo Move this to Game??
         * @param key Set the key that can close the game.
         */
        void setExitKey(Key key);
    };
}

#endif //KEYBOARD_HPP
