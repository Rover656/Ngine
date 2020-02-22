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

#include "graphics/VertexLayout.hpp"

#include "graphics/Buffer.hpp"
#include "Console.hpp"

namespace ngine::graphics {
    VertexLayout::VertexLayout(GraphicsDevice *graphicsDevice, Buffer *vertexBuffer, Buffer *indexBuffer)
            : m_API(graphicsDevice->getAPI()) {
        // Check buffers
        if (vertexBuffer->Type != BufferType::Vertex || (indexBuffer != nullptr && indexBuffer->Type != BufferType::Index) )
            Console::fail("VertexLayout", "Buffers are not correct type.");

        // Save buffers
        m_vertexBuffer = vertexBuffer;
        m_indexBuffer = indexBuffer;

        // Use API to init
        m_API->createVertexLayout(this);
    }

    VertexLayout::~VertexLayout() {
        // Use API to cleanup
        m_API->deleteVertexLayout(this);
    }

    void VertexLayout::addElement(const VertexLayout::VertexElement &element) {
        if (m_locked)
            Console::fail("VertexLayout", "Cannot change layout once configured.");
        m_elements.push_back(element);
    }

    std::vector<VertexLayout::VertexElement> VertexLayout::getElements() {
        return m_elements;
    }

    void VertexLayout::configure() {
        // Configure
        m_API->configureVertexLayout(this);

        // Lock
        m_locked = true;
    }

    Buffer *VertexLayout::getVertexBuffer() {
        return m_vertexBuffer;
    }

    Buffer *VertexLayout::getIndexBuffer() {
        return m_indexBuffer;
    }

    void VertexLayout::use() {
        if (!m_using) {
            m_API->useVertexLayout(this);
            m_using = true;
        }
    }

    void VertexLayout::stop() {
        if (m_using) {
            m_API->stopVertexLayout(this);
            m_using = false;
        }
    }
}