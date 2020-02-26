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

#include "input/Keyboard.hpp"

#if defined(PLATFORM_DESKTOP)

#include <GLFW/glfw3.h>

#elif defined(PLATFORM_UWP)
#include "UWP/GameApp.hpp"
#endif

#include "Game.hpp"

namespace ngine::input {
#if defined(PLATFORM_DESKTOP)

    void Keyboard::_GLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto kbd = ((Window *) glfwGetWindowUserPointer(window))->getKeyboard();
        kbd->m_nextKeyState[key] = (action != GLFW_RELEASE);
        kbd->m_latestKeyPress = (Key) key;
    }

#elif defined(PLATFORM_UWP)
    Keyboard *Keyboard::m_UWPKeyboard = nullptr;

    Key Keyboard::_keyFromVirtualKey(int key) {
        Key actualKey = Key::None;
        switch (key)
        {
        case 0x08: actualKey = Key::Backspace; break;
        case 0x20: actualKey = Key::Space; break;
        case 0x1B: actualKey = Key::Escape; break;
        case 0x0D: actualKey = Key::Enter; break;
        case 0x2E: actualKey = Key::Delete; break;
        case 0x27: actualKey = Key::Right; break;
        case 0x25: actualKey = Key::Left; break;
        case 0x28: actualKey = Key::Down; break;
        case 0x26: actualKey = Key::Up; break;
        case 0x70: actualKey = Key::F1; break;
        case 0x71: actualKey = Key::F2; break;
        case 0x72: actualKey = Key::F3; break;
        case 0x73: actualKey = Key::F4; break;
        case 0x74: actualKey = Key::F5; break;
        case 0x75: actualKey = Key::F6; break;
        case 0x76: actualKey = Key::F7; break;
        case 0x77: actualKey = Key::F8; break;
        case 0x78: actualKey = Key::F9; break;
        case 0x79: actualKey = Key::F10; break;
        case 0x7A: actualKey = Key::F11; break;
        case 0x7B: actualKey = Key::F12; break;
        case 0xA0: actualKey = Key::LeftShift; break;
        case 0xA2: actualKey = Key::LeftControl; break;
        case 0xA4: actualKey = Key::LeftAlt; break;
        case 0xA1: actualKey = Key::RightShift; break;
        case 0xA3: actualKey = Key::RightControl; break;
        case 0xA5: actualKey = Key::RightAlt; break;
        case 0x30: actualKey = Key::Zero; break;
        case 0x31: actualKey = Key::One; break;
        case 0x32: actualKey = Key::Two; break;
        case 0x33: actualKey = Key::Three; break;
        case 0x34: actualKey = Key::Four; break;
        case 0x35: actualKey = Key::Five; break;
        case 0x36: actualKey = Key::Six; break;
        case 0x37: actualKey = Key::Seven; break;
        case 0x38: actualKey = Key::Eight; break;
        case 0x39: actualKey = Key::Nine; break;
        case 0x41: actualKey = Key::A; break;
        case 0x42: actualKey = Key::B; break;
        case 0x43: actualKey = Key::C; break;
        case 0x44: actualKey = Key::D; break;
        case 0x45: actualKey = Key::E; break;
        case 0x46: actualKey = Key::F; break;
        case 0x47: actualKey = Key::G; break;
        case 0x48: actualKey = Key::H; break;
        case 0x49: actualKey = Key::I; break;
        case 0x4A: actualKey = Key::J; break;
        case 0x4B: actualKey = Key::K; break;
        case 0x4C: actualKey = Key::L; break;
        case 0x4D: actualKey = Key::M; break;
        case 0x4E: actualKey = Key::N; break;
        case 0x4F: actualKey = Key::O; break;
        case 0x50: actualKey = Key::P; break;
        case 0x51: actualKey = Key::Q; break;
        case 0x52: actualKey = Key::R; break;
        case 0x53: actualKey = Key::S; break;
        case 0x54: actualKey = Key::T; break;
        case 0x55: actualKey = Key::U; break;
        case 0x56: actualKey = Key::V; break;
        case 0x57: actualKey = Key::W; break;
        case 0x58: actualKey = Key::X; break;
        case 0x59: actualKey = Key::Y; break;
        case 0x5A: actualKey = Key::Z; break;
        default: break;
        }
        return actualKey;
    }

    void Keyboard::_UWPKeyDown(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args) {
        Key key = _keyFromVirtualKey((int)args->VirtualKey);
        m_UWPKeyboard->m_nextKeyState[(int) key] = true;
    }

    void Keyboard::_UWPKeyUp(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args)
    {
        Key key = _keyFromVirtualKey((int)args->VirtualKey);
        m_UWPKeyboard->m_nextKeyState[(int) key] = false;
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

        // Set key states to default to prevent bugs
        for (auto i = 0; i <= KEY_MAX; i++) {
            m_currentKeyState[i] = m_previousKeyState[i] = m_nextKeyState[i] = false;
        }
    }

    Keyboard *Keyboard::getCurrent() {
        auto w = Window::GetCurrent();
        return w == nullptr ? nullptr : w->getKeyboard();
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

        return (Key)key;
    }

    Key Keyboard::getLatestKeypress() const {
        return m_latestKeyPress;
    }

    bool Keyboard::isKeyDown(Key key) const {
        if (key == Key::None) return false;
        return m_currentKeyState[(int) key];
    }

    bool Keyboard::isKeyDown(char key) const {
        return isKeyDown(getKey(key));
    }

    bool Keyboard::isKeyPressed(Key key) const {
        if (key == Key::None) return false;
        return m_currentKeyState[(int) key] != m_previousKeyState[(int) key] && m_currentKeyState[(int) key];
    }

    bool Keyboard::isKeyPressed(char key) const {
        return isKeyPressed(getKey(key));
    }

    bool Keyboard::isKeyReleased(Key key) const {
        if (key == Key::None) return false;
        return m_currentKeyState[(int) key] != m_previousKeyState[(int) key] && !m_currentKeyState[(int) key];
    }

    bool Keyboard::isKeyReleased(char key) const {
        return isKeyReleased(getKey(key));
    }

    void Keyboard::pollInputs() {
        // Move current to last and next into current
        for (auto i = 0; i <= KEY_MAX; i++) {
            m_previousKeyState[i] = m_currentKeyState[i];
            m_currentKeyState[i] = m_nextKeyState[i];
        }

        // Fire events
        for (auto i = 0; i <= KEY_MAX; i++) {
            auto k = (Key) i;
            if (isKeyPressed(k)) {
                EventDispatcher::fire(KeyPressEvent(this, m_window, k));
            } else if (isKeyReleased(k)) {
                EventDispatcher::fire(KeyReleaseEvent(this, m_window, k));
            }
        }
    }

    void Keyboard::setExitKey(Key key) {
        m_exitKey = key;
    }
}
