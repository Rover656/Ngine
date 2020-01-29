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

#include "Window.hpp"

// EGL stuff
#if defined(PLATFORM_UWP) // TODO: Add other platforms which use EGL
#include "Graphics/API/PlatformGLAPI.hpp"
#endif

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#endif

#include <stdexcept>

#include "Audio/AudioDevice.hpp"
#include "Graphics/GraphicsDevice.hpp"
#include "Input/Gamepad.hpp"
#include "Input/Mouse.hpp"
#include "Input/Keyboard.hpp"
#include "Console.hpp"

namespace Ngine {
    Window *Window::m_currentWindow = nullptr;

#if defined(PLATFORM_UWP)
    int Window::m_UWPWindowCount = 0;
#endif

    void Window::_applyConfig(const WindowConfig &config_) {
        if (!m_initialized) return;

        // Enable debug console
        SetEnableNativeConsole(config_.NativeDebugConsole);

        // Set fullscreen mode.
        SetFullscreen(config_.Fullscreen);

        // Set the window icon.
        SetIcon(config_.Icon);

        // Set resizeable or not.
        SetResizable(config_.Resizable);

        // Enable/disable V-Sync
        SetVSyncEnabled(config_.VSync);
    }

    Window::Window(const WindowConfig &config_) {
#if defined(PLATFORM_UWP)
        if (m_UWPWindowCount > 0)
            throw std::runtime_error("Cannot open more than one window on UWP.");
        m_UWPWindowCount++;
#endif
        // Init
#if defined(PLATFORM_DESKTOP)
        // Init GLFW
        if (!glfwInit()) {
            Console::Fail("Window", "Failed to init GLFW.");
        }
        Console::Notice("Window", "Initialized GLFW.");

        // Set defaults
        glfwDefaultWindowHints();

        // Apply MSAA
        if (config_.MSAA_4X) glfwWindowHint(GLFW_SAMPLES, 4);

        // Set version string
        auto targetAPI = Graphics::GraphicsDevice::GetTargetAPI();
        auto targetMajor = Graphics::GraphicsDevice::GetTargetAPIMajorVersion();
        auto targetMinor = Graphics::GraphicsDevice::GetTargetAPIMinorVersion();

        // OpenGL Core
        if (targetAPI == Graphics::GraphicsAPI::OpenGL) {
            // Set core profile for GL 3.2 or greater
            if (targetMajor >= 3 && targetMinor >= 2) {
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            }

            // Set target version
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, targetMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, targetMinor);
        } else if (targetAPI == Graphics::GraphicsAPI::OpenGLES) {
            // Setup for GLES
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
            glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

            // Set target version
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, targetMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, targetMinor);
        } else if (targetAPI == Graphics::GraphicsAPI::DirectX) {
            // No API
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }
#endif

        // Creation
#if defined(PLATFORM_DESKTOP)
        // Set callback
        glfwSetErrorCallback([] (int,const char*msg) {
            Console::Error("GLFW", "%s", msg);
        });

        // Create window
        m_GLFWWindow = glfwCreateWindow(config_.InitialWidth, config_.InitialHeight, config_.Title.c_str(), nullptr, nullptr);

        // If creation failed
        if (!m_GLFWWindow) {
            glfwTerminate();
            Console::Fail("Window", "Failed to create window with GLFW.");
        }

        Console::Notice("Window", "Successfully created GLFW window.");

        // Save title
        m_windowTitle = config_.Title;

        // Save this as a user pointer
        glfwSetWindowUserPointer(m_GLFWWindow, this);

        // Get initial size
        glfwGetWindowSize((GLFWwindow *)m_GLFWWindow, &m_currentWidth, &m_currentHeight);
#endif

        // Get window title (UWP)
#if defined(PLATFORM_UWP)
        auto pName = Windows::ApplicationModel::Package::Current->DisplayName;
        std::wstring tmp(pName->Begin());
        std::string name(tmp.begin(), tmp.end());
        m_windowTitle = name;
