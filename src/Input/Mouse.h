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

#ifndef MOUSE_H
#define MOUSE_H

#include "../ngine.h"

namespace Ngine::Input {
    enum MouseButton {
        LEFT = 0,
        RIGHT,
        MIDDLE
    };

    struct MouseState {
        // Public Fields

        /*
         * Buttons that are down
         */
        bool ButtonsDown[3] = {false, false, false};

        /*
         * Buttons pressed this frame
         */
        bool ButtonsPressed[3] = {false, false, false};

        /*
         * Buttons released this frame
         */
        bool ButtonsReleased[3] = { false, false, false };

        /*
         * The mouse wheel y movement
         */
        int MouseWheelMovementY = 0;

        /*
         * Mouse cursor position
         */
        Types::TVector2 Position = { 0, 0 };
    };

    /*
     * Mouse input manager.
     * 
     * Currently does not support touch platforms. Will see if raylib is moving touch into mouse first.
     */
    class NEAPI Mouse {
    public:
        // Public Methods

        /*
         * Get mouse position
         */
        static Types::TVector2 GetMousePosition();

        /*
         * Get the mouse state
         */
        static MouseState GetMouseState();

        /*
         * Get mouse wheel movement
         */
        static int GetMouseWheelMovement();

        /*
         * Is button down
         */
        static bool IsButtonDown(MouseButton button_);

        /*
         * Was button pushed this frame
         */
        static bool IsButtonPressed(MouseButton button_);

        /*
         * Was button released this frame
         */
        static bool IsButtonReleased(MouseButton button_);
    };
}

#endif //MOUSE_H
