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

#ifndef NGINE_VERTEX_ARRAY_HPP
#define NGINE_VERTEX_ARRAY_HPP

#include "ngine/config.hpp"

#include "buffer.hpp"
#include "graphics_resource.hpp"

#include <string>
#include <vector>

namespace ngine::graphics {
    class GraphicsDevice;

    // TODO: More permanent location
    enum class ElementType {
        Byte,
        UnsignedByte,
        Short,
        UnsignedShort,
        Int,
        UnsignedInt,
        Float
    };

    /**
     * A vertex buffer element is an entry in a vertex layout.
     * It details an element in the vertex data.
     */
    struct VertexBufferElement {
        /**
         * Vertex element name.
         */
        std::string Name;

        /**
         * Data type.
         */
        ElementType Type;

        /**
         * Data count.
         */
        int Count;

        /**
         * Is the data normalized?
         */
        bool Normalized;

        int getSize() const;
    };

    /**
     * Vertex buffer layout.
     * @note Should match layout in the shader.
     */
    struct VertexBufferLayout {
        /**
         * The vertex layouts elements.
         */
        std::vector<VertexBufferElement> Elements;

        int getSize() const;
    };

    /**
     * A vertex array defines a buffer filled with vertices and a buffer with the indices.
     * This controls both the array and the layout.
     */
    class NEAPI VertexArray : public GraphicsResource {
    public:
        /**
         * Create a vertex array.
         * @note The vertex array does not assume control over the buffers, these still need to be destroyed by you.
         * @param graphicsDevice The graphics device.
         * @param vertexBuffer The vertex buffer for the array.
         * @param indexBuffer The index buffer for the array. Can be null.
         * @param layout The vertex buffer layout.
         */
        VertexArray(graphics::GraphicsDevice *graphicsDevice, Buffer *vertexBuffer, Buffer *indexBuffer, VertexBufferLayout layout);

        /**
         * Destroy a vertex array.
         */
        ~VertexArray();

        Buffer *getVertexBuffer();

        Buffer *getIndexBuffer();

        const VertexBufferLayout getLayout() const;
    private:
        /**
         * The vertex buffer layout this array uses.
         */
        VertexBufferLayout m_vertexLayout;

        /**
         * Attached vertex buffer.
         */
        Buffer *m_vertexBuffer;

        /**
         * Attached index buffer.
         */
        Buffer *m_indexBuffer;
    };
}

#endif //NGINE_VERTEX_ARRAY_HPP
