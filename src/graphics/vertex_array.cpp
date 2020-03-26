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

#include "ngine/graphics/vertex_array.hpp"

#include "ngine/graphics/graphics_device.hpp"

namespace ngine::graphics {
    int VertexBufferElement::getSize() const {
        switch (Type) {
            case ElementType::Byte:
            case ElementType::UnsignedByte:
                return sizeof(char) * Count;
            case ElementType::Short:
            case ElementType::UnsignedShort:
                return sizeof(short) * Count;
            case ElementType::Int:
            case ElementType::UnsignedInt:
                return sizeof(int) * Count;
            case ElementType::Float:
                return sizeof(float) * Count;
        }
    }

    int VertexBufferLayout::getSize() const {
        int s = 0;
        for (const auto &e : Elements) {
            s += e.getSize();
        }
        return s;
    }

    VertexArray::VertexArray(graphics::GraphicsDevice *graphicsDevice, Buffer *vertexBuffer, Buffer *indexBuffer,
                             VertexBufferLayout layout)
            : GraphicsResource(graphicsDevice, ResourceType::VertexArray), m_vertexBuffer(vertexBuffer),
              m_indexBuffer(indexBuffer), m_vertexLayout(layout) {
        m_graphicsDevice->_initVertexArray(this);
    }

    VertexArray::~VertexArray() {
        free();
    }

    Buffer *VertexArray::getVertexBuffer() {
        return m_vertexBuffer;
    }

    Buffer *VertexArray::getIndexBuffer() {
        return m_indexBuffer;
    }

    const VertexBufferLayout VertexArray::getLayout() const {
        return m_vertexLayout;
    }
}