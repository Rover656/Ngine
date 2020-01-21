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

// Platform specifics
#if defined(GRAPHICS_OPENGL33)
#include <glad/glad.h>
#elif defined(GRAPHICS_OPENGLES2)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>

#if defined(_WIN32)
#include <Windows.h>
#endif

#elif defined(PLATFORM_UWP)
#include <angle_windowsstore.h>
#include "UWP/GameApp.hpp"
#endif

#include <stdexcept>

#include "Audio/AudioDevice.hpp"
#include "Input/Gamepad.hpp"
#include "Input/Mouse.hpp"
#include "Input/Keyboard.hpp"
#include "Console.hpp"

namespace NerdThings::Ngine {
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
#if defined(GRAPHICS_OPENGL33)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif defined(GRAPHICS_OPENGL21)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#elif defined(GRAPHICS_OPENGLES2)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
                glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
#endif
#endif

        // Creation
#if defined(PLATFORM_DESKTOP)
        // Create window
        m_GLFWWindow = glfwCreateWindow(config_.InitialWidth, config_.InitialHeight, config_.Title.c_str(), nullptr, nullptr);

        // Save title
        m_windowTitle = config_.Title;

        // Save this as a user pointer
        glfwSetWindowUserPointer(m_GLFWWindow, this);

        if (!m_GLFWWindow) {
            glfwTerminate();
            throw std::runtime_error("Failed to create game window.");
        }

        // Get initial size
        glfwGetWindowSize((GLFWwindow *)m_GLFWWindow, &m_currentWidth, &m_currentHeight);
#elif defined(GRAPHICS_OPENGLES2)
        EGLint samples = 0;
        EGLint sampleBuffer = 0;

        if (config_.MSAA_4X) {
#if !defined(PLATFORM_UWP) // Seems to crash UWP, so don't
            samples = 4;
            sampleBuffer = 1;
#endif
        }

        const EGLint framebufferAttribs[] = {
            // Type of context support -> Required on RPI?
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,

            // RED color bit depth (alternative: 5)
            EGL_RED_SIZE, 8,

            // GREEN color bit depth (alternative: 6)
            EGL_GREEN_SIZE, 8,

            // BLUE color bit depth (alternative: 5)
            EGL_BLUE_SIZE, 8,

            // ALPHA bit depth (required for transparent framebuffer)
            EGL_ALPHA_SIZE, 8,

            // Depth buffer size (Required to use Depth testing!)
            EGL_DEPTH_SIZE, 16,

            // Stencil buffer size
            EGL_STENCIL_SIZE, 8,

            // Activate MSAA
            EGL_SAMPLE_BUFFERS, sampleBuffer,

            // 4x Antialiasing if activated (Free on MALI GPUs)
            EGL_SAMPLES, samples,

            // No idea what this does
            EGL_NONE
        };

        const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
        };

