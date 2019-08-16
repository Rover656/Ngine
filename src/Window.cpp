#include "Window.h"

// Platform specifics
#if defined(GRAPHICS_OPENGL33)
    #include <glad/glad.h>
#elif defined(GRAPHICS_OPENGLES2)
    #include <EGL/egl.h>
    #include <EGL/eglext.h>

    // Defined here so it doesn't appear in Window.h
    static EGLDisplay display; // Native display device (physical screen connection)
    static EGLSurface surface; // Surface to draw on, framebuffers (connected to context)
    static EGLContext context; // Graphic context, mode in which drawing can be done
    static EGLConfig config; // Graphic config
#endif

#if defined(PLATFORM_DESKTOP)
    #include <GLFW/glfw3.h>
#elif defined(PLATFORM_UWP)
#include <angle_windowsstore.h>
#include "Platform/UWP/GameApp.h"
#endif

#include <stdexcept>

#include "Graphics/OpenGL/OpenGL.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"

namespace NerdThings::Ngine {
    // Private Fields

    int Window::_Height = 0;
    int Window::_Width = 0;

    // Public Fields

    void *Window::WindowPtr = nullptr;

#if defined(PLATFORM_UWP)
    UWP::GameApp ^Window::UWPApp;
#endif

    // Private Methods
#if defined(PLATFORM_UWP)
    void Window::Suspended(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args) {
        /*
         * According to the UWP Lifecycle, this is called when the app is placed into the background.
         * I reckon that because we have a constant loop running, this will only be called when the game is closed.
         * This is used because the Run thread is immediately killed, it is not allowed to run to the end.
         * Resuming event should not be required because of this, so we can assume that the application is closing.
         */

        // Delete loaded resources
        Resources::DeleteAll();

        // Close audio
        ConsoleMessage("Closing audio device.", "NOTICE", "WINDOW");
        Audio::AudioManager::CloseDevice();

        // Close window
        ConsoleMessage("Closing window.", "NOTICE", "WINDOW");
        Window::Cleanup();
    }
#endif

    void Window::Cleanup() {
#if defined(PLATFORM_DESKTOP)
        // Destroy window
        glfwDestroyWindow((GLFWwindow*)WindowPtr);
        glfwTerminate();
#elif defined(PLATFORM_UWP)
        // Close surface, context and display
        if (display != EGL_NO_DISPLAY) {
            eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

            if (surface != EGL_NO_SURFACE) {
                eglDestroySurface(display, surface);
                surface = EGL_NO_SURFACE;
            }

            if (context != EGL_NO_CONTEXT) {
                eglDestroyContext(display, context);
                context = EGL_NO_CONTEXT;
            }

            eglTerminate(display);
            display = EGL_NO_DISPLAY;
        }
#endif
        // Unset width and height as the window is closed
        _Width = 0;
        _Height = 0;
    }

    int Window::GetHeight() {
        return _Height;
    }

    int Window::GetWidth() {
        return _Width;
    }

