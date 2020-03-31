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

#include "ngine/graphics/uniform_data_manager.hpp"

#include "ngine/graphics/graphics_device.hpp"
#include "ngine/console.hpp"

#include <cstring>
#include <utility>

namespace ngine::graphics {
    unsigned int Uniform::getSize() const {
        switch (Type) {
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

    IUniformDataManager::~IUniformDataManager() {
        free();
    }

    std::vector<Uniform> IUniformDataManager::getLayout() const {
        return m_uniforms;
    }

    void *IUniformDataManager::getData() const {
        return m_data;
    }

    unsigned int IUniformDataManager::getDataSize() const {
        return m_internalDataSize;
    }

    IUniformDataManager::IUniformDataManager(IGraphicsDevice *graphicsDevice, std::vector<Uniform> layout)
            : IGraphicsResource(graphicsDevice, ResourceType::UniformDataManager), m_uniforms(std::move(layout)) {}

    void IUniformDataManager::_write(const char *name, void *value, int size, int count) {
        // Find the uniform
        for (auto i = 0; i < m_uniforms.size(); i++) {
            auto u = m_uniforms[i];
            if (u.Name == name) {
                if (count > u.Count)
                    Console::fail("UniformData", "Too many elements for uniform array!");

                // Get offset
                auto off = m_offsets[i];

                // Write to buffer
                memcpy((unsigned char*) m_data + off, value, size * count);
                return;
            }
        }

        Console::fail("UniformData", "Failed to find uniform named %s.", name);
    }

    void IUniformDataManager::writeTo(IBuffer *buffer) const {
        buffer->write(getData(), 1);
    }

    void IUniformDataManager::free() {
        ::free(m_data);
        m_data = nullptr;
    }
}