#if defined(PLATFORM_UWP)

        const EGLint surfaceAttributes[] = {
            // EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER is part of the same optimization as EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER (see above).
            // If you have compilation issues with it then please update your Visual Studio templates.
            EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_NONE
        };

        const EGLint defaultDisplayAttributes[] = {
            // These are the default display attributes, used to request ANGLE's D3D11 renderer.
            // eglInitialize will only succeed with these attributes if the hardware supports D3D11 Feature Level 10_0+.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,

            // EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER is an optimization that can have large performance benefits on mobile devices.
            // Its syntax is subject to change, though. Please update your Visual Studio templates if you experience compilation issues with it.
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,

            // EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE is an option that enables ANGLE to automatically call
            // the IDXGIDevice3::Trim method on behalf of the application when it gets suspended.
            // Calling IDXGIDevice3::Trim when an application is suspended is a Windows Store application certification requirement.
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        const EGLint fl9_3DisplayAttributes[] = {
            // These can be used to request ANGLE's D3D11 renderer, with D3D11 Feature Level 9_3.
            // These attributes are used if the call to eglInitialize fails with the default display attributes.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE, 9,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE, 3,
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        const EGLint warpDisplayAttributes[] = {
            // These attributes can be used to request D3D11 WARP.
            // They are used if eglInitialize fails with both the default display attributes and the 9_3 display attributes.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_DEVICE_TYPE_WARP_ANGLE,
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        EGLConfig config = nullptr;

        // eglGetPlatformDisplayEXT is an alternative to eglGetDisplay. It allows us to pass in Display attributes, used to configure D3D11.
        PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)(eglGetProcAddress("eglGetPlatformDisplayEXT"));
        if (!eglGetPlatformDisplayEXT) {
            throw std::runtime_error("Failed to get function eglGetPlatformDisplayEXT");
        }

        // To initialize the Display, we make three sets of calls to eglGetPlatformDisplayEXT and eglInitialize, with varying
        // parameters passed to eglGetPlatformDisplayEXT:
        // 1) The first calls uses "defaultDisplayAttributes" as a parameter. This corresponds to D3D11 Feature Level 10_0+.
        // 2) If eglInitialize fails for step 1 (e.g. because 10_0+ isn't supported by the default GPU), then we try again
        //    using "fl9_3DisplayAttributes". This corresponds to D3D11 Feature Level 9_3.
        // 3) If eglInitialize fails for step 2 (e.g. because 9_3+ isn't supported by the default GPU), then we try again
        //    using "warpDisplayAttributes".  This corresponds to D3D11 Feature Level 11_0 on WARP, a D3D11 software rasterizer.

        // This tries to initialize EGL to D3D11 Feature Level 10_0+. See above comment for details.
        m_display = (void*)eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, defaultDisplayAttributes);
        if (m_display == EGL_NO_DISPLAY) {
            throw std::runtime_error("Failed to initialize EGL Display");
        }

        if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
            // This tries to initialize EGL to D3D11 Feature Level 9_3, if 10_0+ is unavailable (e.g. on some mobile devices).
            m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, fl9_3DisplayAttributes);
            if (m_display == EGL_NO_DISPLAY) {
                throw std::runtime_error("Failed to initialize EGL Display");
            }

            if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
                // This initializes EGL to D3D11 Feature Level 11_0 on WARP, if 9_3+ is unavailable on the default GPU.
                m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, warpDisplayAttributes);
                if (m_display == EGL_NO_DISPLAY) {
                    throw std::runtime_error("Failed to initialize EGL Display");
                }

                if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
                    // If all of the calls to eglInitialize returned EGL_FALSE then an error has occurred.
                    throw std::runtime_error("Failed to initialize EGL");
                }
            }
        }

        EGLint numConfigs = 0;
        if ((eglChooseConfig(m_display, framebufferAttribs, &config, 1, &numConfigs) == EGL_FALSE) || (numConfigs == 0)) {
            throw std::runtime_error("Failed to choose first EGLConfig");
        }

#if defined(PLATFORM_UWP)
        // Create a PropertySet and initialize with the EGLNativeWindowType.
        PropertySet^ surfaceCreationProperties = ref new PropertySet();
        surfaceCreationProperties->Insert(ref new String(EGLNativeWindowTypeProperty), CoreWindow::GetForCurrentThread());     // CoreWindow^ window
