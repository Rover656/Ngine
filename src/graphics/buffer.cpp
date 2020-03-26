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
    Buffer::Buffer(graphics::GraphicsDevice *graphicsDevice, BufferType type, BufferUsage usage, int dataSize,
                   int dataCount) : GraphicsResource(graphicsDevice, ResourceType::Buffer), Type(type), Usage(usage),
                                    Size(dataSize), Count(dataCount) {
        m_graphicsDevice->_initBuffer(this, dataSize, dataCount);
    }

    Buffer::~Buffer() {
        free();
    }

    void Buffer::write(void *data, int size, int count) {
        if (size != Size)
            Console::fail("Buffer", "Cannot write data with different size to buffer!");
        if (count > Count)
            Console::fail("Buffer", "Attempt to write too many elements to buffer!");

        m_graphicsDevice->_writeBuffer(this, data, count);
    }
}