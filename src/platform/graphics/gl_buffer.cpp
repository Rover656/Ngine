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

#include "gl_buffer.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

namespace ngine::platform::graphics {
    GLBuffer::GLBuffer(IGraphicsDevice *graphicsDevice, BufferType type, BufferUsage usage, void *data,
                       unsigned int size, unsigned int count) : IBuffer(graphicsDevice, type, usage, data, size,
                                                                        count) {
        // Create
        glGenBuffers(1, &m_id);

        // Get GL enums
        GLenum glType = toGLType(m_type);
        GLenum glUsage = toGLUsage(m_usage);

        // Bind and write
        bind();
        glBufferData(glType, m_size * m_count, data, glUsage);
    }

    GLenum GLBuffer::toGLType(BufferType type) {
        switch (type) {
            case BufferType::Vertex:
                return GL_ARRAY_BUFFER;
            case BufferType::Index:
                return GL_ELEMENT_ARRAY_BUFFER;
            case BufferType::Uniform:
                return GL_UNIFORM_BUFFER;
        }
    }

    GLenum GLBuffer::toGLUsage(BufferUsage usage) {
        switch (usage) {
            case BufferUsage::Static:
                return GL_STATIC_DRAW;
            case BufferUsage::Dynamic:
                return GL_DYNAMIC_DRAW;
        }
    }

    GLBuffer::~GLBuffer() {
        free();
    }

    void GLBuffer::free() {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }

    void GLBuffer::bind(int slot) {
        // Get GL enums
        GLenum glType = toGLType(m_type);

        switch (m_type) {
            case BufferType::Vertex:
            case BufferType::Index:
                // Bind
                glBindBuffer(glType, m_id);
                break;
            case BufferType::Uniform:
                glBindBufferBase(glType, slot, m_id);
                break;
        }
    }

    void GLBuffer::_write(void *data, unsigned int count) {
        // Get GL enums
        GLenum glType = toGLType(m_type);

        // Bind
        bind();

        // Write (we don't use glBufferData as we want to restrict buffer size for parity with DX11).
        glBufferSubData(glType, 0, m_size * count, data);
    }
}

#endif // defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)
