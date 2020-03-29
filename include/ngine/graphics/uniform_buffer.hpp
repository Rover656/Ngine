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

#ifndef NGINE_UNIFORM_BUFFER_HPP
#define NGINE_UNIFORM_BUFFER_HPP

#include "ngine/config.hpp"

#include "graphics_resource.hpp"
#include "shader_program.hpp"

namespace ngine::graphics {
    /**
     * A uniform buffer (not to be confused with graphics::Buffer) holds uniform data for a shader.
     * It follows a strict layout, set within the shader program it targets.
     * You can use uniform buffers across shaders as long as they obey the same format.
     */
    class NEAPI UniformBuffer : public GraphicsResource {
    public:
        UniformBuffer(GraphicsDevice *graphicsDevice, BufferLayout layout);
        ~UniformBuffer();

        void bind();

        void *getData() const;

        BufferLayout getLayout() const;

        bool isDirty() const;

        template<typename T>
        T getUniform(const char *name) {
            return *((T *)_getUniform(name, sizeof(T), 1));
        }

        template<typename T>
        T *getUniformArray(const char *name, int count) {
            return (T *)_getUniformArray(name, sizeof(T), count);
        }

        template<typename T>
        void setUniform(const char *name, T value) {
            _setUniform(name, &value, sizeof(T), 1);
        }

        template<typename T>
        void setUniformArray(const char *name, T *value, int count) {
            _setUniform(name, &value, sizeof(T), count);
        }

    private:
        /**
         * The layout for the internal data structure.
         */
        BufferLayout m_layout;

        /**
         * The uniform data.
         */
        void *m_data;

        /**
         * Whether or not the buffer is dirty (i.e. not been rewritten)
         */
        bool m_dirty = false;

        void *_getUniformArray(const char* name, int size, int count);

        /**
         * Set a raw uniform.
         */
        void _setUniform(const char *name, void *value, int size, int count);
    };
}

#endif //NGINE_UNIFORM_BUFFER_HPP
