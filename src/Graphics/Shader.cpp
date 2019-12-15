/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Shader.hpp"

#ifdef USE_EXPERIMENTAL_RENDERER

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#include "OpenGL.h"
#endif

// TODO: Logger messages

namespace NerdThings::Ngine::Graphics {
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

#endif