#endif

        // Initialized
        m_initialized = true;

        // Create graphics device
        m_graphicsDevice = new Graphics::GraphicsDevice(this);

#if defined(PLATFORM_UWP) // TODO: Add other platforms which use EGL
        // If we are using GLES, use the EGL API
        if (Graphics::GraphicsDevice::GetTargetAPI() == Graphics::GraphicsAPI::OpenGLES) {
            // Get EGL surface size
            ((Graphics::API::PlatformGLAPI *) m_graphicsDevice->GetAPI())->QueryEGLSurfaceSize(&m_currentWidth, &m_currentHeight);
        }
#endif

        // Make this window current
        MakeCurrent();

        // Create Input APIs
        m_mouseInput = new Input::Mouse(this);
        m_keyboardInput = new Input::Keyboard(this);
        Console::Notice("Window", "Successfully opened keyboard and mouse APIs.");

        // Apply any after-creation config
        _applyConfig(config_);
    }

    Window::~Window() {
        // Close if we aren't
        if (m_initialized) Close();
    }

    Graphics::GraphicsDevice *Window::GetGraphicsDevice() {
        return m_graphicsDevice;
    }

    void Window::MakeCurrent() {
        if (!m_initialized) throw std::runtime_error("This window is not ready to be made current!");
#if defined(PLATFORM_DESKTOP)
        // Make this context current
        glfwMakeContextCurrent((GLFWwindow*) m_GLFWWindow);
#elif defined(PLATFORM_UWP)
        // Make egl context current.
        if (Graphics::GraphicsDevice::GetTargetAPI() == Graphics::GraphicsAPI::OpenGLES)
            ((Graphics::API::PlatformGLAPI *) m_graphicsDevice->GetAPI())->MakeEGLCurrent();
#endif

        // Make current
        m_currentWindow = this;
    }

    Window *Window::GetCurrent() {
        return m_currentWindow;
    }

    Input::Mouse *Window::GetMouse() {
        return m_mouseInput;
    }

    Input::Keyboard *Window::GetKeyboard() {
        return m_keyboardInput;
    }

    int Window::GetWidth() {
        return m_currentWidth;
    }

    int Window::GetHeight() {
        return m_currentHeight;
    }

    void Window::Resize(int width_, int height_) {
#if defined(PLATFORM_DESKTOP)
        // Set size
        glfwSetWindowSize((GLFWwindow *)m_GLFWWindow, width_, height_);
#elif defined(PLATFORM_UWP)
        // Set size
        Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()->TryResizeView(Windows::Foundation::Size(width_, height_));
#endif
    }

    void Window::SetVSyncEnabled(bool flag_) {
        // Make this window context current
        MakeCurrent();

#if defined(PLATFORM_DESKTOP)
        glfwSwapInterval(flag_ ? 1 : 0);
#else
        if (Graphics::GraphicsDevice::GetTargetAPI() == Graphics::GraphicsAPI::OpenGLES)
            ((Graphics::API::PlatformGLAPI *) m_graphicsDevice->GetAPI())->SetEGLSwapInterval(flag_ ? 1 : 0);
#endif
    }

    void Window::SetIcon(Graphics::Image *icon_) {
#if defined(PLATFORM_DESKTOP)
        if (icon_ == nullptr) {
            glfwSetWindowIcon(m_GLFWWindow, 0, nullptr);
        } else {
            GLFWimage icon;
            icon.width = icon_->Width;
            icon.height = icon_->Height;
            icon.pixels = icon_->PixelData;
            glfwSetWindowIcon(m_GLFWWindow, 1, &icon);
        }
#endif
    }

    bool Window::IsFullscreen() {
        return m_isFullscreen;
    }

    void Window::ToggleFullscreen() {
        SetFullscreen(!m_isFullscreen);
    }

    void Window::SetFullscreen(bool fullscreen_) {
#if defined(PLATFORM_DESKTOP)
        // TODO: This is very buggy.

        // Get display info
        auto display = glfwGetPrimaryMonitor();
        auto vidmode = glfwGetVideoMode(display);

        // Toggle fullscreen
        if (fullscreen_ && !m_isFullscreen) {
            // Save info
            glfwGetWindowSize((GLFWwindow *)m_GLFWWindow, &m_preFullscreenSizeWidth, &m_preFullscreenSizeHeight);
            glfwGetWindowPos((GLFWwindow *)m_GLFWWindow, &m_preFullscreenPosX, &m_preFullscreenPosY);

            // Fullscreen
            glfwSetWindowMonitor((GLFWwindow *)m_GLFWWindow, display, 0, 0, vidmode->width, vidmode->height, GLFW_DONT_CARE);
            m_isFullscreen = true;
        } else {
            // No fullscreen
            if (m_isFullscreen) {
                glfwSetWindowMonitor((GLFWwindow *)m_GLFWWindow, nullptr, m_preFullscreenPosX, m_preFullscreenPosY, m_preFullscreenSizeWidth, m_preFullscreenSizeHeight, GLFW_DONT_CARE);
                m_isFullscreen = false;
            }
        }
#elif defined(PLATFORM_UWP)
        // Toggle fullscreen.
        m_fullscreenSet = fullscreen_ ? 1 : 2;
#endif
    }

    void Window::SetResizable(bool resizable_) {
#if defined(PLATFORM_DESKTOP)
        glfwSetWindowAttrib((GLFWwindow *)m_GLFWWindow, GLFW_RESIZABLE, resizable_ ? 1 : 0);
#endif
    }

    std::string Window::GetTitle() {
        return m_windowTitle;
    }

    void Window::SetTitle(const std::string& title_) {
#if defined(PLATFORM_DESKTOP)
        glfwSetWindowTitle(m_GLFWWindow, title_.c_str());
        m_windowTitle = title_;
#else
        Console::Notice("Window", "Window title was not changed as title changing is not enabled on this platform.");
#endif
    }

    void Window::SetEnableNativeConsole(bool flag_) {
#if defined(PLATFORM_DESKTOP) && defined(_WIN32)
        if (flag_ && !m_consoleAllocated) {
            // Add console
            AllocConsole();
            auto title = GetTitle();
            if (!title.empty())
                SetConsoleTitle((GetTitle() + ": Debug Console").c_str());
            else SetConsoleTitle("Ngine Game Debug Console.");

            // Save old buffers
            m_oldCinBuffer = std::cin.rdbuf();
            m_oldCoutBuffer = std::cout.rdbuf();
            m_oldCerrBuffer = std::cerr.rdbuf();

            // Open new buffers
            m_consoleIn.open("CONIN$", std::ios::in);
            m_consoleOut.open("CONOUT$", std::ios::out);
            m_consoleErr.open("CONOUT$", std::ios::out);
            std::cin.rdbuf(m_consoleIn.rdbuf());
            std::cout.rdbuf(m_consoleOut.rdbuf());
            std::cerr.rdbuf(m_consoleErr.rdbuf());

            // Disable close button
            HWND hwnd = GetConsoleWindow();
            if (hwnd != nullptr)
            {
                HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
                if (hMenu != nullptr) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
            }

            // Disable Ctrl+C
            SetConsoleCtrlHandler(nullptr, true);

            // Mark as open
            m_consoleAllocated = true;
            Console::Notice("Window", "Allocated a console to the game window. Output logs forwarded.");
        } else if (!flag_ && m_consoleAllocated) {
            // Close our buffers
            m_consoleIn.close();
            m_consoleOut.close();
            m_consoleErr.close();

            // Reset old
            std::cin.rdbuf(m_oldCinBuffer);
            std::cout.rdbuf(m_oldCoutBuffer);
            std::cerr.rdbuf(m_oldCerrBuffer);

            m_oldCinBuffer = nullptr;
            m_oldCoutBuffer = nullptr;
            m_oldCerrBuffer = nullptr;

            // Release console
            FreeConsole();

            // Mark as deallocated
            m_consoleAllocated = false;
            Console::Notice("Window", "Deallocated the console from the game window. Output logs restored.");
        }
#endif
    }

    bool Window::IsFocussed() {
#if defined(PLATFORM_DESKTOP)
        return glfwGetWindowAttrib((GLFWwindow *)m_GLFWWindow, GLFW_FOCUSED) == GLFW_TRUE;
#elif defined(PLATFORM_UWP)
        return m_focussed;
#endif

        // Default to true
        return true;
    }

    bool Window::IsVisible() {
#if defined(PLATFORM_DESKTOP)
        return glfwGetWindowAttrib((GLFWwindow *)m_GLFWWindow, GLFW_ICONIFIED) == 0;
#elif defined(PLATFORM_UWP)
        return m_visible;
#endif
        // Default case.
        return true;
    }

    void Window::PollEvents() {
#if defined(PLATFORM_DESKTOP)
        // Poll window events
        glfwPollEvents();

        // Query dimensions
        glfwGetWindowSize((GLFWwindow *)m_GLFWWindow, &m_currentWidth, &m_currentHeight);
#elif defined(PLATFORM_UWP)
        // Poll window events
        if (IsVisible())
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
        else
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);

        // Test visibility and focus
        m_focussed = CoreWindow::GetForCurrentThread()->ActivationMode == CoreWindowActivationMode::ActivatedInForeground;
        m_visible = CoreWindow::GetForCurrentThread()->Visible == true;

        // Query dimensions
        if (Graphics::GraphicsDevice::GetTargetAPI() == Graphics::GraphicsAPI::OpenGLES)
            ((Graphics::API::PlatformGLAPI *) m_graphicsDevice->GetAPI())->QueryEGLSurfaceSize(&m_currentWidth, &m_currentHeight);

        // Toggle fullscreen if necessary
        if (m_fullscreenSet != 0) {
            if (m_fullscreenSet == 1 && !m_isFullscreen) {
                Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()->TryEnterFullScreenMode();
                m_isFullscreen = true;
            }
            else if (m_fullscreenSet == 2 && m_isFullscreen) {
                Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()->ExitFullScreenMode();
                m_isFullscreen = false;
            }
            m_fullscreenSet = 0;
        }
