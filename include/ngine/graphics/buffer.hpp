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
        Dynamic
    };

    enum class BufferType {
        Vertex,
        Index,
        Uniform,
        // TODO: Instanced, after the removal of ES 2.0, we can confirm instancing support.
    };

    /**
     * A buffer stores data on the GPU.
     */
    class NEAPI Buffer : public GraphicsResource {
        friend class ngine::graphics::IGraphicsDevice;
    public:
        /**
         * Create a new buffer.
         * @param graphicsDevice Graphics device to create with.
         * @param type Buffer type.
         * @param usage Buffer usage.
         * @param size The size of each data entry.
         * @param count The number of data entries.
         * @param data The data to be written initially or null. Cannot be null for a static buffer.
         */
        Buffer(IGraphicsDevice *graphicsDevice, BufferType type, BufferUsage usage, void *data, unsigned int size, unsigned int count);

        /**
         * Get the buffer type.
         */
        BufferType getType() const;

        /**
         * Get the buffer usage mode.
         */
        BufferUsage getUsage() const;

        /**
         * Get the size of a single entry.
         */
        unsigned int getSize() const;

        /**
         * Get the number of entries in the buffer.
         */
        unsigned int getCount() const;

        /**
         * Write the given data to the buffer.
         * @warning The data provided is expected to be the correct size!
         * @warning Count may not exceed the buffer's count value.
         * @warning Static buffers cannot be written to in this way.
         * @param data Data to write.
         * @param count The number of data entries.
         */
        void write(void *data, unsigned int count);
    private:
        /**
         * The buffer type
         */
        const BufferType m_type;

        /**
         * Buffer usage mode.
         */
        const BufferUsage m_usage;

        /**
         * The size of the data
         */
        const unsigned int m_size;

        /**
         * The number of data entries in the buffer.
         */
        const unsigned int m_count;
    };
}

#endif //NGINE_BUFFER_HPP
