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
#include "graphics/API/PlatformGLAPI.hpp"
#endif

#if defined(PLATFORM_DESKTOP)

#include <GLFW/glfw3.h>

#endif

#include <stdexcept>

#include "graphics/GraphicsDevice.hpp"
#include "input/Mouse.hpp"
#include "input/Keyboard.hpp"
#include "Console.hpp"

namespace ngine {
    Window *Window::m_currentWindow = nullptr;

#if defined(PLATFORM_UWP)
    int Window::m_UWPWindowCount = 0;
#endif

    void Window::_applyConfig(const WindowConfig &config) {
        if (!m_initialized) return;

        // Enable debug console
        setEnableNativeConsole(config.NativeDebugConsole);

        // Set fullscreen mode.
        setFullscreen(config.Fullscreen);

        // Set the window icon.
        setIcon(config.Icon);

        // Set resizeable or not.
        setResizable(config.Resizable);

        // Enable/disable V-Sync
        setVSyncEnabled(config.VSync);
    }

    Window::Window(const WindowConfig &config) : m_creationConfig(config), m_windowViewport(0, 0, config.InitialWidth, config.InitialHeight) {
#if defined(PLATFORM_UWP)
        if (m_UWPWindowCount > 0)
            throw std::runtime_error("Cannot open more than one window on UWP.");
        m_UWPWindowCount++;
#endif
        // Init
#if defined(PLATFORM_DESKTOP)
        // Init GLFW
        if (!glfwInit()) {
            Console::fail("Window", "Failed to init GLFW.");
        }
        Console::notice("Window", "Initialized GLFW.");

        // Set defaults
        glfwDefaultWindowHints();

        // Apply MSAA
        if (config.MSAA_4X) glfwWindowHint(GLFW_SAMPLES, 4);

        // Get API versions
        m_targetAPI = config.TargetAPI;
        m_targetMajor = config.TargetAPIMajorVersion;
        m_targetMinor = config.TargetAPIMinorVersion;

        if (m_targetAPI == graphics::GraphicsAPI::Default) {
#if defined(PLATFORM_DESKTOP)
#if defined(_WIN32)
            // Prefer ANGLE (DX) on Windows
            m_targetAPI = graphics::GraphicsAPI::OpenGLES;
            m_targetMajor = 2;
            m_targetMinor = 0;
#else
            m_targetAPI = graphics::GraphicsAPI::OpenGL;
            m_targetMajor = 3;
            m_targetMinor = 3;
#endif
#elif defined(PLATFORM_UWP)
            m_targetAPI = graphics::GraphicsAPI::OpenGLES;
            m_targetMajor = 2;
            m_targetMinor = 0;
#else
            Console::Fail("GraphicsDevice", "Unable to determine default graphics API.");
#endif
        }

        // OpenGL Core
        if (m_targetAPI == graphics::GraphicsAPI::OpenGL) {
            // Set core profile for GL 3.2 or greater
            if (m_targetMajor >= 3 && m_targetMinor >= 2) {
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            }

            // Set target version
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_targetMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_targetMinor);
        } else if (m_targetAPI == graphics::GraphicsAPI::OpenGLES) {
            // Setup for GLES
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
            glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

            // Set target version
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_targetMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_targetMinor);
        } else if (m_targetAPI == graphics::GraphicsAPI::DirectX) {
            // No API
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }
#endif

        // Creation
#if defined(PLATFORM_DESKTOP)
        // Set callback
        glfwSetErrorCallback([](int, const char *msg) {
            Console::error("GLFW", "%s", msg);
        });

        // Create window
        m_GLFWWindow = glfwCreateWindow(config.InitialWidth, config.InitialHeight, config.Title.c_str(), nullptr,
                                        nullptr);

        // If creation failed
        if (!m_GLFWWindow) {
            glfwTerminate();
            Console::fail("Window", "Failed to create window with GLFW.");
        }

        Console::notice("Window", "Successfully created GLFW window.");

        // Save title
        m_windowTitle = config.Title;

        // Save this as a user pointer
        glfwSetWindowUserPointer(m_GLFWWindow, this);

        // Get initial size
        glfwGetWindowSize((GLFWwindow *) m_GLFWWindow, &m_currentWidth, &m_currentHeight);
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
        m_graphicsDevice = new graphics::GraphicsDevice(this);

#if defined(PLATFORM_UWP) // TODO: Add other platforms which use EGL
        // If we are using GLES, use the EGL API
        if (graphics::GraphicsDevice::GetTargetAPI() == graphics::GraphicsAPI::OpenGLES) {
            // Get EGL surface size
            ((graphics::API::PlatformGLAPI *) m_graphicsDevice->getAPI())->QueryEGLSurfaceSize(&m_currentWidth, &m_currentHeight);
        }
