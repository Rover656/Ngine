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

#include "gl_context.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#include "ngine/console.hpp"

#if defined(GLAD)
#include <glad/glad.h>
#endif

#if defined(GLFW)
#include <GLFW/glfw3.h>
#endif

#if defined(EGL)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#if defined(NGINE_ENABLE_OPENGLES) && !defined(GLAD)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1

 // Include latest GLES header
#include <GLES3/gl31.h>
// Add GLES2 extensions
#include <GLES2/gl2ext.h>
#endif

#if defined(PLATFORM_UWP)
#include "ngine/platform/UWP/uwp_bootstrap.hpp"
#include <angle_windowsstore.h>
#endif

namespace ngine::platform::graphics::gl {
    GLContext::GLContext(IWindow *window) : m_window(window) {
        // Init EGL
#if defined(EGL)
        // Init EGL
        EGLint samples = 0;
        EGLint sampleBuffer = 0;

        // Enable MSAA x4 if enabled.
        /*if (graphicsDevice->getWindow()->getCreationConfig().MSAA_4X) {
#if !defined(PLATFORM_UWP) // Seems to crash UWP, so don't
            samples = 4;
            sampleBuffer = 1;
#endif
        }*/

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
            EGL_NONE };

        // Setup context version
        auto contextDescriptor = m_window->getContextDescriptor();
        const EGLint contextAttribs[] = {EGL_CONTEXT_MAJOR_VERSION, contextDescriptor.MajorVersion,
                                         EGL_CONTEXT_MINOR_VERSION, contextDescriptor.MinorVersion,
                                         EGL_NONE};

#if defined(PLATFORM_UWP)
        const EGLint surfaceAttributes[] = {
            // EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER is part of the same
            // optimization as EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER
            // (see above).
            // If you have compilation issues with it then please update your
            // Visual Studio templates.
            EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER, EGL_TRUE, EGL_NONE };

        const EGLint defaultDisplayAttributes[] = {
            // These are the default display attributes, used to request ANGLE's
            // D3D11 renderer.
            // eglInitialize will only succeed with these attributes if the
            // hardware supports D3D11 Feature Level 10_0+.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE,
            EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,

            // EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER is an optimization
            // that can have large performance benefits on mobile devices.
            // Its syntax is subject to change, though. Please update your
            // Visual Studio templates if you experience compilation issues with
            // it.
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER,
            EGL_TRUE,

            // EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE is an option that
            // enables ANGLE to automatically call
            // the IDXGIDevice3::Trim method on behalf of the application when
            // it gets suspended.
            // Calling IDXGIDevice3::Trim when an application is suspended is a
            // Windows Store application certification requirement.
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE,
            EGL_TRUE,
            EGL_NONE,
        };

        const EGLint fl9_3DisplayAttributes[] = {
            // These can be used to request ANGLE's D3D11 renderer, with D3D11
            // Feature Level 9_3.
            // These attributes are used if the call to eglInitialize fails with
            // the default display attributes.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE,
            EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE,
            9,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE,
            3,
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER,
            EGL_TRUE,
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE,
            EGL_TRUE,
            EGL_NONE,
        };

        const EGLint warpDisplayAttributes[] = {
            // These attributes can be used to request D3D11 WARP.
            // They are used if eglInitialize fails with both the default
            // display attributes and the 9_3 display attributes.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE,
            EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE,
            EGL_PLATFORM_ANGLE_DEVICE_TYPE_WARP_ANGLE,
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER,
            EGL_TRUE,
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE,
            EGL_TRUE,
            EGL_NONE,
        };

        EGLConfig config = nullptr;

        // eglGetPlatformDisplayEXT is an alternative to eglGetDisplay. It
        // allows us to pass in Display attributes, used to configure D3D11.
        PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT =
            (PFNEGLGETPLATFORMDISPLAYEXTPROC)(
                eglGetProcAddress("eglGetPlatformDisplayEXT"));
        if (!eglGetPlatformDisplayEXT) {
            Console::fail("PlatformGLAPI",
                "Failed to get function eglGetPlatformDisplayEXT");
        }

        // To initialize the Display, we make three sets of calls to
        // eglGetPlatformDisplayEXT and eglInitialize, with varying parameters
        // passed to eglGetPlatformDisplayEXT: 1) The first calls uses
        // "defaultDisplayAttributes" as a parameter. This corresponds to D3D11
        // Feature Level 10_0+. 2) If eglInitialize fails for step 1 (e.g.
        // because 10_0+ isn't supported by the default GPU), then we try again
        //    using "fl9_3DisplayAttributes". This corresponds to D3D11 Feature
        //    Level 9_3.
        // 3) If eglInitialize fails for step 2 (e.g. because 9_3+ isn't
        // supported by the default GPU), then we try again
        //    using "warpDisplayAttributes".  This corresponds to D3D11 Feature
        //    Level 11_0 on WARP, a D3D11 software rasterizer.