    void Window::Init(int width_, int height_, const std::string& title_) {
        // Init
#if defined(PLATFORM_DESKTOP)
        // Init GLFW
        if (!glfwInit()) {
            ConsoleMessage("Failed to init GLFW.", "ERROR", "WINDOW");
            throw std::runtime_error("[Window::Init] Failed to init GLFW.");
        }
        glfwDefaultWindowHints();

        switch(Graphics::OpenGL::GL::GetGLVersion()) {
            case Graphics::OpenGL::OPENGL_33:
                glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                break;
            case Graphics::OpenGL::OPENGL_ES2:
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
                glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
                break;
            case Graphics::OpenGL::OPENGL_UNKNOWN:
                throw std::runtime_error("Error, we were unable to determine an OpenGL version to use.");
        }
#endif

        // Creation
#if defined(PLATFORM_DESKTOP)
        // Create window
        WindowPtr = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
        if (!WindowPtr) {
            glfwTerminate();
            throw std::runtime_error("[Window::Window] Failed to create.");
        }

        // Get initial size
        glfwGetWindowSize((GLFWwindow *)WindowPtr, &_Width, &_Height);

        // Use new context
        glfwMakeContextCurrent((GLFWwindow*) WindowPtr);
#elif defined(PLATFORM_UWP)
        EGLint samples = 0;
        EGLint sampleBuffer = 0;
        // TODO: MSAA support
        //if (configFlags & FLAG_MSAA_4X_HINT)
        //{
        //    samples = 4;
        //    sampleBuffer = 1;
        //}

        // TODO: Review all of this that came from raylib to determine if this is all up to scratch
        const EGLint framebufferAttribs[] =
        {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,     // Type of context support -> Required on RPI?
            //EGL_SURFACE_TYPE, EGL_WINDOW_BIT,          // Don't use it on Android!
            EGL_RED_SIZE, 8,            // RED color bit depth (alternative: 5)
            EGL_GREEN_SIZE, 8,          // GREEN color bit depth (alternative: 6)
            EGL_BLUE_SIZE, 8,           // BLUE color bit depth (alternative: 5)
            //EGL_ALPHA_SIZE, 8,        // ALPHA bit depth (required for transparent framebuffer)
            //EGL_TRANSPARENT_TYPE, EGL_NONE, // Request transparent framebuffer (EGL_TRANSPARENT_RGB does not work on RPI)
            EGL_DEPTH_SIZE, 16,         // Depth buffer size (Required to use Depth testing!)
            //EGL_STENCIL_SIZE, 8,      // Stencil buffer size
            EGL_SAMPLE_BUFFERS, sampleBuffer,    // Activate MSAA
            EGL_SAMPLES, samples,       // 4x Antialiasing if activated (Free on MALI GPUs)
            EGL_NONE
        };

        const EGLint contextAttribs[] =
        {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
        };

        const EGLint surfaceAttributes[] =
        {
            // EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER is part of the same optimization as EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER (see above).
            // If you have compilation issues with it then please update your Visual Studio templates.
            EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_NONE
        };

        const EGLint defaultDisplayAttributes[] =
        {
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

        const EGLint fl9_3DisplayAttributes[] =
        {
            // These can be used to request ANGLE's D3D11 renderer, with D3D11 Feature Level 9_3.
            // These attributes are used if the call to eglInitialize fails with the default display attributes.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE, 9,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE, 3,
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        const EGLint warpDisplayAttributes[] =
        {
            // These attributes can be used to request D3D11 WARP.
            // They are used if eglInitialize fails with both the default display attributes and the 9_3 display attributes.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_DEVICE_TYPE_WARP_ANGLE,
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        EGLConfig config = NULL;

        // eglGetPlatformDisplayEXT is an alternative to eglGetDisplay. It allows us to pass in display attributes, used to configure D3D11.
        PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)(eglGetProcAddress("eglGetPlatformDisplayEXT"));
        if (!eglGetPlatformDisplayEXT)
        {
            throw std::runtime_error("Failed to get function eglGetPlatformDisplayEXT");
        }

        //
        // To initialize the display, we make three sets of calls to eglGetPlatformDisplayEXT and eglInitialize, with varying
        // parameters passed to eglGetPlatformDisplayEXT:
        // 1) The first calls uses "defaultDisplayAttributes" as a parameter. This corresponds to D3D11 Feature Level 10_0+.
        // 2) If eglInitialize fails for step 1 (e.g. because 10_0+ isn't supported by the default GPU), then we try again
        //    using "fl9_3DisplayAttributes". This corresponds to D3D11 Feature Level 9_3.
        // 3) If eglInitialize fails for step 2 (e.g. because 9_3+ isn't supported by the default GPU), then we try again
        //    using "warpDisplayAttributes".  This corresponds to D3D11 Feature Level 11_0 on WARP, a D3D11 software rasterizer.
        //

        // This tries to initialize EGL to D3D11 Feature Level 10_0+. See above comment for details.
        display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, defaultDisplayAttributes);
        if (display == EGL_NO_DISPLAY)
        {
            throw std::runtime_error("Failed to initialize EGL display");
        }

        if (eglInitialize(display, NULL, NULL) == EGL_FALSE)
        {
            // This tries to initialize EGL to D3D11 Feature Level 9_3, if 10_0+ is unavailable (e.g. on some mobile devices).
            display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, fl9_3DisplayAttributes);
            if (display == EGL_NO_DISPLAY)
            {
                throw std::runtime_error("Failed to initialize EGL display");
            }

            if (eglInitialize(display, NULL, NULL) == EGL_FALSE)
            {
                // This initializes EGL to D3D11 Feature Level 11_0 on WARP, if 9_3+ is unavailable on the default GPU.
                display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, warpDisplayAttributes);
                if (display == EGL_NO_DISPLAY)
                {
                    throw std::runtime_error("Failed to initialize EGL display");
                }

                if (eglInitialize(display, NULL, NULL) == EGL_FALSE)
                {
                    // If all of the calls to eglInitialize returned EGL_FALSE then an error has occurred.
                    throw std::runtime_error("Failed to initialize EGL");
                }
            }
        }