#endif

        // Ensure viewport is up to date
        m_windowViewport.update(0, 0, m_currentWidth, m_currentHeight);

        // Make this window current
        makeCurrent();

        // Create Input APIs
        m_mouseInput = new input::Mouse(this);
        m_keyboardInput = new input::Keyboard(this);
        Console::notice("Window", "Successfully opened keyboard and mouse APIs.");

        // Apply any after-creation config
        _applyConfig(config);
    }

    Window::~Window() {
        // Close if we aren't
        if (m_initialized) close();
    }

    const WindowConfig Window::getCreationConfig() const {
        return m_creationConfig;
    }

    graphics::GraphicsAPI Window::getContextAPI() const {
        return m_targetAPI;
    }

    int Window::getContextAPIMajorVersion() const {
        return m_targetMajor;
    }

    int Window::getContextAPIMinorVersion() const {
        return m_targetMinor;
    }

    graphics::GraphicsDevice *Window::getGraphicsDevice() {
        return m_graphicsDevice;
    }

    void Window::makeCurrent() {
        if (!m_initialized) throw std::runtime_error("This window is not ready to be made current!");
#if defined(PLATFORM_DESKTOP)
        // Make this context current
        glfwMakeContextCurrent((GLFWwindow *) m_GLFWWindow);
#elif defined(PLATFORM_UWP)
        // Make egl context current.
        if (graphics::GraphicsDevice::GetTargetAPI() == graphics::GraphicsAPI::OpenGLES)
            ((graphics::API::PlatformGLAPI *) m_graphicsDevice->getAPI())->MakeEGLCurrent();
#endif

        // Make current
        m_currentWindow = this;
    }

    Window *Window::GetCurrent() {
        return m_currentWindow;
    }

    input::Mouse *Window::getMouse() {
        return m_mouseInput;
    }

    input::Keyboard *Window::getKeyboard() {
        return m_keyboardInput;
    }

    int Window::getWidth() {
        return m_currentWidth;
    }

    int Window::getHeight() {
        return m_currentHeight;
    }

    void Window::setSize(int width, int height) {
#if defined(PLATFORM_DESKTOP)
        // Set size
        glfwSetWindowSize((GLFWwindow *) m_GLFWWindow, width, height);
#elif defined(PLATFORM_UWP)
        // Set size
        Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()->TryResizeView(Windows::Foundation::Size(width, height));
#endif
    }

    const graphics::Viewport *Window::getWindowViewport() {
        return &m_windowViewport;
    }

    bool Window::getVSyncEnabled() {
        return m_vsync;
    }

    void Window::setVSyncEnabled(bool flag) {
        // Make this window context current
        makeCurrent();

        // Save for getting
        m_vsync = flag;

#if defined(PLATFORM_DESKTOP)
        glfwSwapInterval(flag ? 1 : 0);
#else
        if (graphics::GraphicsDevice::GetTargetAPI() == graphics::GraphicsAPI::OpenGLES)
            ((graphics::API::PlatformGLAPI *) m_graphicsDevice->getAPI())->SetEGLSwapInterval(flag ? 1 : 0);
#endif
    }

    void Window::setIcon(graphics::Image *icon) {
#if defined(PLATFORM_DESKTOP)
        if (icon == nullptr) {
            glfwSetWindowIcon(m_GLFWWindow, 0, nullptr);
        } else {
            GLFWimage glfwIcon;
            glfwIcon.width = icon->Width;
            glfwIcon.height = icon->Height;
            glfwIcon.pixels = icon->PixelData;
            glfwSetWindowIcon(m_GLFWWindow, 1, &glfwIcon);
        }
#endif
    }

    bool Window::isFullscreen() {
        return m_isFullscreen;
    }

    void Window::toggleFullscreen() {
        setFullscreen(!m_isFullscreen);
    }

    void Window::setFullscreen(bool fullscreen) {
#if defined(PLATFORM_DESKTOP)
        // TODO: This is very buggy.

        // Get display info
        auto display = glfwGetPrimaryMonitor();
        auto vidmode = glfwGetVideoMode(display);

        // Toggle fullscreen
        if (fullscreen && !m_isFullscreen) {
            // Save info
            glfwGetWindowSize((GLFWwindow *) m_GLFWWindow, &m_preFullscreenSizeWidth, &m_preFullscreenSizeHeight);
            glfwGetWindowPos((GLFWwindow *) m_GLFWWindow, &m_preFullscreenPosX, &m_preFullscreenPosY);

            // Fullscreen
            glfwSetWindowMonitor((GLFWwindow *) m_GLFWWindow, display, 0, 0, vidmode->width, vidmode->height,
                                 GLFW_DONT_CARE);
            m_isFullscreen = true;
        } else {
            // No fullscreen
            if (m_isFullscreen) {
                glfwSetWindowMonitor((GLFWwindow *) m_GLFWWindow, nullptr, m_preFullscreenPosX, m_preFullscreenPosY,
                                     m_preFullscreenSizeWidth, m_preFullscreenSizeHeight, GLFW_DONT_CARE);
                m_isFullscreen = false;
            }
        }
#elif defined(PLATFORM_UWP)
        // Toggle fullscreen.
        m_fullscreenSet = fullscreen ? 1 : 2;
#endif
    }

    void Window::setResizable(bool resizable) {
#if defined(PLATFORM_DESKTOP)
        glfwSetWindowAttrib((GLFWwindow *) m_GLFWWindow, GLFW_RESIZABLE, resizable ? 1 : 0);
#endif
    }

    std::string Window::getTitle() {
        return m_windowTitle;
    }

    void Window::setTitle(const std::string &title) {
#if defined(PLATFORM_DESKTOP)
        glfwSetWindowTitle(m_GLFWWindow, title.c_str());
        m_windowTitle = title;
#else
        Console::Notice("Window", "Window title was not changed as title changing is not enabled on this platform.");
#endif
    }

    void Window::setEnableNativeConsole(bool flag) {
#if defined(PLATFORM_DESKTOP) && defined(_WIN32)
        if (flag && !m_consoleAllocated) {
            // Add console
            AllocConsole();
            auto title = getTitle();
            if (!title.empty())
                SetConsoleTitle((getTitle() + ": Debug Console").c_str());
            else
                SetConsoleTitle("Ngine Game Debug Console.");

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
            if (hwnd != nullptr) {
                HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
                if (hMenu != nullptr) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
            }

            // Disable Ctrl+C
            SetConsoleCtrlHandler(nullptr, true);

            // Mark as open
            m_consoleAllocated = true;
            Console::notice("Window", "Allocated a console to the game window. Output logs forwarded.");
        } else if (!flag && m_consoleAllocated) {
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
            Console::notice("Window", "Deallocated the console from the game window. Output logs restored.");
        }
#endif
    }

    bool Window::isFocussed() {
#if defined(PLATFORM_DESKTOP)
        return glfwGetWindowAttrib((GLFWwindow *) m_GLFWWindow, GLFW_FOCUSED) == GLFW_TRUE;
#elif defined(PLATFORM_UWP)
        return m_focussed;
#endif

        // Default to true
        return true;
    }

    bool Window::isVisible() {
#if defined(PLATFORM_DESKTOP)
        return glfwGetWindowAttrib((GLFWwindow *) m_GLFWWindow, GLFW_ICONIFIED) == 0;
#elif defined(PLATFORM_UWP)
        return m_visible;
#endif
        // Default case.
        return true;
    }

    void Window::pollEvents() {
#if defined(PLATFORM_DESKTOP)
        // Poll window events
        glfwPollEvents();

        // Query dimensions
        glfwGetWindowSize((GLFWwindow *) m_GLFWWindow, &m_currentWidth, &m_currentHeight);
#elif defined(PLATFORM_UWP)
        // Poll window events
        if (isVisible())
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
        else
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);

        // Test visibility and focus
        m_focussed = CoreWindow::GetForCurrentThread()->ActivationMode == CoreWindowActivationMode::ActivatedInForeground;
        m_visible = CoreWindow::GetForCurrentThread()->Visible == true;

        // Query dimensions
        if (graphics::GraphicsDevice::GetTargetAPI() == graphics::GraphicsAPI::OpenGLES)
            ((graphics::API::PlatformGLAPI *) m_graphicsDevice->getAPI())->QueryEGLSurfaceSize(&m_currentWidth, &m_currentHeight);

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

        // Ensure viewport is up to date
        m_windowViewport.update(0, 0, m_currentWidth, m_currentHeight);
    }

    void Window::close() {
        // Destroy input APIs
        delete m_mouseInput;
        delete m_keyboardInput;

        // Delete graphics device
        delete m_graphicsDevice;

#if defined(PLATFORM_DESKTOP)
        // Destroy window
        glfwDestroyWindow((GLFWwindow *) m_GLFWWindow);

        // Disable GLFW
        glfwTerminate();
#endif

        // Ensure the console is deallocated.
        setEnableNativeConsole(false);

        // Inform of closure
        Console::notice("Window", "Closed window.");

        // Unset width and height as the window is closed
        m_currentWidth = 0;
        m_currentHeight = 0;
        m_initialized = false;
    }

    bool Window::shouldClose() {
        auto windowWantsClose = false;
#if defined(PLATFORM_DESKTOP)
        windowWantsClose = glfwWindowShouldClose((GLFWwindow *) m_GLFWWindow);
#endif
        return windowWantsClose || m_keyboardInput->isKeyDown(m_keyboardInput->m_exitKey);
    }

    void Window::swapBuffers() {
#if defined(PLATFORM_DESKTOP)
        glfwSwapBuffers((GLFWwindow *) m_GLFWWindow);
#elif defined(PLATFORM_UWP)
        if (graphics::GraphicsDevice::GetTargetAPI() == graphics::GraphicsAPI::OpenGLES)
            ((graphics::API::PlatformGLAPI *) m_graphicsDevice->getAPI())->SwapEGLBuffers();
#endif
    }
}
