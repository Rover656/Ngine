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

namespace ngine::graphics {
    ShaderProgram::ShaderProgram(GraphicsDevice *graphicsDevice)
            : GraphicsResource(graphicsDevice, ResourceType::ShaderProgram) {
        m_graphicsDevice->_initShaderProgram(this);
    }

    ShaderProgram::~ShaderProgram() {
        free();
    }

    void ShaderProgram::attachShader(Shader *shader) {
        m_graphicsDevice->_shaderProgramAttach(this, shader);
    }

    void ShaderProgram::link() {
        m_graphicsDevice->_linkShaderProgram(this);
    }

    void ShaderProgram::use() {
        m_graphicsDevice->_useShaderProgram(this);
    }
}