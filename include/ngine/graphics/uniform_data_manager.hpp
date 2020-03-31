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

#ifndef NGINE_UNIFORM_DATA_MANAGER_HPP
#define NGINE_UNIFORM_DATA_MANAGER_HPP

#include "ngine/config.hpp"

#include "buffer.hpp"
#include "graphics_resource.hpp"
#include "shader_program.hpp"

namespace ngine::graphics {
    struct NEAPI Uniform {
        /**
         * Uniform name.
         * For reference purposes.
         */
        const char *Name;

        /**
         * The uniform data type.
         */
        ElementType Type;

        /**
         * The number of this type between 1 and 4.
         * @note Must be one for Matrices.
         */
        unsigned int Count;

        /**
         * The number of elements in this array or 1 for no array.
         */
        unsigned int ArraySize = 1;

        unsigned int getSize() const;
    };

    /**
     * Manages uniform data for a buffer.
     * Write this to a buffer using the UniformData::writeTo() method or by passing the value returned by getData() to the Buffer::write() method.
     * @note In GLSL, all consumers of this buffer data *must* follow the std140 layout.
     */
    class NEAPI IUniformDataManager : public IGraphicsResource {
    public:
        virtual ~IUniformDataManager();

        std::vector<Uniform> getLayout() const;

        template<typename T>
        void setUniform(const char *name, T value) {
            _write(name, &value, sizeof(T), 1);
        }

        void *getData() const;

        unsigned int getDataSize() const;

        void writeTo(IBuffer *buffer) const;

        void free() override final;

    protected:
        /**
         * Create a uniform data manager
         * @warning layout MUST match the layout in the shader. Failure to do so may lead to unexpected results!
         */
        IUniformDataManager(IGraphicsDevice *graphicsDevice, std::vector<Uniform> layout);

        /**
         * The internal data.
         */
        void *m_data;

        /**
         * The size of the internal data structure.
         */
        unsigned int m_internalDataSize = 0;

        /**
         * The uniform layout, used to build the internal data structure.
         */
        std::vector<Uniform> m_uniforms;

        /**
         * The offsets for each uniform entry.
         */
        std::vector<unsigned int> m_offsets;

        /**
         * Write data to the internal structure.
         */
        void _write(const char *name, void *value, int size, int count);
    };
}

#endif //NGINE_UNIFORM_DATA_MANAGER_HPP
