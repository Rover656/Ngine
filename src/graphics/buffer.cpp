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

namespace ngine::graphics {
    Buffer::Buffer(graphics::GraphicsDevice *graphicsDevice, BufferUsage usage) : m_graphicsDevice(graphicsDevice), Usage(usage) {
        m_graphicsDevice->_initBuffer(this);
    }

    Buffer::~Buffer() {
        m_graphicsDevice->_freeBuffer(this);
    }

    void Buffer::_writeInternal(BufferType type, void *data, int count, int size, bool update) {
        m_graphicsDevice->_writeBuffer(this, type, data, count, size, update);
    }
}