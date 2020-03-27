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

#include "ngine/graphics/shader_program.hpp"

#include "ngine/graphics/graphics_device.hpp"
#include "ngine/console.hpp"

namespace ngine::graphics {
    int VertexBufferElement::getSize() const {
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
        }
    }

    int VertexBufferLayout::getSize() const {
        int s = 0;
        for (const auto &e : Elements) {
            s += e.getSize();
        }
        return s;
    }
    
    ShaderProgram::ShaderProgram(GraphicsDevice *graphicsDevice, VertexBufferLayout layout)
            : GraphicsResource(graphicsDevice, ResourceType::ShaderProgram), m_layout(layout) {
        m_graphicsDevice->_initShaderProgram(this);
    }

    const VertexBufferLayout ShaderProgram::getLayout() const {
        return m_layout;
    }

    void ShaderProgram::attachShader(Shader *shader) {
        for (auto s : m_shaders) {
            if (s->Type == shader->Type) {
                Console::fail("ShaderProgram", "A shader program can only have one of each shader type!");
            }
        }

        m_graphicsDevice->_shaderProgramAttach(this, shader);
        m_shaders.push_back(shader);
    }

    Shader *ShaderProgram::getShaderByType(ShaderType type) {
        for (auto s : m_shaders) {
            if (s->Type == type)
                return s;
        }
        return nullptr;
    }

    void ShaderProgram::link() {
        m_graphicsDevice->_linkShaderProgram(this);
    }

    void ShaderProgram::use() {
        m_graphicsDevice->_useShaderProgram(this);
    }
}