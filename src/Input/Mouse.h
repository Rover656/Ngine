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

#include "../Core/EventHandler.h"

namespace NerdThings::Ngine::Input {
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
        bool ButtonsReleased[3] = {false, false, false};

        /*
         * The mouse wheel y movement
         */
        int MouseWheelMovementY = 0;

        /*
         * Mouse cursor position
         */
        TVector2 Position = {0, 0};
    };

    /*
     * Mouse input manager.
     * 
     * Currently does not support touch platforms. Will see if raylib is moving touch into mouse first.
     */
    class NEAPI Mouse {
    public:
        // Public Fields

        /*
         * The last frame mouse position
         */
        static MouseState LastMouseState;

        /*
         * On mouse button pressed
         */
        static Core::EventHandler<MouseButtonEventArgs> OnMouseButtonPressed;

        /*
         * On mouse button released
         */
        static Core::EventHandler<MouseButtonEventArgs> OnMouseButtonReleased;

        /*
         * On mouse moved event
         */
        static Core::EventHandler<MouseMovedEventArgs> OnMouseMoved;

        /*
         * On mouse scroll Y changed
         */
        static Core::EventHandler<MouseScrollChangedEventArgs> OnMouseScrollYChanged;

        // Public Methods

        /*
         * Get mouse position
         */
        static TVector2 GetMousePosition();

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
        static bool IsButtonDown(EMouseButton button_);

        /*
         * Was button pushed this frame
         */
        static bool IsButtonPressed(EMouseButton button_);

        /*
         * Was button released this frame
         */
        static bool IsButtonReleased(EMouseButton button_);

        /*
         * Set mouse offset.
         * This should only be used internally.
         * This may produce unexpected results if used.
         */
        static void SetOffset(float ox_, float oy_);

        /*
         * Set mouse scale.
         * This should only be used internally.
         * This may produce unexpected results if used.
         */
        static void SetScale(float sx_, float sy_);

        // Public Event Handles

        /*
         * On game run
         */
        static void OnGameRun(EventArgs &e_);

        /*
         * On game update
         */
        static void OnGameUpdate(EventArgs &e_);
    };
}

#endif //MOUSE_H
