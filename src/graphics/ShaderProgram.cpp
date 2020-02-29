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

#include "ngine/graphics/ShaderProgram.hpp"

#include "ngine/graphics/API/PlatformGraphicsAPI.hpp"
#include "ngine/graphics/Shader.hpp"
#include "ngine/Console.hpp"

namespace ngine::graphics {
    ShaderProgram::ShaderProgram(GraphicsDevice *graphicsDevice, Shader *vertexShader, Shader *fragmentShader, std::vector<ShaderDataStructure> uniforms)
            : VertexShader(vertexShader), FragmentShader(fragmentShader) {
        // Check shaders
        if (VertexShader->Type != ShaderType::Vertex)
            Console::fail("ShaderProgram", "Vertex shader is not a vertex shader.");

        if (FragmentShader->Type != ShaderType::Fragment)
            Console::fail("ShaderProgram", "Fragment shader is not a fragment shader.");

        // Get API
        m_API = graphicsDevice->getAPI();

        // Create with API
        m_API->createShaderProgram(this);

        // Add uniforms
        for (const auto& uniform : uniforms) {
            addUniform(uniform);
        }
    }

    ShaderProgram::~ShaderProgram() {
        free();
    }

    void ShaderProgram::addUniform(ShaderDataStructure uniform) {
        // Verify
        if (uniform.Name.empty())
            Console::fail("ShaderProgram", "Uniform must have a name.");
        if (uniform.getSize() <= 0 || uniform.Count <= 0)
            Console::fail("ShaderProgram", "Uniform sizes are incorrect.");
        if (uniform.Offset != -1)
            Console::warn("ShaderProgram",
                          "Uniform offset has been set outside of the program, this value will be ignored.");

        // Get the current data size (total of previous offsets).
        auto size = getUniformDataSize();

        // Set offset
        uniform.Offset = size;

        // Add
        m_uniforms.push_back(uniform);
    }

    int ShaderProgram::getUniformDataSize() const {
        int s = 0;
        for (const auto& uniform : m_uniforms) {
            s += uniform.getSize() * uniform.Count;
        }
        return s;
    }

    std::vector<ShaderDataStructure> ShaderProgram::getUniforms() const {
        return m_uniforms;
    }

    void ShaderProgram::finalize() {
        m_final = true;
    }

    bool ShaderProgram::isFinal() const {
        return m_final;
    }

    bool ShaderProgram::isValid() const {
        return m_API->isShaderProgramValid(this);
    }

    void ShaderProgram::free() {
        m_API->deleteShaderProgram(this);
    }
}
