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

#ifndef NGINE_BUFFER_HPP
#define NGINE_BUFFER_HPP

#include "ngine/config.hpp"

#include "graphics_resource.hpp"

namespace ngine::graphics {
    /**
     * Buffer usage descriptor.
     */
    enum class BufferUsage {
        Static,
        Dynamic,
        Stream
    };

    enum class BufferType {
        Vertex,
        Index
        // TODO: Instanced, we want to allow instancing on capable hardware.
    };

    /**
     * Base buffer class.
     */
    class NEAPI Buffer : public GraphicsResource {
        friend class ngine::graphics::GraphicsDevice;
    public:
        /**
         * The buffer type
         */
        BufferType Type;

        /**
         * Buffer usage mode.
         */
        BufferUsage Usage;

        /**
         * Size of each element in the buffer.
         */
        const int Size;

        /**
         * Number of elements in the buffer.
         */
        const int Count;

        /**
         * Create a new buffer.
         * @param graphicsDevice Graphics device to create with.
         * @param type Buffer type.
         * @param usage Buffer usage.
         * @param dataSize The size of each buffer entry.
         * @param dataCount The number of entries in the buffer.
         */
        Buffer(GraphicsDevice *graphicsDevice, BufferType type, BufferUsage usage, int dataSize, int dataCount);

        /**
         * Write the given data to the buffer.
         * @warning Cannot exceed size set in constructor.
         * @param data Data to write.
         * @param size Size of each data entry (must match constructor).
         * @param count Number of data entries to write (must not exceed constructor value).
         */
        void write(void *data, int size, int count);
    };
}

#endif //NGINE_BUFFER_HPP
