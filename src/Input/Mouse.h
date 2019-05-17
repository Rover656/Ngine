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
        bool ButtonsReleased[3] = {false, false, false};

        /*
         * The mouse wheel y movement
         */
        int MouseWheelMovementY = 0;

        /*
         * Mouse cursor position
         */
        Types::TVector2 Position = {0, 0};
    };

    /*
     * Mouse button down event args
     */
    struct MouseButtonEventArgs : Core::EventArgs {
        // Public Fields

        /*
         * The mouse button
         */
        MouseButton Button;

        /*
         * Whether or not the button is pressed
         */
        bool Pressed;

        // Public Constructor(s)

        MouseButtonEventArgs(const MouseButton button_, const bool pressed_)
            : Button(button_), Pressed(pressed_) {}
    };

    /*
     * Mouse moved event args
     */
    struct MouseMovedEventArgs : Core::EventArgs {
        // Public Fields

        /*
         * The current mouse position
         */
        Types::TVector2 MousePosition;

        /*
         * The change in mouse position
         */
        Types::TVector2 DeltaMousePosition;

        // Public Constructor(s)

        MouseMovedEventArgs(Types::TVector2 mousePosition_, Types::TVector2 deltaMousePosition_)
            : MousePosition(mousePosition_), DeltaMousePosition(deltaMousePosition_) {}
    };

    struct MouseScrollChangedEventArgs : Core::EventArgs {
        // Public Fields

        /*
         * Scroll wheel value
         */
        int Value;

        // Public Constructor(s)

        MouseScrollChangedEventArgs(int value_)
            : Value(value_) {}
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

        // Public Event Handles

        /*
         * On game run
         */
        static void OnGameRun(Core::EventArgs e_);

        /*
         * On game update
         */
        static void OnGameUpdate(Core::EventArgs e_);
    };
}

#endif //MOUSE_H
