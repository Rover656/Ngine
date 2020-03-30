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

#include "ngine/graphics/uniform_data.hpp"

#include "ngine/graphics/graphics_device.hpp"
#include "ngine/console.hpp"

#include <cstring>
#include <utility>

namespace ngine::graphics {
    unsigned int Uniform::getSize() const {
        switch (Type) {
            //case ElementType::Byte:
            //case ElementType::UnsignedByte:
            //    return sizeof(char) * Count;
            //case ElementType::Short:
            //case ElementType::UnsignedShort:
            //    return sizeof(short) * Count;
            case ElementType::Int:
            case ElementType::UnsignedInt:
                return sizeof(int) * Count;
            case ElementType::Float:
                return sizeof(float) * Count;
            case ElementType::Matrix:
                // 4x4 matrices
                return sizeof(float) * 16 * Count;
        }
    }

    UniformData::UniformData(GraphicsDevice *graphicsDevice, std::vector<Uniform> layout)
            : GraphicsResource(graphicsDevice, ResourceType::UniformData), m_uniforms(std::move(layout)) {
        m_graphicsDevice->_allocateUniformData(this, &m_offsets, &m_internalDataSize);
    }

    std::vector<Uniform> UniformData::getLayout() const {
        return m_uniforms;
    }

    void *UniformData::getData() const {
        return Handle;
    }

    unsigned int UniformData::getDataSize() const {
        return m_internalDataSize;
    }

    void UniformData::_write(const char *name, void *value, int size, int count) {
        // Find the uniform
        for (auto i = 0; i < m_uniforms.size(); i++) {
            auto u = m_uniforms[i];
            if (u.Name == name) {
                if (count > u.Count)
                    Console::fail("UniformData", "Too many elements for uniform array!");

                // Get offset
                auto off = m_offsets[i];

                // Write to buffer
                memcpy((unsigned char*) Handle + off, value, size * count);
                return;
            }
        }

        Console::fail("UniformData", "Failed to find uniform named %s.", name);
    }

    void UniformData::writeTo(Buffer *buffer) const {
        buffer->write(getData(), 1);
    }
}