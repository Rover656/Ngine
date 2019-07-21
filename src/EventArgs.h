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

#include "ngine.h"
#include "EventHandler.h"
#include "Vector2.h"

namespace NerdThings::Ngine {
    // Forward declarations

    class BaseEntity;
    class Scene;
    class Game;

    namespace UI {
        class UIControl;
    }

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

    struct EntityTransformChangedEventArgs : EventArgs {
        // Public Fields

        /*
         * Entity origin
         */
        TVector2 EntityOrigin;

        /*
         * The new entity position
         */
        TVector2 EntityPosition;

        /*
         * The new rotation
         */
        float EntityRotation;

        /*
         * The new scale
         */
        float EntityScale;

        // Public Constructor(s)

        EntityTransformChangedEventArgs(TVector2 origin_, TVector2 pos_, float rot_, float scale_)
                : EntityOrigin(origin_), EntityPosition(pos_), EntityRotation(rot_), EntityScale(scale_) {}
    };

    struct SceneLoadEventArgs : EventArgs {
        // Public Fields

        /*
         * The game that the scene has been loaded into
         */
        Game *ParentGame;

        // Public Constructor(s)

        SceneLoadEventArgs(Game *game_)
                : ParentGame(game_) {}
    };

    struct UIControlEventArgs : EventArgs {
        // Public Fields

        /*
         * The control this belongs to
         */
        UI::UIControl *Control;

        // Public Constructor(s)

        UIControlEventArgs(UI::UIControl *control_)
                : Control(control_) {}
    };
}

#endif //EVENTARGS_H
