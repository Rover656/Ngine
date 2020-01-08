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

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#include "Buffer.hpp"

// Platform specifics
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21)
#include <glad/glad.h>
#elif defined(GRAPHICS_OPENGLES2)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1 // UWP???
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#endif

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#elif defined(PLATFORM_UWP)
#include <angle_windowsstore.h>
#endif

#include "../../Logger.hpp"

namespace NerdThings::Ngine::Graphics::OpenGL {
    GLBuffer::GLBuffer(GLBufferType type_, GLBufferUsage usage_) : _Type(type_), _Usage(usage_) {
        // Generate buffer
        glGenBuffers(1, &ID);

        // Logging
        Logger::Notice("GLBuffer", "Created new buffer with ID, %i.", ID);
    }

    GLBuffer::~GLBuffer() {
        // Delete buffer
        glDeleteBuffers(1, &ID);
        Logger::Notice("GLBuffer", "Deleted buffer with ID %i.", ID);

        // Set ID to 0
        ID = 0;
    }

    void GLBuffer::Bind() {
        // Bind buffer
        glBindBuffer(_Type, ID);
    }

    GLBufferType GLBuffer::GetBufferType() {
        return _Type;
    }

    GLBufferUsage GLBuffer::GetBufferUsage() {
        return _Usage;
    }

    void GLBuffer::SetData(void *data_, int dataSize_) {
        // Bind
        Bind();

        // Set data
        glBufferData(_Type, dataSize_, data_, _Usage);
    }
}
#endif
