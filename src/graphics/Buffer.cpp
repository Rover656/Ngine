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

#include "ngine/graphics/Buffer.hpp"

#include "ngine/graphics/API/PlatformGraphicsAPI.hpp"

namespace ngine::graphics {
    void Buffer::_writeBuffer(void *data, int count, int size, bool update) {
        // Use API
        m_API->writeBuffer(this, data, count, size, update);
    }

    Buffer::Buffer(GraphicsDevice *graphicsDevice, BufferType type, BufferUsage usage)
            : m_API(graphicsDevice->getAPI()), Type(type), Usage(usage) {
        // Get API and create buffer.
        m_API->createBuffer(this);
    }

    Buffer::~Buffer() {
        // Delete this buffer
        m_API->deleteBuffer(this);
    }

    void Buffer::bind() {
        m_API->bindBuffer(this);
    }

    void Buffer::unbind() {
        m_API->unbindBuffer(this);
    }
}