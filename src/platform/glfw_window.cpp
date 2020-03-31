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

#include "glfw_window.hpp"

#if defined(PLATFORM_DESKTOP)

#include "ngine/console.hpp"

namespace ngine::platform {
    GLFWWindow::GLFWWindow(WindowConfig config) : IWindow(config) {
        // Verify context descriptor
        _verifyContextDescriptor();

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
                // Use core context as we only support 3.3 or later and these were added in 3.2
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_contextDescriptor.MajorVersion);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_contextDescriptor.MinorVersion);
                break;
            case graphics::ContextType::OpenGLES:
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
            default:
                Console::fail("GLFWWindow", "Unrecognized context type!");
                break;
        }

        // Set error callback
        glfwSetErrorCallback(
                [](int, const char *msg) { Console::error("GLFW", "%s", msg); });

        // Create window
        m_window = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(), nullptr, nullptr);

        // Check window was created
        if (!m_window) {
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
        glfwGetWindowSize((GLFWwindow *) m_window, &m_windowWidth, &m_windowHeight);

        // Create graphics device
        _createGraphicsDevice();
    }

    GLFWWindow::~GLFWWindow() {
        // Destroy
        glfwDestroyWindow((GLFWwindow *) m_window);
        m_windowCount--;
        if (m_windowCount <= 0) {
            Console::notice("GLFWWindow", "Last window closed, terminating GLFW.");
            glfwTerminate();
        }
    }

    void GLFWWindow::pollEvents() {
        // Poll window events
        glfwPollEvents();

        // Collect window size.
        auto w = m_windowWidth;
        auto h = m_windowHeight;
        glfwGetWindowSize((GLFWwindow *) m_window, &m_windowWidth, &m_windowHeight);

        // Tell graphics device to resize if we did.
        _checkResized(w, h);
    }

    bool GLFWWindow::shouldClose() const {
        return glfwWindowShouldClose((GLFWwindow *) m_window);
    }

    void *GLFWWindow::getHandle() const {
        return m_window;
    }
}

#endif // defined(PLATFORM_DESKTOP)
