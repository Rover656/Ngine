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

#include "ngine/graphics/platform/opengl_graphics_device.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#include "ngine/console.hpp"
#include "ngine/window.hpp"

#if !defined(PLATFORM_UWP)
#include <glad/glad.h>
#define GLAD
#endif

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#define GLFW
#endif

#if defined(PLATFORM_UWP)
#define EGL
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#if defined(NGINE_ENABLE_OPENGLES) && !defined(GLAD)
// Include latest GLES header
#include <GLES3/gl31.h>
// Add GLES2 extensions
#include <GLES2/gl2ext.h>
#endif

namespace ngine::graphics::platform {
    OpenGLGraphicsDevice::OpenGLGraphicsDevice(Window *window) : GraphicsDevice(window) {
        // Make window context current
        window->makeCurrent();

        // Setup GLAD
#if defined(GLAD)
        bool status;
        if (window->getContextDescriptor().Type == ContextType::OpenGLES) {
#if defined(GLFW)
            status = gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);
#elif defined(EGL)
            status = gladLoadGLES2Loader((GLADloadproc) eglGetProcAddress);
#endif
        } else {
#if defined(GLFW)
            status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
#endif
        }

        // If we didn't load, terminate
        if (!status)
            Console::fail("OpenGLGraphicsDevice", "Failed to init GLAD.");
#endif

        // Broadcast GL version
        Console::notice("OpenGLGraphicsDevice", "Successfully opened graphics device with OpenGL version: %s", glGetString(GL_VERSION));

        // Determine if we're running GLES
#if !defined(GLAD)
        if (window->getContextDescriptor().Type == ContextType::OpenGLES) {
            auto major = window->getContextDescriptor().MajorVersion;
            m_isGLES2 = major == 2;
            m_isGLES3 = major == 3;
        }
#else
        m_isGLES2 = GLAD_GL_ES_VERSION_2_0 && !GLAD_GL_ES_VERSION_3_0 &&
                  !GLAD_GL_ES_VERSION_3_1 && !GLAD_GL_ES_VERSION_3_2;
        m_isGLES3 = GLAD_GL_ES_VERSION_3_0;
#endif

        // TODO: Load capabilities/extensions
    }

    OpenGLGraphicsDevice::~OpenGLGraphicsDevice() {
        // TODO: Free any resources.
    }
}

#endif // defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)