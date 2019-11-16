/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef MOUSE_H
#define MOUSE_H

#include "../Ngine.h"

#if defined(PLATFORM_DESKTOP)
struct GLFWwindow;
#elif defined(PLATFORM_UWP)
#include "../UWP/GameApp.h"
#endif

#include "../Vector2.h"
#include "../EventHandler.h"

namespace NerdThings::Ngine::Input {
    /**
     * Mouse button
     */
    enum MouseButton {
        MOUSE_BUTTON_LEFT = 0,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_MIDDLE
    };

    /**
     * Mouse button down event args
     */
    struct MouseButtonEventArgs : EventArgs {
        /**
         * The mouse button.
         */
        MouseButton Button;

        /**
         * Whether or not the button is pressed.
         */
        bool Pressed;

        // Public Constructor(s)

        /**
         * Create a mouse changed event argument.
         *
         * @param button_ The button that has changed.
         * @param pressed_ Whether it was pressed or not.
         */
        MouseButtonEventArgs(const MouseButton button_, const bool pressed_)
                : Button(button_), Pressed(pressed_) {}
    };

    /**
     * Mouse moved event args.
     */
    struct MouseMovedEventArgs : EventArgs {
        /**
         * The current mouse position
         */
        Vector2 MousePosition;

        /**
         * The change in mouse position
         */
        Vector2 DeltaMousePosition;

        /**
         * Create a mouse moved event arg.
         *
         * @param mousePosition_ The current position.
         * @param deltaMousePosition_ The delta position relative to the last position.
         */
        MouseMovedEventArgs(Vector2 mousePosition_, Vector2 deltaMousePosition_)
                : MousePosition(mousePosition_), DeltaMousePosition(deltaMousePosition_) {}
    };

    struct MouseScrollChangedEventArgs : EventArgs {
        /*
         * Scroll wheel value
         */
        int Value;

        /**
         * Create a scroll changed event argument.
         *
         * @param value_ Scroll value.
         */
        MouseScrollChangedEventArgs(int value_)
                : Value(value_) {}
    };

    /**
     * Mouse state info.
     */
    struct MouseState {
        /**
         * Buttons that are down.
         */
        bool ButtonsDown[3] = {false, false, false};

        /**
         * The change in mouse wheel X offset between frames.
         */
        int MouseWheelXDelta = 0;

        /**
         * The change in mouse wheel Y offset between frames.
         */
        int MouseWheelYDelta = 0;

        /**
         * Whether or not this mouse state struct has been populated with info.
         */
        bool Populated = false;

        /**
         * Mouse cursor position.
         */
        Vector2 Position = {0, 0};
    };

    /**
     * Mouse input provider.
     */
    class NEAPI Mouse {
        /**
         * The current mouse state
         */
        static MouseState _CurrentMouseState;

        /**
         * The last frame mouse position
         */
        static MouseState _LastMouseState;

        /**
         * Mouse position offset.
         */
        static Vector2 _MouseOffset;

        /**
         * Mouse position scale.
         */
        static Vector2 _MouseScale;

        /**
         * The next mouse state that is being built while the frame is progressing.
         */
        static MouseState _NextMouseState;

        /**
         * Get the mouse position direct from the device.
         */
        static Vector2 InternalGetMousePosition();

#if defined(PLATFORM_DESKTOP)
        static void GLFWMouseButtonCallback(GLFWwindow *window_, int button_, int action_, int mods_);
        static void GLFWScrollCallback(GLFWwindow *window_, double x_, double y_);
#elif defined(PLATFORM_UWP)
        static void UWPMouseMoved(Windows::Devices::Input::MouseDevice ^sender, Windows::Devices::Input::MouseEventArgs ^args);
        static void UWPPointerWheelChanged(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args);
        static void UWPPointerButtonEvent(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args);
#endif
    public:
        /**
         * On mouse button pressed.
         */
        static Event<MouseButtonEventArgs> OnMouseButtonPressed;

        /**
         * On mouse button released.
         */
        static Event<MouseButtonEventArgs> OnMouseButtonReleased;

        /**
         * On mouse moved event.
         */
        static Event<MouseMovedEventArgs> OnMouseMoved;

        /**
         * On mouse scroll X changed.
         */
        static Event<MouseScrollChangedEventArgs> OnMouseScrollXChanged;

        /**
         * On mouse scroll Y changed.
         */
        static Event<MouseScrollChangedEventArgs> OnMouseScrollYChanged;

        /**
         * Cancel button press (Prevents double event checks).
         *
         * @param button_ The button to cancel.
         */
        static void CancelButton(MouseButton button_);

        /**
         * Get mouse position.
         *
         * @return The mouse position in screen coordinates.
         */
        static Vector2 GetMousePosition();

        /**
         * Get mouse wheel X movement.
         *
         * @return Mouse wheel X movement.
         */
        static int GetMouseWheelXDelta();

        /**
         * Get mouse wheel Y movement.
         *
         * @return Mouse wheel Y movement.
         */
        static int GetMouseWheelYDelta();

        /**
         * Init mouse input APIs.
         */
        static void Init();

        /**
         * Is button down.
         *
         * @param button_ Button to check.
         * @return Whether or not the button is down.
         */
        static bool IsButtonDown(MouseButton button_);

        /**
         * Was button pushed this frame.
         *
         * @param button_Button to check.
         * @return Whether or not the button was pushed this frame.
         */
        static bool IsButtonPressed(MouseButton button_);

        /**
         * Was button released this frame.
         *
         * @param button_Button to check.
         * @return Whether or not the button was released this frame.
         */
        static bool IsButtonReleased(MouseButton button_);

        /**
         * Poll Mouse Inputs.
         */
        static void PollInputs();

        /**
         * Set mouse offset.
         * This is used internally for fixing mouse input.
         * This may produce unexpected results if used.
         *
         * @param ox_ X offset.
         * @param oy_ Y offset.
         */
        static void SetOffset(float ox_, float oy_);

        /**
         * Set mouse scale.
         * This is used internally for fixing mouse input.
         * This may produce unexpected results if used.
         *
         * @param sx_ X scale.
         * @param sy_ Y scale.
         */
        static void SetScale(float sx_, float sy_);
    };
}

#endif //MOUSE_H
