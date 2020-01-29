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

#include "Graphics/VertexLayout.hpp"

#include "Graphics/Buffer.hpp"
#include "Console.hpp"

namespace Ngine::Graphics {
    VertexLayout::VertexLayout(GraphicsDevice *graphicsDevice_, Buffer *vertexBuffer_, Buffer *indexBuffer_)
            : m_API(graphicsDevice_->GetAPI()) {
        // Check buffers
        if (vertexBuffer_->Type != BufferType::Vertex || (indexBuffer_ != nullptr && indexBuffer_->Type != BufferType::Index) )
            Console::Fail("VertexLayout", "Buffers are not correct type.");

        // Save buffers
        m_vertexBuffer = vertexBuffer_;
        m_indexBuffer = indexBuffer_;

        // Use API to init
        m_API->CreateVertexLayout(this);
    }

    VertexLayout::~VertexLayout() {
        // Use API to cleanup
        m_API->DeleteVertexLayout(this);
    }

    void VertexLayout::AddElement(const VertexLayout::VertexElement &element_) {
        if (m_locked)
            Console::Fail("VertexLayout", "Cannot change layout once configured.");
        m_elements.push_back(element_);
    }

    std::vector<VertexLayout::VertexElement> VertexLayout::GetElements() {
        return m_elements;
    }

    void VertexLayout::Configure() {
        // Configure
        m_API->ConfigureVertexLayout(this);

        // Lock
        m_locked = true;
    }

    Buffer *VertexLayout::GetVertexBuffer() {
        return m_vertexBuffer;
    }

    Buffer *VertexLayout::GetIndexBuffer() {
        return m_indexBuffer;
    }

    void VertexLayout::Use() {
        if (!m_using) {
            m_API->UseVertexLayout(this);
            m_using = true;
        }
    }

    void VertexLayout::Stop() {
        if (m_using) {
            m_API->StopVertexLayout(this);
            m_using = false;
        }
    }
}