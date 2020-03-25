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
    void OpenGLGraphicsDevice::clear(Color color) {
        glClearColor(color.R, color.G, color.B, color.A);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLGraphicsDevice::bindBuffer(BufferType type, Buffer *buffer) {
        switch(type) {
            case BufferType::Vertex:
                glBindBuffer(GL_ARRAY_BUFFER, buffer ? buffer->GLID : 0);
                break;
            case BufferType::Index:
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer ? buffer->GLID : 0);
                break;
        }
    }

    void OpenGLGraphicsDevice::drawPrimitives(PrimitiveType primitiveType, int start, int count) {
        GLenum prim;
        switch (primitiveType) {
            case PrimitiveType::Triangles:
                prim = GL_TRIANGLES;
                break;
        }
        glDrawArrays(prim, start, count);
    }

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
            Console::fail("OpenGL", "Failed to init GLAD.");
#endif

        // Broadcast GL version
        Console::notice("OpenGL", "Successfully opened graphics device with OpenGL version: %s", glGetString(GL_VERSION));

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

    void OpenGLGraphicsDevice::_initBuffer(Buffer *buffer) {
        buffer->GLID = 0;
        glGenBuffers(1, &buffer->GLID);
    }

    void OpenGLGraphicsDevice::_freeBuffer(Buffer *buffer) {
        glDeleteBuffers(1, &buffer->GLID);
        buffer->GLID = 0;
    }

    void OpenGLGraphicsDevice::_writeBuffer(Buffer *buffer, BufferType type, void *data, int count, int size, bool update) {
        // Bind
        bindBuffer(type, buffer);

        // Get type and usage
        GLenum bufType, bufUsage;
        switch(type) {
            case BufferType::Vertex:
                bufType = GL_ARRAY_BUFFER;
                break;
            case BufferType::Index:
                bufType = GL_ELEMENT_ARRAY_BUFFER;
                break;
        }

        switch(buffer->Usage) {
            case BufferUsage::Static:
                bufUsage = GL_STATIC_DRAW;
                break;
            case BufferUsage::Dynamic:
                bufUsage = GL_DYNAMIC_DRAW;
                break;
            case BufferUsage::Stream:
                bufUsage = GL_STREAM_DRAW;
                break;
        }

        // Write
        if (update) {
            glBufferSubData(bufType, 0, size * count, data);
        } else glBufferData(bufType, size * count, data, bufUsage);
    }

    void OpenGLGraphicsDevice::_initShader(Shader *shader, const std::string &source) {
        // Create shader
        GLenum type;
        switch (shader->Type) {
            case ShaderType::Vertex:
                type = GL_VERTEX_SHADER;
                break;
            case ShaderType::Fragment:
                type = GL_FRAGMENT_SHADER;
                break;
        }
        shader->GLID = glCreateShader(type);

        // Set source
        auto src = source.c_str();
        glShaderSource(shader->GLID, 1, &src, nullptr);

        // Compile
        glCompileShader(shader->GLID);

        // Check for compile errors
        int params = -1;
        glGetShaderiv(shader->GLID, GL_COMPILE_STATUS, &params);
        if (GL_TRUE != params) {
            int max_length = 2048;
            int actual_length = 0;
            char shader_log[2048];
            glGetShaderInfoLog(shader->GLID, max_length, &actual_length, shader_log);
            Console::notice("OpenGL", "Shader info log for shader %u:\n%s", shader->GLID, shader_log);
            Console::fail("OpenGL", "Failed to compile shader %u.", shader->GLID);
        }
        Console::notice("OpenGL", "Successfully created and compiled shader %u.", shader->GLID);
    }

    void OpenGLGraphicsDevice::_freeShader(Shader *shader) {
        glDeleteShader(shader->GLID);
        shader->GLID = 0;
    }

    void OpenGLGraphicsDevice::_initShaderProgram(ShaderProgram *prog) {
        prog->GLID = glCreateProgram();
        Console::notice("OpenGL", "Successfully created shader program %u.", prog->GLID);
    }

    void OpenGLGraphicsDevice::_freeShaderProgram(ShaderProgram *prog) {
        glDeleteProgram(prog->GLID);
        prog->GLID = 0;
    }

    void OpenGLGraphicsDevice::_shaderProgramAttach(ShaderProgram *prog, Shader *shader) {
        glAttachShader(prog->GLID, shader->GLID);
    }

    void OpenGLGraphicsDevice::_linkShaderProgram(ShaderProgram *prog) {
        // Link
        glLinkProgram(prog->GLID);

        // Check for errors
        int params = -1;
        glGetProgramiv(prog->GLID, GL_LINK_STATUS, &params);
        if (GL_TRUE != params) {
            int max_length = 2048;
            int actual_length = 0;
            char program_log[2048];
            glGetProgramInfoLog(prog->GLID, max_length, &actual_length, program_log);
            Console::notice("OpenGL", "Program info log for %u:\n%s", prog->GLID, program_log);
            Console::fail("OpenGL", "Failed to link shader program %u.", prog->GLID);
        }
        Console::notice("OpenGL", "Successfully linked shader program %u.", prog->GLID);
    }

    void OpenGLGraphicsDevice::_useShaderProgram(ShaderProgram *prog) {
        glUseProgram(prog->GLID);
    }
}

#endif // defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)