#endif

        // You can configure the surface to render at a lower resolution and be scaled up to
        // the full window size. The scaling is often free on mobile hardware.
        //
        // One way to configure the SwapChainPanel is to specify precisely which resolution it should render at.
        // Size customRenderSurfaceSize = Size(800, 600);
        // surfaceCreationProperties->Insert(ref new String(EGLRenderSurfaceSizeProperty), PropertyValue::CreateSize(customRenderSurfaceSize));
        //
        // Another way is to tell the SwapChainPanel to render at a certain scale factor compared to its size.
        // e.g. if the SwapChainPanel is 1920x1280 then setting a factor of 0.5f will make the app render at 960x640
        // float customResolutionScale = 0.5f;
        // surfaceCreationProperties->Insert(ref new String(EGLRenderResolutionScaleProperty), PropertyValue::CreateSingle(customResolutionScale));


        // eglCreateWindowSurface() requires a EGLNativeWindowType parameter,
        // In Windows platform: typedef HWND EGLNativeWindowType;


        // Property: EGLNativeWindowTypeProperty
        // Type: IInspectable
        // Description: Set this property to specify the window type to use for creating a surface.
        //              If this property is missing, surface creation will fail.
        //
        //const wchar_t EGLNativeWindowTypeProperty[] = L"EGLNativeWindowTypeProperty";

        // https://stackoverflow.com/questions/46550182/how-to-create-eglsurface-using-c-winrt-and-angle

        // m_surface = eglCreateWindowSurface(m_display, config, reinterpret_cast<IInspectable*>(surfaceCreationProperties), surfaceAttributes);
        m_surface = eglCreateWindowSurface(m_display, config, (EGLNativeWindowType)CoreWindow::GetForCurrentThread(), surfaceAttributes);

        if (m_surface == EGL_NO_SURFACE) {
            throw std::runtime_error("Failed to create EGL fullscreen surface");
        }

        m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
        if (m_context == EGL_NO_CONTEXT) {
            throw std::runtime_error("Failed to create EGL context");
        }

#else
        // For future GLES2 platforms
        EGLint numConfigs;

        // Get an EGL display connection
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_display == EGL_NO_DISPLAY) {
            throw std::runtime_error("Failed to initialize EGL display");
        }

        // Initialize the EGL display connection
        if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
            // If all of the calls to eglInitialize returned EGL_FALSE then an error has occurred.
            throw std::runtime_error("Failed to initialize EGL");
        }

        // Get an appropriate EGL framebuffer configuration
        eglChooseConfig(m_display, framebufferAttribs, &config, 1, &numConfigs);

        // Set rendering API
        eglBindAPI(EGL_OPENGL_ES_API);

        // Create an EGL rendering context
        m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
        if (m_context == EGL_NO_CONTEXT) {
            throw std::runtime_error("Failed to create EGL context");
        }
#endif

        // TODO: For other platforms, initialize window surface

        // Get EGL m_display window size
        eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_currentWidth);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_currentHeight);
#endif

#if defined(PLATFORM_UWP)
        // Get window title (UWP)
        auto pName = Windows::ApplicationModel::Package::Current->DisplayName;
        std::wstring tmp(pName->Begin());
        std::string name(tmp.begin(), tmp.end());
        m_windowTitle = name;
#endif

        Console::Notice("Window", "Successfully created window.");

        // Initialized
        m_initialized = true;

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

    void Window::MakeCurrent() {
        if (!m_initialized) throw std::runtime_error("This window is not ready to be made current!");
#if defined(PLATFORM_DESKTOP)
        // Make this context current
        glfwMakeContextCurrent((GLFWwindow*) m_GLFWWindow);
#elif defined(PLATFORM_UWP)
        // Make egl context current.
        if (eglMakeCurrent(m_display, m_surface, m_surface, m_context) == EGL_FALSE) {
            throw std::runtime_error("Unable to attach EGL rendering context to EGL surface");
        }
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
        eglSwapInterval(m_display, flag_ ? 1 : 0);
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
        eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_currentWidth);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_currentHeight);

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

#if defined(PLATFORM_DESKTOP)
        // Destroy window
        glfwDestroyWindow((GLFWwindow*)m_GLFWWindow);

        // Disable GLFW
        glfwTerminate();
#elif defined(PLATFORM_UWP)
        // Close surface, context and Display
        if (m_display != EGL_NO_DISPLAY) {
            eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

            if (m_surface != EGL_NO_SURFACE) {
                eglDestroySurface(m_display, m_surface);
                m_surface = EGL_NO_SURFACE;
            }

            if (m_context != EGL_NO_CONTEXT) {
                eglDestroyContext(m_display, m_context);
                m_context = EGL_NO_CONTEXT;
            }

            eglTerminate(m_display);
            m_display = EGL_NO_DISPLAY;
        }
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
        glfwSwapBuffers((GLFWwindow *)m_GLFWWindow);
#elif defined(PLATFORM_UWP)
        eglSwapBuffers(m_display, m_surface);
#endif
    }
}
