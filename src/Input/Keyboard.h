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

namespace NerdThings::Ngine::Input {
    /*
     * Keyboard input
     */
    class NEAPI Keyboard {
    public:
        // Public Methods

        /*
         * Get the latest keypress
         */
        static EKey GetLatestKeypress();

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
         * Set the key to close the game
         */
        static void SetExitKey(EKey key_);
    };
}

#endif //KEYBOARD_H
