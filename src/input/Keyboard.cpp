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

    void Keyboard::_GLFWKeyCallback(GLFWwindow *window_, int key_, int scancode_, int action_, int mods_) {
        auto kbd = ((Window *) glfwGetWindowUserPointer(window_))->getKeyboard();
        kbd->m_nextKeyState[key_] = (action_ != GLFW_RELEASE);
        kbd->m_latestKeyPress = (Key)key_;
    }

#elif defined(PLATFORM_UWP)
    Keyboard *Keyboard::m_UWPKeyboard = nullptr;

    Key Keyboard::_keyFromVirtualKey(int key_) {
        Key actualKey = KEY_NONE;
        switch (key_)
        {
            case 0x08: actualKey = KEY_BACKSPACE; break;
            case 0x20: actualKey = KEY_SPACE; break;
            case 0x1B: actualKey = KEY_ESCAPE; break;
            case 0x0D: actualKey = KEY_ENTER; break;
            case 0x2E: actualKey = KEY_DELETE; break;
            case 0x27: actualKey = KEY_RIGHT; break;
            case 0x25: actualKey = KEY_LEFT; break;
            case 0x28: actualKey = KEY_DOWN; break;
            case 0x26: actualKey = KEY_UP; break;
            case 0x70: actualKey = KEY_F1; break;
            case 0x71: actualKey = KEY_F2; break;
            case 0x72: actualKey = KEY_F3; break;
            case 0x73: actualKey = KEY_F4; break;
            case 0x74: actualKey = KEY_F5; break;
            case 0x75: actualKey = KEY_F6; break;
            case 0x76: actualKey = KEY_F7; break;
            case 0x77: actualKey = KEY_F8; break;
            case 0x78: actualKey = KEY_F9; break;
            case 0x79: actualKey = KEY_F10; break;
            case 0x7A: actualKey = KEY_F11; break;
            case 0x7B: actualKey = KEY_F12; break;
            case 0xA0: actualKey = KEY_LEFT_SHIFT; break;
            case 0xA2: actualKey = KEY_LEFT_CONTROL; break;
            case 0xA4: actualKey = KEY_LEFT_ALT; break;
            case 0xA1: actualKey = KEY_RIGHT_SHIFT; break;
            case 0xA3: actualKey = KEY_RIGHT_CONTROL; break;
            case 0xA5: actualKey = KEY_RIGHT_ALT; break;
            case 0x30: actualKey = KEY_ZERO; break;
            case 0x31: actualKey = KEY_ONE; break;
            case 0x32: actualKey = KEY_TWO; break;
            case 0x33: actualKey = KEY_THREE; break;
            case 0x34: actualKey = KEY_FOUR; break;
            case 0x35: actualKey = KEY_FIVE; break;
            case 0x36: actualKey = KEY_SIX; break;
            case 0x37: actualKey = KEY_SEVEN; break;
            case 0x38: actualKey = KEY_EIGHT; break;
            case 0x39: actualKey = KEY_NINE; break;
            case 0x41: actualKey = KEY_A; break;
            case 0x42: actualKey = KEY_B; break;
            case 0x43: actualKey = KEY_C; break;
            case 0x44: actualKey = KEY_D; break;
            case 0x45: actualKey = KEY_E; break;
            case 0x46: actualKey = KEY_F; break;
            case 0x47: actualKey = KEY_G; break;
            case 0x48: actualKey = KEY_H; break;
            case 0x49: actualKey = KEY_I; break;
            case 0x4A: actualKey = KEY_J; break;
            case 0x4B: actualKey = KEY_K; break;
            case 0x4C: actualKey = KEY_L; break;
            case 0x4D: actualKey = KEY_M; break;
            case 0x4E: actualKey = KEY_N; break;
            case 0x4F: actualKey = KEY_O; break;
            case 0x50: actualKey = KEY_P; break;
            case 0x51: actualKey = KEY_Q; break;
            case 0x52: actualKey = KEY_R; break;
            case 0x53: actualKey = KEY_S; break;
            case 0x54: actualKey = KEY_T; break;
            case 0x55: actualKey = KEY_U; break;
            case 0x56: actualKey = KEY_V; break;
            case 0x57: actualKey = KEY_W; break;
            case 0x58: actualKey = KEY_X; break;
            case 0x59: actualKey = KEY_Y; break;
            case 0x5A: actualKey = KEY_Z; break;
            default: break;
        }
        return actualKey;
    }

    void Keyboard::_UWPKeyDown(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args) {
        Key key = _keyFromVirtualKey((int)args->VirtualKey);
        m_UWPKeyboard->m_nextKeyState[key] = true;
    }

    void Keyboard::_UWPKeyUp(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args)
    {
        Key key = _keyFromVirtualKey((int)args->VirtualKey);
        m_UWPKeyboard->m_nextKeyState[key] = false;
    }
#endif

    Keyboard::Keyboard(Window *window_) {
#if defined(PLATFORM_DESKTOP)
        // Register events
        glfwSetKeyCallback(window_->m_GLFWWindow, Keyboard::_GLFWKeyCallback);
#elif defined(PLATFORM_UWP)
        // Throw error.
        if (m_UWPKeyboard != nullptr)
            throw std::runtime_error("On UWP only one keyboard can be created.");

        // Save this. UWP can only have one window.
        m_UWPKeyboard = this;

        // UWP events
        auto window = CoreWindow::GetForCurrentThread();
        window->KeyDown += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::KeyEventArgs ^>(&_UWPKeyDown);
        window->KeyUp += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::KeyEventArgs ^>(&_UWPKeyUp);
#endif

        // Set key states to default to prevent bugs
        for (auto i = 0; i <= KEY_MAX; i++) {
            m_currentKeyState[i] = m_previousKeyState[i] = m_nextKeyState[i] = false;
        }
    }

    Keyboard *Keyboard::GetCurrent() {
        auto w = Window::GetCurrent();
        return w == nullptr ? nullptr : w->getKeyboard();
    }

    Key Keyboard::getLatestKeypress() {
        return m_latestKeyPress;
    }

    bool Keyboard::isKeyDown(Key key_) {
        if (key_ == KEY_NONE) return false;
        return m_currentKeyState[key_];
    }

    bool Keyboard::isKeyPressed(Key key_) {
        return m_currentKeyState[key_] != m_previousKeyState[key_] && m_currentKeyState[key_];
    }

    bool Keyboard::isKeyReleased(Key key_) {
        return m_currentKeyState[key_] != m_previousKeyState[key_] && !m_currentKeyState[key_];
    }

    bool Keyboard::isKeyUp(Key key_) {
        if (key_ == KEY_NONE) return true;
        return !isKeyDown(key_);
    }

    void Keyboard::pollInputs() {
        // Move current to last and next into current
        for (auto i = 0; i <= KEY_MAX; i++) {
            m_previousKeyState[i] = m_currentKeyState[i];
            m_currentKeyState[i] = m_nextKeyState[i];
        }

        // Fire events
        for (auto i = 0; i <= KEY_MAX; i++) {
            auto k = (Key)i;
            if (isKeyPressed(k)) {
                KeyPressed({k});
            } else if (isKeyReleased(k)) {
                KeyReleased({k});
            }
        }
    }

    void Keyboard::setExitKey(Key key_) {
        m_exitKey = key_;
    }
}
