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

namespace ngine::input {
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
         * @param button The button that has changed.
         * @param pressed Whether it was pressed or not.
         */
        MouseButtonEventArgs(const MouseButton button, bool pressed)
                : Button(button), Pressed(pressed) {}
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
         * @param mousePosition The current position.
         * @param deltaMousePosition The delta position relative to the last position.
         */
        MouseMovedEventArgs(Vector2 mousePosition, Vector2 deltaMousePosition)
                : MousePosition(mousePosition), DeltaMousePosition(deltaMousePosition) {}
    };

    struct MouseScrollChangedEventArgs : EventArgs {
        /**
         * Scroll wheel value.
         */
        int Value;

        /**
         * Create a scroll changed event argument.
         *
         * @param value Scroll value.
         */
        MouseScrollChangedEventArgs(int value)
                : Value(value) {}
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
        friend class ngine::Window;

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
        Vector2 m_mouseOffset = {0, 0};

        /**
         * Mouse position scale.
         */
        Vector2 m_mouseScale = {1, 1};

        /**
         * Get the mouse position direct from the device.
         */
        Vector2 _internalGetMousePosition();

#if defined(PLATFORM_DESKTOP)
        static void _GLFWMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
        static void _GLFWScrollCallback(GLFWwindow *window, double x, double y);
#elif defined(PLATFORM_UWP)
        static Mouse *m_UWPMouse;
        static void _UWPPointerWheelChanged(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args);
        static void _UWPPointerButtonEvent(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args);
        static void _UWPPointerMovedEvent(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs ^args);
#endif

        /**
         * Create a new mouse input handler.
         */
        Mouse(Window *window);
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
         * @param button The button to cancel.
         */
        void cancelButton(MouseButton button);

        /**
         * Get mouse position.
         *
         * @return The mouse position in screen coordinates.
         */
        Vector2 getMousePosition();

        /**
         * Get mouse wheel X movement.
         *
         * @return Mouse wheel X movement.
         */
        int getMouseWheelXDelta();

        /**
         * Get mouse wheel Y movement.
         *
         * @return Mouse wheel Y movement.
         */
        int getMouseWheelYDelta();

        /**
         * Is button down.
         *
         * @param button Button to check.
         * @return Whether or not the button is down.
         */
        bool isButtonDown(MouseButton button);

        /**
         * Was button pushed this frame.
         *
         * @param button_Button to check.
         * @return Whether or not the button was pushed this frame.
         */
        bool isButtonPressed(MouseButton button);

        /**
         * Was button released this frame.
         *
         * @param button_Button to check.
         * @return Whether or not the button was released this frame.
         */
        bool isButtonReleased(MouseButton button);

        /**
         * Poll Mouse Inputs.
         */
        void pollInputs();

        /**
         * Set mouse offset.
         * This is used internally for fixing mouse input.
         *
         * @param xOffset X offset.
         * @param yOffset Y offset.
         */
        void setOffset(float xOffset, float yOffset);

        /**
         * Set mouse scale.
         * This is used internally for fixing mouse input.
         *
         * @param xScale X scale.
         * @param yScale Y scale.
         */
        void setScale(float xScale, float yScale);
    };
}

#endif //MOUSE_H
