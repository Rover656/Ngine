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

#include "Graphics/API/Buffer.hpp"

namespace Ngine::Graphics::API {
    void Buffer::_writeBuffer(void *data_, int count_, int size_, bool update_) {
        // Use API
        m_API->WriteBuffer(this, data_, count_, size_, update_);
    }

    Buffer::Buffer(GraphicsDevice *graphicsDevice_, BufferType type_, BufferUsage usage_)
            : m_API(graphicsDevice_->GetAPI()), Type(type_), Usage(usage_) {
        // Get API and create buffer.
        m_API->InitializeBuffer(this);
    }

    Buffer::~Buffer() {
        // Delete this buffer
        m_API->CleanupBuffer(this);
    }

    void Buffer::Bind() {
        m_API->BindBuffer(this);
    }

    void Buffer::Unbind() {
        m_API->UnbindBuffer(this);
    }
}