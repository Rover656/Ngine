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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../ngine.h"

#if defined(PLATFORM_DESKTOP)
struct GLFWwindow;
#endif

namespace NerdThings::Ngine::Input {
    /*
     * Keyboard input
     */
    class NEAPI Keyboard {
        // Private Fields

        static bool _CurrentKeyState[];
        static EKey _ExitKey;
        static bool _PreviousKeyState[];
        static EKey _LatestKeyPress;
        static bool _NextKeyState[];

        // Private Methods

#if defined(PLATFORM_DESKTOP)

        static void GLFWKeyCallback(GLFWwindow *window_, int key_, int scancode_, int action_, int mods_);

#elif defined(PLATFORM_UWP)

        static EKey KeyFromVirtualKey(int key_);

        static void UWPKeyDown(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args);
        static void UWPKeyUp(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args);

#endif

    public:
        // Public Methods

        /*
         * Get the latest keypress
         */
        static EKey GetLatestKeypress();

        /*
         * Init keyboard input APIs
         */
        static void Init();

        /*
         * Is the key down
         */
        static bool IsKeyDown(EKey key_);

        /*
         * Has a key been pressed this tick
         */
        static bool IsKeyPressed(EKey key_);

        /*
         * Was the key released this frame
         */
        static bool IsKeyReleased(EKey key_);

        /*
         * Is the key not pressed
         */
        static bool IsKeyUp(EKey key_);

        /*
         * Poll keyboard inputs
         */
        static void PollInputs();

        /*
         * Set the key to close the game
         */
        static void SetExitKey(EKey key_);
    };
}

#endif //KEYBOARD_H