        EGLint numConfigs = 0;
        if ((eglChooseConfig(display, framebufferAttribs, &config, 1, &numConfigs) == EGL_FALSE) || (numConfigs == 0))
        {
            throw std::runtime_error("Failed to choose first EGLConfig");
        }

        // Create a PropertySet and initialize with the EGLNativeWindowType.
        //PropertySet^ surfaceCreationProperties = ref new PropertySet();
        //surfaceCreationProperties->Insert(ref new String(EGLNativeWindowTypeProperty), window);     // CoreWindow^ window

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

        //https://stackoverflow.com/questions/46550182/how-to-create-eglsurface-using-c-winrt-and-angle

        //surface = eglCreateWindowSurface(display, config, reinterpret_cast<IInspectable*>(surfaceCreationProperties), surfaceAttributes);
        surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)UWPApp->GetWindow(), surfaceAttributes);
        if (surface == EGL_NO_SURFACE)
        {
            throw std::runtime_error("Failed to create EGL fullscreen surface");
        }

        context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
        if (context == EGL_NO_CONTEXT)
        {
            throw std::runtime_error("Failed to create EGL context");
        }

        // Get EGL display window size
        eglQuerySurface(display, surface, EGL_WIDTH, &_Width);
        eglQuerySurface(display, surface, EGL_HEIGHT, &_Height);

        if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
        {
            throw std::runtime_error("Unable to attach EGL rendering context to EGL surface");
        }

        // UWP on suspend. This handles resource cleanup
        CoreApplication::Suspending += ref new Windows::Foundation::EventHandler<Windows::ApplicationModel::SuspendingEventArgs ^>(&Window::Suspended);
#endif
        // Window is now created, init OpenGL
        Graphics::OpenGL::GL::Init();
        ConsoleMessage("The OpenGL API has been initialized.", "NOTICE", "WINDOW");

        // Init Input
        Input::Mouse::Init();
        Input::Keyboard::Init();
        ConsoleMessage("Input API's have been initialized.", "NOTICE", "Window");
    }

    void Window::PollEvents() {
#if defined(PLATFORM_DESKTOP)
        // Poll window events
        glfwPollEvents();

        // Query dimensions
        glfwGetWindowSize((GLFWwindow *)WindowPtr, &_Width, &_Height);
#elif defined(PLATFORM_UWP)
        // Poll window events
        if (ShouldRenderFrame())
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
        else
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);

        // Query dimensions
        eglQuerySurface(display, surface, EGL_WIDTH, &_Width);
        eglQuerySurface(display, surface, EGL_HEIGHT, &_Height);
#endif
    }

    void Window::SetResizable(bool resizable_) {
#if defined(PLATFORM_DESKTOP)
        glfwSetWindowAttrib((GLFWwindow *)WindowPtr, GLFW_RESIZABLE, resizable_ ? 1 : 0);
#endif
    }

    void Window::SetTitle(const std::string& title_) {
#if defined(PLATFORM_DESKTOP)
        glfwSetWindowTitle((GLFWwindow *)WindowPtr, title_.c_str());
#endif
    }

    bool Window::ShouldClose() {
#if defined(PLATFORM_DESKTOP)
        return glfwWindowShouldClose((GLFWwindow *)WindowPtr);
#elif defined(PLATFORM_UWP)
        return false; // This is not used for anything anyway.
#endif
        return true;
    }

    bool Window::ShouldRenderFrame()
    {
#if defined(PLATFORM_UWP)
        return UWPApp->GetWindow()->Visible == true;
#endif
        return true;
    }

    void Window::SwapBuffers() {
#if defined(PLATFORM_DESKTOP)
        glfwSwapBuffers((GLFWwindow *)WindowPtr);
#elif defined(PLATFORM_UWP)
        eglSwapBuffers(display, surface);
#endif
    }
}