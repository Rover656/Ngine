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

#include <type_traits>
#include "ngine/config.hpp"

namespace ngine::graphics {
    class GraphicsDevice;

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
    };

    /**
     * Base buffer class.
     */
    class NEAPI Buffer {
        friend class ngine::graphics::GraphicsDevice;
    public:
        union {
            /**
             * Buffer OGL id.
             * @warning DO NOT MODIFY.
             */
            unsigned int GLID;
        };

        /**
         * Buffer usage mode.
         */
        BufferUsage Usage;

        /**
         * Create a new buffer.
         * @param graphicsDevice Graphics device to create with.
         * @param usage Buffer usage.
         */
        Buffer(graphics::GraphicsDevice *graphicsDevice, BufferUsage usage);

        /**
         * Destroy the buffer.
         */
        virtual ~Buffer();

        /**
         * Write the given data to the buffer.
         * @param type The type of buffer to write as.
         * @param data Data to write.
         * @param count Number of data entries to write.
         * @param update Whether or not to update the buffer or resize.
         */
        template <typename T>
        void write(BufferType type, T *data, int count, bool update = false) {
            _writeInternal(type, data, count, sizeof(T), update);
        }
    protected:
        /**
         * The graphics device we belong to.
         */
        graphics::GraphicsDevice *m_graphicsDevice;
    private:
        /**
         * Internally write buffer data.
         */
        void _writeInternal(BufferType type, void *data, int count, int size, bool update = false);
    };
}

#endif //NGINE_BUFFER_HPP