#endif
    }

    void Window::Close() {
        // Destroy input APIs
        delete m_mouseInput;
        delete m_keyboardInput;

        // Delete graphics device
        delete m_graphicsDevice;

#if defined(PLATFORM_DESKTOP)
        // Destroy window
        glfwDestroyWindow((GLFWwindow*)m_GLFWWindow);

        // Disable GLFW
        glfwTerminate();
#endif

        // Ensure the console is deallocated.
        SetEnableNativeConsole(false);

        // Inform of closure
        Console::Notice("Window", "Closed window.");

        // Unset width and height as the window is closed
        m_currentWidth = 0;
        m_currentHeight = 0;
        m_initialized = false;
    }

    bool Window::ShouldClose() {
        auto windowWantsClose = false;
#if defined(PLATFORM_DESKTOP)
        windowWantsClose = glfwWindowShouldClose((GLFWwindow *)m_GLFWWindow);
#endif
        return windowWantsClose || m_keyboardInput->IsKeyDown(m_keyboardInput->m_exitKey);
    }

    void Window::SwapBuffers() {
#if defined(PLATFORM_DESKTOP)
        glfwSwapBuffers((GLFWwindow *) m_GLFWWindow);
#elif defined(PLATFORM_UWP)
        if (Graphics::GraphicsDevice::GetTargetAPI() == Graphics::GraphicsAPI::OpenGLES)
            ((Graphics::API::PlatformGLAPI *) m_graphicsDevice->GetAPI())->SwapEGLBuffers();
#endif
    }
}
