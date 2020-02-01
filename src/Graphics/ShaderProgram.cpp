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

#include "Graphics/OpenGL.hpp" // TEMP

#include "Graphics/API/PlatformGraphicsAPI.hpp"
#include "Graphics/Shader.hpp"
#include "Console.hpp"

namespace Ngine::Graphics {
    ShaderProgram::ShaderProgram(GraphicsDevice *graphicsDevice_, Shader *vertexShader_, Shader *fragmentShader_)
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
    }

    ShaderProgram::~ShaderProgram() {
        Free();
    }

    unsigned int ShaderProgram::GetAttribLocation(const std::string &name_) {
        if (_AttribLocationCache.find(name_) != _AttribLocationCache.end())
            return _AttribLocationCache.at(name_);
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        unsigned int loc = glGetAttribLocation(ID, name_.c_str());
        _AttribLocationCache.insert({name_, loc});
        return loc;
#endif
        return 0;
    }

    unsigned int ShaderProgram::GetUniformLocation(const std::string &name_) {
        if (_UniformLocationCache.find(name_) != _UniformLocationCache.end())
            return _UniformLocationCache.at(name_);
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        unsigned int loc = glGetUniformLocation(ID, name_.c_str());
        _UniformLocationCache.insert({name_, loc});
        return loc;
#endif
        return 0;
    }

    bool ShaderProgram::IsValid() const {
        return m_API->IsShaderProgramValid(this);
    }

    void ShaderProgram::Free() {
        m_API->DeleteShaderProgram(this);
    }
}
