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

#include "ngine/graphics/platform/opengl_context.hpp"

#if defined(NGINE_ENABLE_OPENGL)// || defined(NGINE_ENABLE_OPENGLES)

#include "ngine/console.hpp"

#if !defined(PLATFORM_UWP)
#include <glad/glad.h>
#define GLAD
#endif

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#define GLFW
#endif

namespace ngine::graphics::platform {
    OpenGLContext::OpenGLContext(Window *window) : m_window(window) {
        // TODO: EGL context creation

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
        }

        // Broadcast GL version
        Console::notice("OpenGL", "Successfully opened graphics device with OpenGL version: %s", glGetString(GL_VERSION));
    }

    OpenGLContext::~OpenGLContext() {}

    void OpenGLContext::swapBuffers() {
#if defined(GLFW)
        glfwSwapBuffers((GLFWwindow *) m_window->getHandle());
#endif
    }

    bool OpenGLContext::m_gladInit = false;
}

#endif