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
#endif

#include <stdexcept>

#include "Graphics/OpenGL/OpenGL.h"

namespace NerdThings::Ngine {
    // Private Fields

    int Window::_Height = 0;
    int Window::_Width = 0;
    void *Window::_WindowPtr = nullptr;

#if defined(PLATFORM_UWP)
    Windows::UI::Core::CoreWindow ^Window::UWPWindow;
#endif

    // Private Methods


#if defined(PLATFORM_DESKTOP)
    void Window::UpdateWindowSize(void *window_, int width_, int height_) {
        _Width = width_;
        _Height = height_;
#elif defined(PLATFORM_UWP)
    void Window::UpdateWindowSize(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args) {
        eglQuerySurface(display, surface, EGL_WIDTH, &_Width);
        eglQuerySurface(display, surface, EGL_HEIGHT, &_Height);
#endif
    }

    void Window::Cleanup() {
#if defined(PLATFORM_DESKTOP)
        glfwDestroyWindow((GLFWwindow*)_WindowPtr);
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
    }

    int Window::GetHeight() {
        return _Height;
    }

    int Window::GetWidth() {
        return _Width;
    }

    void Window::Init(int width_, int height_, std::string title_) {
// Init
#if defined(PLATFORM_DESKTOP)
        if (!glfwInit())
            throw std::runtime_error("[Window::Init] Failed to init GLFW.");
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
        }
#endif

        // Creation
#if defined(PLATFORM_DESKTOP)
        // Create window
        _WindowPtr = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
        if (!_WindowPtr) {
            glfwTerminate();
            throw std::runtime_error("[Window::Window] Failed to create.");
        }

        // Set resize callback
        glfwSetWindowSizeCallback((GLFWwindow *)_WindowPtr, (void(*)(GLFWwindow*, int, int))Window::UpdateWindowSize);

        // Get initial size
        glfwGetWindowSize((GLFWwindow *)_WindowPtr, &_Width, &_Height);

        // Use new context
        glfwMakeContextCurrent((GLFWwindow*) _WindowPtr);
#elif defined(PLATFORM_UWP)
        EGLint samples = 0;
        EGLint sampleBuffer = 0;
        //if (configFlags & FLAG_MSAA_4X_HINT)
        //{
        //    samples = 4;
        //    sampleBuffer = 1;
        //}

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
        surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)UWPWindow, surfaceAttributes);
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
#endif
    }

    void Window::PollEvents() {
#if defined(PLATFORM_DESKTOP)
        // Poll window events
        glfwPollEvents();
#endif
    }

    void Window::SetResizable(bool resizable_) {
#if defined(PLATFORM_DESKTOP)
        glfwSetWindowAttrib((GLFWwindow *)_WindowPtr, GLFW_RESIZABLE, resizable_ ? 1 : 0);
#endif
    }

    void Window::SetTitle(std::string title_) {
#if defined(PLATFORM_DESKTOP)
        glfwSetWindowTitle((GLFWwindow *)_WindowPtr, title_.c_str());
#endif
    }

    bool Window::ShouldClose() {
#if defined(PLATFORM_DESKTOP)
        return glfwWindowShouldClose((GLFWwindow *)_WindowPtr);
#elif defined(PLATFORM_UWP)
        return false; // TODO: Get from a window callback
#endif
        return true;
    }

    void Window::SwapBuffers() {
#if defined(PLATFORM_DESKTOP)
        glfwSwapBuffers((GLFWwindow *)_WindowPtr);
#elif defined(PLATFORM_UWP)
        eglSwapBuffers(display, surface);
#endif
    }
}