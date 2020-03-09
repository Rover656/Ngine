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

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "../Config.hpp"

#include "GraphicsDevice.hpp"

namespace ngine::graphics {
    /**
     * A GPU buffer type.
     */
    enum class BufferType {
        Vertex,
        Index
        // TODO: Support other buffer types.
    };

    /**
     * Buffer memory usage
     */
    enum class BufferUsage { Static, Dynamic };

    /**
     * A GPU buffer.
     */
    class NEAPI Buffer {
        /**
         * The graphics API.
         */
        API::PlatformGraphicsAPI *m_API;

        /**
         * Write data to the buffer (internal).
         *
         * @param data The data array.
         * @param count The amount of data.
         * @param size The size of one element.
         */
        void _writeBuffer(void *data, int count, int size, bool update);

    public:
        union {
            /**
             * Buffer ID
             */
            unsigned int ID;

            /**
             * Buffer Pointer
             */
            void *Ptr;
        };

        /**
         * Buffer type.
         */
        const BufferType Type;

        /**
         * Buffer memory usage.
         */
        const BufferUsage Usage;

        /**
         * Create a new GPU buffer.
         */
        Buffer(GraphicsDevice *graphicsDevice, BufferType type,
               BufferUsage usage);
        ~Buffer();

        /**
         * Bind the buffer.
         */
        void bind();

        /**
         * Unbind the buffer.
         */
        void unbind();

        /**
         * Write data to the buffer.
         *
         * @tparam Type The type that is stored in the data.
         * @param data The data pointer.
         * @param count The amount of data.
         * @param update Whether or not this is an update. If the size of the
         * buffer is not changed and the buffer is dynamic, then use this.
         */
        template <typename Type>
        void write(Type *data, int count, bool update = false) {
            _writeBuffer(data, count, sizeof(Type), update);
        }
    };
} // namespace ngine::graphics

#endif // BUFFER_HPP
