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

#include "Graphics/ShaderProgram.hpp"

#include "Graphics/API/PlatformGraphicsAPI.hpp"
#include "Graphics/Shader.hpp"
#include "Console.hpp"

namespace Ngine::Graphics {
    ShaderProgram::ShaderProgram(GraphicsDevice *graphicsDevice_, Shader *vertexShader_, Shader *fragmentShader_, std::vector<ShaderDataStructure> uniforms_)
            : VertexShader(vertexShader_), FragmentShader(fragmentShader_) {
        // Check shaders
        if (VertexShader->Type != ShaderType::Vertex)
            Console::Fail("ShaderProgram", "Vertex shader is not a vertex shader.");

        if (FragmentShader->Type != ShaderType::Fragment)
            Console::Fail("ShaderProgram", "Fragment shader is not a fragment shader.");

        // Get API
        m_API = graphicsDevice_->GetAPI();

        // Create with API
        m_API->CreateShaderProgram(this);

        // Add uniforms
        for (const auto& uniform : uniforms_) {
            AddUniform(uniform);
        }
    }

    ShaderProgram::~ShaderProgram() {
        Free();
    }

    void ShaderProgram::AddUniform(ShaderDataStructure uniform_) {
        // Verify
        if (uniform_.Name.empty())
            Console::Fail("ShaderProgram", "Uniform must have a name.");
        if (uniform_.GetSize() <= 0 || uniform_.Count <= 0)
            Console::Fail("ShaderProgram", "Uniform sizes are incorrect.");
        if (uniform_.Offset != -1)
            Console::Warn("ShaderProgram", "Uniform offset has been set outside of the program, this value will be ignored.");

        // Get the current data size (total of previous offsets).
        auto size = GetUniformDataSize();

        // Set offset
        uniform_.Offset = size;

        // Add
        m_uniforms.push_back(uniform_);
    }

    int ShaderProgram::GetUniformDataSize() const {
        int s = 0;
        for (const auto& uniform : m_uniforms) {
            s += uniform.GetSize() * uniform.Count;
        }
        return s;
    }

    std::vector<ShaderDataStructure> ShaderProgram::GetUniforms() const {
        return m_uniforms;
    }

    void ShaderProgram::Finalize() {
        m_final = true;
    }

    bool ShaderProgram::IsFinal() const {
        return m_final;
    }

    bool ShaderProgram::IsValid() const {
        return m_API->IsShaderProgramValid(this);
    }

    void ShaderProgram::Free() {
        m_API->DeleteShaderProgram(this);
    }
}
