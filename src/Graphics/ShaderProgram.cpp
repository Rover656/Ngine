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

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#include "Graphics/OpenGL.hpp"
#endif

#include "Graphics/Shader.hpp"

namespace Ngine::Graphics {
    ShaderProgram::ShaderProgram() {
        ID = glCreateProgram();
    }

    ShaderProgram::ShaderProgram(Shader *shader_) : ShaderProgram() {
        SetShader(shader_);
        Link();
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(ID);
        ID = 0;
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

    void ShaderProgram::SetShader(Shader *shader_) {
        if (!shader_->IsValid()) throw std::runtime_error("Invalid shader!");

        _Shader = shader_;
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        glAttachShader(ID, _Shader->_FragmentShader);
        glAttachShader(ID, _Shader->_VertexShader);
#endif

        _Linked = false;
    }

    bool ShaderProgram::Link() {
        _Linked = false;
        _AttribLocationCache.clear();
        _UniformLocationCache.clear();

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        // Link program
        glLinkProgram(ID);

        // Get link status
        int linked = GL_TRUE;
        glGetProgramiv(ID, GL_LINK_STATUS, &linked);

        if (linked == GL_TRUE) {
            _Linked = true;

            // Bind attrib locations
            glBindAttribLocation(ID, 0, "NG_VertexPos");
            glBindAttribLocation(ID, 1, "NG_VertexColor");
            glBindAttribLocation(ID, 2, "NG_VertexTexCoord");
        }
#endif

        return _Linked;
    }

    void ShaderProgram::Use() {
        if (!IsValid()) throw std::runtime_error("Attempted to use incomplete shader program.");
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        glUseProgram(ID);
#endif
    }

    bool ShaderProgram::IsValid() {
        return _Linked;
    }
}
