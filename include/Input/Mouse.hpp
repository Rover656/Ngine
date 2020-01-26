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

#ifndef MOUSE_H
#define MOUSE_H

#include "../Config.hpp"

#if defined(PLATFORM_DESKTOP)
struct GLFWwindow;
#elif defined(PLATFORM_UWP)
#include "../UWP/GameApp.hpp"
#endif

#include "../Events.hpp"
#include "../Math.hpp"
#include "../Window.hpp"

namespace Ngine::Input {
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
        /**
         * Scroll wheel value.
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
        // Window needs our constructor
        friend class Ngine::Window;

        /**
         * The window we are attached to.
         */
        Window *m_attachedWindow;

        /**
         * The last frame mouse position
         */
        MouseState m_previousMouseState;

        /**
         * The current mouse state
         */
        MouseState m_currentMouseState;

        /**
         * The next mouse state that is being built while the frame is progressing.
         */
        MouseState m_nextMouseState;

        /**
         * Mouse position offset.
         */
        Vector2 m_mouseOffset;

        /**
         * Mouse position scale.
         */
        Vector2 m_mouseScale;

        /**
         * Get the mouse position direct from the device.
         */
        Vector2 _internalGetMousePosition();

#if defined(PLATFORM_DESKTOP)
        static void _GLFWMouseButtonCallback(GLFWwindow *window_, int button_, int action_, int mods_);
        static void _GLFWScrollCallback(GLFWwindow *window_, double x_, double y_);
#elif defined(PLATFORM_UWP)
        static Mouse *m_UWPMouse;
        static void _UWPPointerWheelChanged(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args);
        static void _UWPPointerButtonEvent(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args);
        static void _UWPPointerMovedEvent(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args);
#endif

        /**
         * Create a new mouse input handler.
         */
        Mouse(Window *window_);
    public:
        /**
         * On mouse button pressed.
         */
        Event<MouseButtonEventArgs> OnMouseButtonPressed;

        /**
         * On mouse button released.
         */
        Event<MouseButtonEventArgs> OnMouseButtonReleased;

        /**
         * On mouse moved event.
         */
        Event<MouseMovedEventArgs> OnMouseMoved;

        /**
         * On mouse scroll X changed.
         */
        Event<MouseScrollChangedEventArgs> OnMouseScrollXChanged;

        /**
         * On mouse scroll Y changed.
         */
        Event<MouseScrollChangedEventArgs> OnMouseScrollYChanged;

        // TODO: CLEAN ORDER

        /**
         * Get the current window's mouse manager.
         *
         * @return The current window's mouse manager.
         */
        static Mouse *GetCurrent();

        /**
         * Cancel button press (Prevents double event checks).
         *
         * @param button_ The button to cancel.
         */
        void CancelButton(MouseButton button_);

        /**
         * Get mouse position.
         *
         * @return The mouse position in screen coordinates.
         */
        Vector2 GetMousePosition();

        /**
         * Get mouse wheel X movement.
         *
         * @return Mouse wheel X movement.
         */
        int GetMouseWheelXDelta();

        /**
         * Get mouse wheel Y movement.
         *
         * @return Mouse wheel Y movement.
         */
        int GetMouseWheelYDelta();

        /**
         * Is button down.
         *
         * @param button_ Button to check.
         * @return Whether or not the button is down.
         */
        bool IsButtonDown(MouseButton button_);

        /**
         * Was button pushed this frame.
         *
         * @param button_Button to check.
         * @return Whether or not the button was pushed this frame.
         */
        bool IsButtonPressed(MouseButton button_);

        /**
         * Was button released this frame.
         *
         * @param button_Button to check.
         * @return Whether or not the button was released this frame.
         */
        bool IsButtonReleased(MouseButton button_);

        /**
         * Poll Mouse Inputs.
         */
        void PollInputs();

        /**
         * Set mouse offset.
         * This is used internally for fixing mouse input.
         *
         * @param ox_ X offset.
         * @param oy_ Y offset.
         */
        void SetOffset(float ox_, float oy_);

        /**
         * Set mouse scale.
         * This is used internally for fixing mouse input.
         *
         * @param sx_ X scale.
         * @param sy_ Y scale.
         */
        void SetScale(float sx_, float sy_);
    };
}

#endif //MOUSE_H
                                                                                                                  