        // This tries to initialize EGL to D3D11 Feature Level 10_0+. See above
        // comment for details.
        m_display = (void*)eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE,
            EGL_DEFAULT_DISPLAY,
            defaultDisplayAttributes);
        if (m_display == EGL_NO_DISPLAY) {
            Console::fail("PlatformGLAPI", "Failed to initialize EGL Display");
        }

        if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
            // This tries to initialize EGL to D3D11 Feature Level 9_3, if 10_0+
            // is unavailable (e.g. on some mobile devices).
            m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE,
                EGL_DEFAULT_DISPLAY,
                fl9_3DisplayAttributes);
            if (m_display == EGL_NO_DISPLAY) {
                Console::fail("PlatformGLAPI",
                    "Failed to initialize EGL Display");
            }

            if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
                // This initializes EGL to D3D11 Feature Level 11_0 on WARP, if
                // 9_3+ is unavailable on the default GPU.
                m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE,
                    EGL_DEFAULT_DISPLAY,
                    warpDisplayAttributes);
                if (m_display == EGL_NO_DISPLAY) {
                    Console::fail("PlatformGLAPI",
                        "Failed to initialize EGL Display");
                }

                if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
                    // If all of the calls to eglInitialize returned EGL_FALSE
                    // then an error has occurred.
                    Console::fail("PlatformGLAPI", "Failed to initialize EGL");
                }
            }
        }

        EGLint numConfigs = 0;
        if ((eglChooseConfig(m_display, framebufferAttribs, &config, 1,
            &numConfigs) == EGL_FALSE) ||
            (numConfigs == 0)) {
            Console::fail("PlatformGLAPI", "Failed to choose first EGLConfig");
        }

        // Create a PropertySet and initialize with the EGLNativeWindowType.
        PropertySet^ surfaceCreationProperties = ref new PropertySet();
        surfaceCreationProperties->Insert(ref new String(EGLNativeWindowTypeProperty), CoreWindow::GetForCurrentThread());

        // Create surface
        m_surface = eglCreateWindowSurface(
            m_display, config,
            (EGLNativeWindowType) CoreWindow::GetForCurrentThread(),
            surfaceAttributes);

        if (m_surface == EGL_NO_SURFACE) {
            Console::fail("PlatformGLAPI",
                "Failed to create EGL fullscreen surface");
        }

        m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
        if (m_context == EGL_NO_CONTEXT) {
            Console::fail("PlatformGLAPI", "Failed to create EGL context.");
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
            // If all of the calls to eglInitialize returned EGL_FALSE then an
            // error has occurred.
            throw std::runtime_error("Failed to initialize EGL");
        }

        // Get an appropriate EGL framebuffer configuration
        eglChooseConfig(m_display, framebufferAttribs, &config, 1, &numConfigs);

        // Set rendering API
        eglBindAPI(EGL_OPENGL_ES_API);

        // Create an EGL rendering context
        m_context =
            eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
        if (m_context == EGL_NO_CONTEXT) {
            throw std::runtime_error("Failed to create EGL context");
        }

        // DEV: For other EGL platforms, initialize window surface
#endif
#endif

        // Make this context current. However, do not assume this context will be used on the thread we are created on!
        makeCurrent();

        // Init GLAD if it hasn't been
        if (!m_gladInit) {
#if defined(GLAD)
            bool status;
            if (window->getContextDescriptor().Type == ContextType::OpenGLES) {
#if defined(GLFW)
                status = gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);
#elif defined(EGL)
                status = gladLoadGLES2Loader((GLADloadproc) eglGetProcAddress);
#endif // defined(EGL)
            } else {
#if defined(GLFW)
                status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
#endif // defined(GLFW)
            }

            // If we didn't load, terminate
            if (!status)
                Console::fail("OpenGLContext", "Failed to init GLAD.");
#endif // defined(GLAD)
            m_gladInit = true;
            m_gladInitContextType = window->getContextDescriptor().Type;
        } else if (m_gladInitContextType != window->getContextDescriptor().Type) {
            Console::fail("OpenGLContext", "Cannot use two different OpenGL context types in the same program!");
        }

        // Broadcast GL version
        Console::notice("OpenGL", "Successfully opened graphics device with OpenGL version: %s", glGetString(GL_VERSION));
    }

    GLContext::~GLContext() {
#if defined(EGL)
        // Close surface, context and Display
        if (m_display != EGL_NO_DISPLAY) {
            eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                EGL_NO_CONTEXT);

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
    }

    void GLContext::makeCurrent() {
#if defined(GLFW)
        glfwMakeContextCurrent((GLFWwindow *) m_window->getHandle());
#elif defined(EGL)
        eglMakeCurrent(m_display, m_surface, m_surface, m_context);
#endif
    }

    void GLContext::swapBuffers() {
#if defined(GLFW)
        glfwSwapBuffers((GLFWwindow *) m_window->getHandle());
#elif defined(EGL)
        eglSwapBuffers(m_display, m_surface);
#endif
    }

    bool GLContext::m_gladInit = false;
    ContextType GLContext::m_gladInitContextType;
}

#endif