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
#endif

#include "../Events.hpp"
#include "../Math.hpp"
#include "../Window.hpp"

namespace ngine::input {
    /**
     * Mouse button
     */
    enum class MouseButton : int {
        Left = 0,
        Right = 1,
        Middle = 2
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
         * Mouse button pressed event.
         */
        class ButtonPressedEvent : public Event {
        public:
            /**
             * The window this event was fired for.
             */
            ngine::Window *Window;

            /**
             * The button pressed.
             */
            MouseButton Button;

            /**
             * Create a button pressed event.
             *
             * @param sender The sender.
             * @param window The window this was fired for.
             * @param button The button pressed.
             */
            ButtonPressedEvent(Mouse *sender, ngine::Window *window, MouseButton button) : Window(window),
                                                                                           Button(button),
                                                                                           Event(sender) {}
        };

        /**
         * Mouse button released event.
         */
        class ButtonReleasedEvent : public Event {
        public:
            /**
             * The window this event was fired for.
             */
            ngine::Window *Window;

            /**
             * The button released.
             */
            MouseButton Button;

            /**
             * Create a button pressed event.
             *
             * @param sender The sender.
             * @param window The window this was fired for.
             * @param button The button released.
             */
            ButtonReleasedEvent(Mouse *sender, ngine::Window *window, MouseButton button) : Window(window),
                                                                                            Button(button),
                                                                                            Event(sender) {}
        };

        /**
         * Mouse moved event.
         */
        class MovedEvent : public Event {
        public:
            /**
             * The window this event was fired for.
             */
            ngine::Window *Window;

            /**
             * The mouse position.
             */
            Vector2 CurrentPosition;

            /**
             * The mouse delta position.
             */
            Vector2 DeltaPosition;

            /**
             * Create a button pressed event.
             *
             * @param sender The sender.
             * @param window The window this was fired for.
             * @param pos The current mouse position.
             * @param delta The delta mouse position.
             */
            MovedEvent(Mouse *sender, ngine::Window *window, Vector2 pos, Vector2 delta) : Window(window),
                                                                                           CurrentPosition(pos),
                                                                                           DeltaPosition(delta),
                                                                                           Event(sender) {}
        };

        /**
         * Mouse wheel Y changed event.
         */
        class WheelYChangedEvent : public Event {
        public:
            /**
             * The window this event was fired for.
             */
            ngine::Window *Window;

            /**
             * The scroll value.
             */
            int Value;

            /**
             * Create a button pressed event.
             *
             * @param sender The sender.
             * @param window The window this was fired for.
             * @param pos The current mouse position.
             * @param delta The delta mouse position.
             */
            WheelYChangedEvent(Mouse *sender, ngine::Window *window, float value) : Window(window),
                                                                                    Value(value),
                                                                                    Event(sender) {}
        };

        /**
         * Mouse wheel X changed event.
         */
        class WheelXChangedEvent : public Event {
        public:
            /**
             * The window this event was fired for.
             */
            ngine::Window *Window;

            /**
             * The scroll value.
             */
            float Value;

            /**
             * Create a button pressed event.
             *
             * @param sender The sender.
             * @param window The window this was fired for.
             * @param pos The current mouse position.
             * @param delta The delta mouse position.
             */
            WheelXChangedEvent(Mouse *sender, ngine::Window *window, float value) : Window(window),
                                                                                    Value(value),
                                                                                    Event(sender) {}
        };

        // TODO: CLEAN ORDER

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
