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

#include "Graphics/Shader.hpp"

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#include "Graphics/OpenGL.hpp"
#endif

// TODO: Logger messages

namespace Ngine::Graphics {
    bool Shader::__Compile(unsigned int id_) {
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        // Compile
        glCompileShader(id_);

        // Check compile status
        int success = GL_TRUE;
        glGetShaderiv(id_, GL_COMPILE_STATUS, &success);

        return success == GL_TRUE;
#endif
        return false;
    }

    Shader::Shader(std::string vertSrc_, std::string fragSrc_) {
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        // Create shaders
        _VertexShader = glCreateShader(GL_VERTEX_SHADER);
        _FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Set source
        auto vSrc = vertSrc_.c_str();
        auto fSrc = fragSrc_.c_str();
        glShaderSource(_VertexShader, 1, &vSrc, nullptr);
        glShaderSource(_FragmentShader, 1, &fSrc, nullptr);
#endif
    }

    Shader::~Shader() {
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        Unload();
#endif
    }

    bool Shader::Compile() {
        // Unset compiled
        _Compiled = false;

        // Try to compile
        if (!__Compile(_VertexShader))
            return false;
        if (!__Compile(_FragmentShader))
            return false;

        // We managed.
        _Compiled = true;
        return true;
    }

    bool Shader::IsValid() const {
        return _Compiled;
    }

    void Shader::Unload() {
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        // Delete shaders
        glDeleteShader(_VertexShader);
        glDeleteShader(_FragmentShader);
        _VertexShader = 0;
        _FragmentShader = 0;
#endif
    }
}
