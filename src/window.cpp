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

#include "ngine/window.hpp"

// TODO: Init checks

#include "ngine/console.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)
#include "ngine/graphics/platform/opengl_graphics_device.hpp"
#endif

#if defined(NGINE_ENABLE_DIRECTX)
#include "ngine/graphics/platform/directx_graphics_device.hpp"
#endif

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#endif

#if defined(PLATFORM_UWP)
#include "ngine/platform/UWP/uwp_bootstrap.hpp"
#endif

namespace ngine {
    int Window::m_windowCount = 0;

    Window::Window(WindowConfig config)
            : m_contextDescriptor(config.ContextDescriptor) {
        // Verify context descriptor
        if (!m_contextDescriptor.verify()) {
            Console::fail("Window", "Invalid context descriptor.");
        }

#if defined(PLATFORM_DESKTOP)
        // Init GLFW (if required)
        if (m_windowCount <= 0) {
            if (!glfwInit()) {
                Console::fail("Window", "Failed to initialize GLFW.");
            }
        }

        // Setup window hints
        glfwDefaultWindowHints();

        // Setup context
        switch (m_contextDescriptor.Type) {
            case graphics::ContextType::OpenGL:
                if (m_contextDescriptor.MajorVersion > 3 ||
                    (m_contextDescriptor.MajorVersion == 3 && m_contextDescriptor.MinorVersion > 2))
                    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_contextDescriptor.MajorVersion);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_contextDescriptor.MinorVersion);
                break;
            case graphics::ContextType::OpenGLES:
                // TODO Finish support
                Console::fail("Window", "OpenGL ES is not implemented.");

                // Setup for GLES
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
                glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_contextDescriptor.MajorVersion);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_contextDescriptor.MinorVersion);
                break;
            case graphics::ContextType::DirectX:
                // Ask GLFW to not init OGL.
                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
                break;
            case graphics::ContextType::Vulkan:
                // TODO One day
                Console::fail("Window", "Vulkan is not implemented.");
                break;
        }

        // Set error callback
        glfwSetErrorCallback(
                [](int, const char *msg) { Console::error("GLFW", "%s", msg); });

        // Create window
        m_handle = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(), nullptr, nullptr);

        // Check window was created
        if (!m_handle) {
            if (m_windowCount <= 0) {
                Console::notice("Window", "Last window closed, terminating GLFW.");
                glfwTerminate();
            }
            Console::fail("Window", "Failed to create GLFW window.");
        }
        Console::notice("Window", "Successfully created window.");

        // Gather remaining information
        m_windowCount++;
        m_title = config.Title;
        glfwGetWindowSize((GLFWwindow*)m_handle, &m_windowWidth, &m_windowHeight);
#elif defined(PLATFORM_UWP)
        auto bounds = CoreWindow::GetForCurrentThread()->Bounds;
        m_windowWidth = bounds.Width;
        m_windowHeight = bounds.Height;
#endif

        // Finish setup
        m_initialized = true;

        // Create graphics device
        switch (m_contextDescriptor.Type) {
            case graphics::ContextType::OpenGL:
#if defined(NGINE_ENABLE_OPENGL)
                m_graphicsDevice = new graphics::platform::OpenGLGraphicsDevice(this);
#else
                Console::fail("Window", "Cannot create OpenGL graphics device, OpenGL is not enabled.");
#endif
                break;
            case graphics::ContextType::OpenGLES:
                // TODO: GLES is not fully implemented
                Console::fail("Window", "OpenGL ES is not implemented.");
#if defined(NGINE_ENABLE_OPENGLES) && FALSE
                m_graphicsDevice = new graphics::platform::OpenGLGraphicsDevice(this);
#else
                Console::fail("Window", "Cannot create OpenGLES graphics device, OpenGL is not enabled.");
#endif
                break;
            case graphics::ContextType::DirectX:
                m_graphicsDevice = new graphics::platform::DirectXGraphicsDevice(this);
                break;
            case graphics::ContextType::Vulkan:
                Console::fail("Window", "Vulkan is not implemented.");
                break;
        }

        // TODO: Create viewport.
        // TODO: Create input APIs.
    }

    Window::~Window() {
        // Delete
        m_initialized = false;
        delete m_graphicsDevice;

#if defined(PLATFORM_DESKTOP)
        // Destroy
        glfwDestroyWindow((GLFWwindow *) m_handle);
        m_windowCount--;
        if (m_windowCount <= 0) {
            Console::notice("Window", "Last window closed, terminating GLFW.");
            glfwTerminate();
        }
#endif
        Console::notice("Window", "Successfully closed window.");
    }

    const graphics::ContextDescriptor Window::getContextDescriptor() const {
        return m_contextDescriptor;
    }

    graphics::GraphicsDevice *Window::getGraphicsDevice() {
        return m_graphicsDevice;
    }

    const graphics::GraphicsDevice *Window::getGraphicsDevice() const {
        return m_graphicsDevice;
    }

    float Window::getWidth() const {
        return m_windowWidth;
    }

    float Window::getHeight() const {
        return m_windowHeight;
    }

    void Window::pollEvents() {
        if (!m_initialized)
            Console::fail("Window", "Window not initialized.");
#if defined(PLATFORM_DESKTOP)
        // Poll window events
        glfwPollEvents();

        // Collect window size.
        auto w = m_windowWidth;
        auto h = m_windowHeight;
        glfwGetWindowSize((GLFWwindow *) m_handle, &m_windowWidth, &m_windowHeight);

        // Tell graphics device to resize if we did.
        if (m_windowWidth != w || m_windowHeight != h)
            m_graphicsDevice->_onResize();
#elif defined(PLATFORM_UWP)
        // Poll window events
        if (CoreWindow::GetForCurrentThread()->Visible == true) // TODO: Visible field
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(
                CoreProcessEventsOption::ProcessAllIfPresent);
        else
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(
                CoreProcessEventsOption::ProcessOneAndAllPending);

        auto bounds = CoreWindow::GetForCurrentThread()->Bounds;
        m_windowWidth = bounds.Width;
        m_windowHeight = bounds.Height;
#endif
    }

    bool Window::pendingClose() {
        if (!m_initialized)
            Console::fail("Window", "Window not initialized.");
#if defined(PLATFORM_DESKTOP)
        return glfwWindowShouldClose((GLFWwindow *) m_handle);
#endif
        return false;
    }

    void Window::makeCurrent() {
        if (!m_initialized)
            Console::fail("Window", "Window not initialized.");
#if defined(PLATFORM_DESKTOP)
        glfwMakeContextCurrent((GLFWwindow *) m_handle);
#endif
    }

    void *Window::getHandle() const {
        return m_handle;
    }
}