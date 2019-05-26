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

#ifndef EVENTARGS_H
#define EVENTARGS_H

#include "Enums.h"

namespace NerdThings::Ngine {
    // Forward declarations

    namespace Core {
        class BaseEntity;
        class Scene;
        class Game;
    }

    // Base Event Args

    /*
     * Default event arguments
     */
    struct EventArgs {
        // Public Fields

        /*
         * Unbind event handle once run
         */
        bool UnBind = false;
    };

    // Other event args

    /*
     * Mouse button down event args
     */
    struct MouseButtonEventArgs : EventArgs {
        // Public Fields

        /*
         * The mouse button
         */
        EMouseButton Button;

        /*
         * Whether or not the button is pressed
         */
        bool Pressed;

        // Public Constructor(s)

        MouseButtonEventArgs(const EMouseButton button_, const bool pressed_)
            : Button(button_), Pressed(pressed_) {}
    };

    /*
     * Mouse moved event args
     */
    struct MouseMovedEventArgs : EventArgs {
        // Public Fields

        /*
         * The current mouse position
         */
        TVector2 MousePosition;

        /*
         * The change in mouse position
         */
        TVector2 DeltaMousePosition;

        // Public Constructor(s)

        MouseMovedEventArgs(TVector2 mousePosition_, TVector2 deltaMousePosition_)
            : MousePosition(mousePosition_), DeltaMousePosition(deltaMousePosition_) {}
    };

    struct MouseScrollChangedEventArgs : EventArgs {
        // Public Fields

        /*
         * Scroll wheel value
         */
        int Value;

        // Public Constructor(s)

        MouseScrollChangedEventArgs(int value_)
            : Value(value_) {}
    };

    struct SceneLoadEventArgs : EventArgs {
        // Public Fields

        /*
         * The game that the scene has been loaded into
         */
        Core::Game *Game;

        // Public Constructor(s)

        SceneLoadEventArgs(Core::Game *game_)
            : Game(game_) {}
    };
}

#endif //EVENTARGS_H
