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

#if defined(PLATFORM_DESKTOP)
struct GLFWwindow;
#elif defined(PLATFORM_UWP)
#include "../Platform/UWP/GameApp.h"
#endif

#include "../Vector2.h"
#include "../EventArgs.h"
#include "../EventHandler.h"

namespace NerdThings::Ngine::Input {
    struct MouseState {
        // Public Fields

        /*
         * Buttons that are down
         */
        bool ButtonsDown[3] = {false, false, false};

        /*
         * The change in mouse wheel X offset between frames
         */
        int MouseWheelXDelta = 0;

        /*
         * The change in mouse wheel Y offset between frames
         */
        int MouseWheelYDelta = 0;

        /*
         * Whether or not this mouse state struct has been populated with info.
         */
        bool Populated = false;

        /*
         * Mouse cursor position
         */
        TVector2 Position = {0, 0};
    };

    /*
     * Mouse input manager.
     */
    class NEAPI Mouse {
        // Private Fields

        /*
         * The current mouse state
         */
        static MouseState _CurrentMouseState;

        /*
         * The last frame mouse position
         */
        static MouseState _LastMouseState;

        /*
         * Mouse position offset
         */
        static TVector2 _MouseOffset;

        /*
         * Mouse position scale
         */
        static TVector2 _MouseScale;

        /*
         * The next mouse state that is being built while the frame is progressing
         */
        static MouseState _NextMouseState;

        // Private Methods

        /*
         * Get the mouse position direct from the device
         */
        static TVector2 InternalGetMousePosition();

#if defined(PLATFORM_DESKTOP)
        static void GLFWMouseButtonCallback(GLFWwindow *window_, int button_, int action_, int mods_);
        static void GLFWScrollCallback(GLFWwindow *window_, double x_, double y_);
#elif defined(PLATFORM_UWP)
        static void UWPMouseMoved(Windows::Devices::Input::MouseDevice ^sender, Windows::Devices::Input::MouseEventArgs ^args);
        static void UWPPointerWheelChanged(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args);
        static void UWPPointerButtonEvent(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args);
#endif
    public:
        // Public Fields

        /*
         * On mouse button pressed
         */
        static EventHandler<MouseButtonEventArgs> OnMouseButtonPressed;

        /*
         * On mouse button released
         */
        static EventHandler<MouseButtonEventArgs> OnMouseButtonReleased;

        /*
         * On mouse moved event
         */
        static EventHandler<MouseMovedEventArgs> OnMouseMoved;

        /*
         * On mouse scroll X changed
         */
        static EventHandler<MouseScrollChangedEventArgs> OnMouseScrollXChanged;

        /*
         * On mouse scroll Y changed
         */
        static EventHandler<MouseScrollChangedEventArgs> OnMouseScrollYChanged;

        // Public Methods

        /*
         * Cancel button press (Prevents double event checks).
         */
        static void CancelButton(EMouseButton button_);

        /*
         * Get mouse position
         */
        static TVector2 GetMousePosition();

        /*
         * Get the mouse state.
         * Use this if you use cancellable events.
         */
        static MouseState GetMouseState();

        /*
         * Get mouse wheel X movement.
         */
        static int GetMouseWheelXDelta();

        /*
         * Get mouse wheel Y movement.
         */
        static int GetMouseWheelYDelta();

        /*
         * Init mouse input APIs.
         */
        static void Init();

        /*
         * Is button down (even if cancelled).
         */
        static bool IsButtonDown(EMouseButton button_);

        /*
         * Was button pushed this frame (even if cancelled).
         */
        static bool IsButtonPressed(EMouseButton button_);

        /*
         * Was button released this frame (even if cancelled).
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
         * On game update
         */
        static void OnGameUpdate(EventArgs &e_);
};
}

#endif //MOUSE_H
