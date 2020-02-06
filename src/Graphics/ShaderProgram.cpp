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

#include "graphics/ShaderProgram.hpp"

#include "graphics/API/PlatformGraphicsAPI.hpp"
#include "graphics/Shader.hpp"
#include "Console.hpp"

namespace ngine::graphics {
    ShaderProgram::ShaderProgram(GraphicsDevice *graphicsDevice_, Shader *vertexShader_, Shader *fragmentShader_, std::vector<ShaderDataStructure> uniforms_)
            : VertexShader(vertexShader_), FragmentShader(fragmentShader_) {
        // Check shaders
        if (VertexShader->Type != ShaderType::Vertex)
            Console::Fail("ShaderProgram", "Vertex shader is not a vertex shader.");

        if (FragmentShader->Type != ShaderType::Fragment)
            Console::Fail("ShaderProgram", "Fragment shader is not a fragment shader.");

        // Get API
        m_API = graphicsDevice_->getAPI();

        // Create with API
        m_API->createShaderProgram(this);

        // Add uniforms
        for (const auto& uniform : uniforms_) {
            addUniform(uniform);
        }
    }

    ShaderProgram::~ShaderProgram() {
        free();
    }

    void ShaderProgram::addUniform(ShaderDataStructure uniform_) {
        // Verify
        if (uniform_.Name.empty())
            Console::Fail("ShaderProgram", "Uniform must have a name.");
        if (uniform_.getSize() <= 0 || uniform_.Count <= 0)
            Console::Fail("ShaderProgram", "Uniform sizes are incorrect.");
        if (uniform_.Offset != -1)
            Console::Warn("ShaderProgram", "Uniform offset has been set outside of the program, this value will be ignored.");

        // Get the current data size (total of previous offsets).
        auto size = getUniformDataSize();

        // Set offset
        uniform_.Offset = size;

        // Add
        m_uniforms.push_back(uniform_);
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
