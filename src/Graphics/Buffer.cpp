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

#include "graphics/Buffer.hpp"

#include "graphics/API/PlatformGraphicsAPI.hpp"

namespace ngine::graphics {
    void Buffer::_writeBuffer(void *data_, int count_, int size_, bool update_) {
        // Use API
        m_API->writeBuffer(this, data_, count_, size_, update_);
    }

    Buffer::Buffer(GraphicsDevice *graphicsDevice_, BufferType type_, BufferUsage usage_)
            : m_API(graphicsDevice_->getAPI()), Type(type_), Usage(usage_) {
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