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

#include "ngine/graphics/buffer.hpp"

#include "ngine/graphics/graphics_device.hpp"
#include "ngine/console.hpp"

namespace ngine::graphics {
    Buffer::Buffer(IGraphicsDevice *graphicsDevice, BufferType type, BufferUsage usage, void *data, unsigned int size,
                   unsigned int count)
            : IGraphicsResource(graphicsDevice, ResourceType::Buffer), m_type(type), m_usage(usage), m_size(size),
              m_count(count) {
        m_graphicsDevice->_initBuffer(this, data, m_size * m_count);
    }

    BufferType Buffer::getType() const {
        return m_type;
    }

    BufferUsage Buffer::getUsage() const {
        return m_usage;
    }

    unsigned int Buffer::getSize() const {
        return m_size;
    }

    void Buffer::write(void *data, unsigned int count) {
        if (m_usage == BufferUsage::Static)
            Console::fail("Buffer",
                          "Cannot write to a static buffer! Create a dynamic buffer if you need this ability!");
        if (count > m_count)
            Console::fail("Buffer", "Cannot write more elements than the buffer can store!");
        m_graphicsDevice->_writeBuffer(this, data, m_size * count);
    }
}