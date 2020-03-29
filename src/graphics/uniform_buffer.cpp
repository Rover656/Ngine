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

#include "ngine/graphics/uniform_buffer.hpp"

#include "ngine/graphics/graphics_device.hpp"
#include "ngine/console.hpp"

#include <cstdlib>
#include <cstring>

namespace ngine::graphics {
    UniformBuffer::UniformBuffer(GraphicsDevice *graphicsDevice, BufferLayout layout)
            : GraphicsResource(graphicsDevice, ResourceType::UniformBuffer), m_layout(layout) {
        // Allocate memory for the data (calloc so that the memory is zeroed)
        m_data = calloc(1, m_layout.getSize());
        m_graphicsDevice->_initUniformBuffer(this);
    }

    UniformBuffer::~UniformBuffer() {
        // Free
        ::free(m_data);
    }

    void UniformBuffer::bind() {
        m_graphicsDevice->_bindUniformBuffer(this);
        m_dirty = false;
    }

    void *UniformBuffer::getData() const {
        return m_data;
    }

    BufferLayout UniformBuffer::getLayout() const {
        return m_layout;
    }

    bool UniformBuffer::isDirty() const {
        return m_dirty;
    }

    void *UniformBuffer::_getUniformArray(const char *name, int size, int count) {
        int offset = 0;
        for (auto e : m_layout.Elements) {
            if (e.Name == name) {
                // Check data size
                if (size != e.getSize())
                    Console::fail("UniformBuffer", "Invalid data size for uniform named \"%s\"!", name);
                if (count != e.ArraySize)
                    Console::fail("UniformBuffer", "Array size must match!");

                // Set the data then return.
                auto dat = (unsigned char *)m_data + offset;
                return dat;
            }

            offset += e.getSize();
        }
        Console::fail("UniformBuffer", "Could not find a uniform named \"%s\" in the layout!", name);
        return nullptr;
    }

    void UniformBuffer::_setUniform(const char *name, void *value, int size, int count) {
        int offset = 0;
        for (auto e : m_layout.Elements) {
            if (e.Name == name) {
                // Check data size
                if (size != e.getSize())
                    Console::fail("UniformBuffer", "Invalid data size for uniform named \"%s\"!", name);
                if (count != e.ArraySize)
                    Console::fail("UniformBuffer", "Array size must match!");

                // Set the data then return.
                auto dat = (unsigned char *)m_data + offset;
                memcpy(dat, value, size * count);
                m_dirty = true;
                return;
            }

            offset += e.getSize();
        }
        Console::fail("UniformBuffer", "Could not find a uniform named \"%s\" in the layout!", name);
    }
}