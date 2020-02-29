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

#include "ngine/input/Keyboard.hpp"

#if defined(PLATFORM_DESKTOP)

#include <GLFW/glfw3.h>

#elif defined(PLATFORM_UWP)
#include "ngine/UWP/GameApp.hpp"
#endif

#include "ngine/Game.hpp"
#include "ngine/Window.hpp"

namespace ngine::input {
    KeyboardState::KeyboardState() {
        // Set to defaults
        for (auto i = 0; i <= KEY_MAX; i++) {
            m_keys[i] = 0;
        }
    }

    bool KeyboardState::getKeyState(Key key) const {
        if (key == Key::None) return false;
        return m_keys[(int) key];
    }

    void KeyboardState::setKeyState(Key key, bool state) {
        if (key == Key::None) return;
        m_keys[(int) key] = state;
    }

#if defined(PLATFORM_DESKTOP)

    void Keyboard::_GLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto kbd = ((Window *) glfwGetWindowUserPointer(window))->getKeyboard();
        // TODO: If we find its released again, should we keep it as pressed if it is already so we don't miss is?
        kbd->m_nextState.setKeyState((Key) key, action != GLFW_RELEASE);

        // Fire event
        if (action == GLFW_PRESS) {
            EventDispatcher::fire(KeyPressEvent(kbd,
                                                kbd->m_window,
                                                (Key) key,
                                                (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT,
                                                (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL,
                                                (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT,
                                                (mods & GLFW_MOD_SUPER) == GLFW_MOD_SUPER,
                                                (mods & GLFW_MOD_CAPS_LOCK) == GLFW_MOD_CAPS_LOCK,
                                                (mods & GLFW_MOD_NUM_LOCK) == GLFW_MOD_NUM_LOCK));
        } else if (action == GLFW_RELEASE) {
            EventDispatcher::fire(KeyReleaseEvent(kbd,
                                                  kbd->m_window,
                                                  (Key) key,
                                                  (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT,
                                                  (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL,
                                                  (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT,
                                                  (mods & GLFW_MOD_SUPER) == GLFW_MOD_SUPER,
                                                  (mods & GLFW_MOD_CAPS_LOCK) == GLFW_MOD_CAPS_LOCK,
                                                  (mods & GLFW_MOD_NUM_LOCK) == GLFW_MOD_NUM_LOCK));
        }
    }

#elif defined(PLATFORM_UWP)
    Keyboard *Keyboard::m_UWPKeyboard = nullptr;

    Key Keyboard::_keyFromVirtualKey(int key) {
        using Windows::System::VirtualKey;

        Key actualKey = Key::None;
        switch ((VirtualKey) key)
        {
        // No apostrophe
        // No comma
        // No minus
        // No period
        // No slash
        case VirtualKey::Number0: actualKey = Key::Zero; break;
        case VirtualKey::Number1: actualKey = Key::One; break;
        case VirtualKey::Number2: actualKey = Key::Two; break;
        case VirtualKey::Number3: actualKey = Key::Three; break;
        case VirtualKey::Number4: actualKey = Key::Four; break;
        case VirtualKey::Number5: actualKey = Key::Five; break;
        case VirtualKey::Number6: actualKey = Key::Six; break;
        case VirtualKey::Number7: actualKey = Key::Seven; break;
        case VirtualKey::Number8: actualKey = Key::Eight; break;
        case VirtualKey::Number9: actualKey = Key::Nine; break;

        // No semicolon
        // No equals

        case VirtualKey::A: actualKey = Key::A; break;
        case VirtualKey::B: actualKey = Key::B; break;
        case VirtualKey::C: actualKey = Key::C; break;
        case VirtualKey::D: actualKey = Key::D; break;
        case VirtualKey::E: actualKey = Key::E; break;
        case VirtualKey::F: actualKey = Key::F; break;
        case VirtualKey::G: actualKey = Key::G; break;
        case VirtualKey::H: actualKey = Key::H; break;
        case VirtualKey::I: actualKey = Key::I; break;
        case VirtualKey::J: actualKey = Key::J; break;
        case VirtualKey::K: actualKey = Key::K; break;
        case VirtualKey::L: actualKey = Key::L; break;
        case VirtualKey::M: actualKey = Key::M; break;
        case VirtualKey::N: actualKey = Key::N; break;
        case VirtualKey::O: actualKey = Key::O; break;
        case VirtualKey::P: actualKey = Key::P; break;
        case VirtualKey::Q: actualKey = Key::Q; break;
        case VirtualKey::R: actualKey = Key::R; break;
        case VirtualKey::S: actualKey = Key::S; break;
        case VirtualKey::T: actualKey = Key::T; break;
        case VirtualKey::U: actualKey = Key::U; break;
        case VirtualKey::V: actualKey = Key::V; break;
        case VirtualKey::W: actualKey = Key::W; break;
        case VirtualKey::X: actualKey = Key::X; break;
        case VirtualKey::Y: actualKey = Key::Y; break;
        case VirtualKey::Z: actualKey = Key::Z; break;

        case VirtualKey::Space: actualKey = Key::Space; break;
        case VirtualKey::Escape: actualKey = Key::Escape; break;
        case VirtualKey::Enter: actualKey = Key::Enter; break;
        case VirtualKey::Tab: actualKey = Key::Tab; break;
        case VirtualKey::Back: actualKey = Key::Backspace; break;
        case VirtualKey::Insert: actualKey = Key::Insert; break;
        case VirtualKey::Delete: actualKey = Key::Delete; break;
        case VirtualKey::Right: actualKey = Key::Right; break;
        case VirtualKey::Left: actualKey = Key::Left; break;
        case VirtualKey::Down: actualKey = Key::Down; break;
        case VirtualKey::Up: actualKey = Key::Up; break;
        case VirtualKey::PageUp: actualKey = Key::PageUp; break;
        case VirtualKey::PageDown: actualKey = Key::PageDown; break;
        case VirtualKey::Home: actualKey = Key::Home; break;
        case VirtualKey::End: actualKey = Key::End; break;
        case VirtualKey::CapitalLock: actualKey = Key::CapsLock; break;
        case VirtualKey::Scroll: actualKey = Key::ScrollLock; break;
        case VirtualKey::NumberKeyLock: actualKey = Key::NumLock; break;
        // No print screen
        case VirtualKey::Pause: actualKey = Key::Pause; break;
        case VirtualKey::F1: actualKey = Key::F1; break;
        case VirtualKey::F2: actualKey = Key::F2; break;
        case VirtualKey::F3: actualKey = Key::F3; break;
        case VirtualKey::F4: actualKey = Key::F4; break;
        case VirtualKey::F5: actualKey = Key::F5; break;
        case VirtualKey::F6: actualKey = Key::F6; break;
        case VirtualKey::F7: actualKey = Key::F7; break;
        case VirtualKey::F8: actualKey = Key::F8; break;
        case VirtualKey::F9: actualKey = Key::F9; break;
        case VirtualKey::F10: actualKey = Key::F10; break;
        case VirtualKey::F11: actualKey = Key::F11; break;
        case VirtualKey::F12: actualKey = Key::F12; break;
        case VirtualKey::LeftShift: actualKey = Key::LeftShift; break;
        case VirtualKey::LeftControl: actualKey = Key::LeftControl; break;
        // No left alt
        case VirtualKey::LeftWindows: actualKey = Key::LeftSuper; break;
        case VirtualKey::RightShift: actualKey = Key::RightShift; break;
        case VirtualKey::RightControl: actualKey = Key::RightControl; break;
        // No right alt
        case VirtualKey::RightWindows: actualKey = Key::RightSuper; break;
        case VirtualKey::Menu: actualKey = Key::Menu; break;
        // No left bracket
        // No backslash
        // No right bracket
        // No grave
        
        case VirtualKey::NumberPad0: actualKey = Key::NumPad0; break;
        case VirtualKey::NumberPad1: actualKey = Key::NumPad1; break;
        case VirtualKey::NumberPad2: actualKey = Key::NumPad2; break;
        case VirtualKey::NumberPad3: actualKey = Key::NumPad3; break;
        case VirtualKey::NumberPad4: actualKey = Key::NumPad4; break;
        case VirtualKey::NumberPad5: actualKey = Key::NumPad5; break;
        case VirtualKey::NumberPad6: actualKey = Key::NumPad6; break;
        case VirtualKey::NumberPad7: actualKey = Key::NumPad7; break;
        case VirtualKey::NumberPad8: actualKey = Key::NumPad8; break;
        case VirtualKey::NumberPad9: actualKey = Key::NumPad9; break;
        case VirtualKey::Add: actualKey = Key::NumPadAdd; break;
        case VirtualKey::Subtract: actualKey = Key::NumPadSubtract; break;
        case VirtualKey::Multiply: actualKey = Key::NumPadMultiply; break;
        case VirtualKey::Divide: actualKey = Key::NumPadDivide; break;
        
        default: break;
        }
        return actualKey;
    }

    void Keyboard::_UWPKeyDown(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args) {
        Key key = _keyFromVirtualKey((int)args->VirtualKey);
        m_UWPKeyboard->m_nextKeyState[(int) key] = true;

        // Fire event
        EventDispatcher::fire(KeyPressEvent(m_UWPKeyboard,
            m_UWPKeyboard->m_window,
            (Key)key,
            (sender->GetKeyState(Windows::System::VirtualKey::Shift) & CoreVirtualKeyStates::Down) == CoreVirtualKeyStates::Down,
            (sender->GetKeyState(Windows::System::VirtualKey::Control) & CoreVirtualKeyStates::Down) == CoreVirtualKeyStates::Down,
            false, // No alt
            (sender->GetKeyState(Windows::System::VirtualKey::LeftWindows) & CoreVirtualKeyStates::Down) == CoreVirtualKeyStates::Down
                || (sender->GetKeyState(Windows::System::VirtualKey::RightWindows) & CoreVirtualKeyStates::Down) == CoreVirtualKeyStates::Down, // Annoyingly, we need to check both
            (sender->GetKeyState(Windows::System::VirtualKey::CapitalLock) & CoreVirtualKeyStates::Locked) == CoreVirtualKeyStates::Locked,
            (sender->GetKeyState(Windows::System::VirtualKey::NumberKeyLock) & CoreVirtualKeyStates::Locked) == CoreVirtualKeyStates::Locked));
    }

    void Keyboard::_UWPKeyUp(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args) {
        Key key = _keyFromVirtualKey((int)args->VirtualKey);
        m_UWPKeyboard->m_nextKeyState[(int) key] = false;

        // Fire event
        EventDispatcher::fire(KeyReleaseEvent(m_UWPKeyboard,
            m_UWPKeyboard->m_window,
            (Key)key,
            (sender->GetKeyState(Windows::System::VirtualKey::Shift) & CoreVirtualKeyStates::Down) == CoreVirtualKeyStates::Down,
            (sender->GetKeyState(Windows::System::VirtualKey::Control) & CoreVirtualKeyStates::Down) == CoreVirtualKeyStates::Down,
            false, // No alt
            (sender->GetKeyState(Windows::System::VirtualKey::LeftWindows) & CoreVirtualKeyStates::Down) == CoreVirtualKeyStates::Down
                || (sender->GetKeyState(Windows::System::VirtualKey::RightWindows) & CoreVirtualKeyStates::Down) == CoreVirtualKeyStates::Down, // Annoyingly, we need to check both
            (sender->GetKeyState(Windows::System::VirtualKey::CapitalLock) & CoreVirtualKeyStates::Locked) == CoreVirtualKeyStates::Locked,
            (sender->GetKeyState(Windows::System::VirtualKey::NumberKeyLock) & CoreVirtualKeyStates::Locked) == CoreVirtualKeyStates::Locked));
    }
#endif

    Keyboard::Keyboard(Window *window) : m_window(window) {
#if defined(PLATFORM_DESKTOP)
        // Register events
        glfwSetKeyCallback(m_window->m_GLFWWindow, Keyboard::_GLFWKeyCallback);
#elif defined(PLATFORM_UWP)
        // Throw error.
        if (m_UWPKeyboard != nullptr)
            throw std::runtime_error("On UWP only one keyboard can be created.");

        // Save this. UWP can only have one window.
        m_UWPKeyboard = this;

        // UWP events
        auto UWPwindow = CoreWindow::GetForCurrentThread();
        UWPwindow->KeyDown += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::KeyEventArgs ^>(&_UWPKeyDown);
        UWPwindow->KeyUp += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::KeyEventArgs ^>(&_UWPKeyUp);
#endif
    }

    const KeyboardState &Keyboard::getState() const {
        return m_currentState;
    }

    Key Keyboard::getKey(char key) {
        // Skip keys that we can't have
        if (key < 32 || key > 122) return Key::None;
        if (key > 32 && key < 39) return Key::None;
        if (key > 39 && key < 44) return Key::None;
        if (key > 57 && key < 61) return Key::None;
        if (key > 61 && key < 64) return Key::None;
        if (key == 94) return Key::None;

        // Convert lower to upper
        if (key >= 97 && key <= 122) {
            key -= 32;
        }

        return (Key) key;
    }

    bool Keyboard::isKeyDown(Key key) const {
        return m_currentState.getKeyState(key);
    }

    bool Keyboard::isKeyDown(char key) const {
        return isKeyDown(getKey(key));
    }

    bool Keyboard::isKeyPressed(Key key) const {
        return m_currentState.getKeyState(key) && !m_previousState.getKeyState(key);
    }

    bool Keyboard::isKeyPressed(char key) const {
        return isKeyPressed(getKey(key));
    }

    bool Keyboard::isKeyReleased(Key key) const {
        return !m_currentState.getKeyState(key) && m_previousState.getKeyState(key);
    }

    bool Keyboard::isKeyReleased(char key) const {
        return isKeyReleased(getKey(key));
    }

    void Keyboard::pollInputs() {
        // Move current to last and next into current
        m_previousState = m_currentState;
        m_currentState = m_nextState;
    }
}
