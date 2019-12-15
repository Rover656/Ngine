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

#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "../Config.hpp"

#ifdef USE_EXPERIMENTAL_RENDERER

namespace NerdThings::Ngine::Graphics {
    class Shader;

    /**
     * Shader program which contains a shader and manages it.
     *
     * Predefined attributes:
     *  * NG_VertexPos
     *  * NG_VertexTexCoord
     *  * NG_VertexColor
     *
     * Predefined uniforms:
     *  * NGU_MATRIX_MVP
     *  * NGU_TEXTURE
     */
    class NEAPI ShaderProgram {
    private:
        /**
         * The attached shader.
         */
        Shader *_Shader = nullptr;

        /**
         * Whether or not the shader program has linked.
         */
        bool _Linked = false;

        /**
         * Attribute location cache.
         */
        std::unordered_map<std::string, unsigned int> _AttribLocationCache;

        /**
         * Uniform location cache.
         */
        std::unordered_map<std::string, unsigned int> _UniformLocationCache;
    public:
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        /**
         * Shader program ID.
         */
        unsigned int ID = 0;
#endif

        ShaderProgram();
        explicit ShaderProgram(Shader *shader_);
        ~ShaderProgram();

        unsigned int GetAttribLocation(const std::string &name_);

        unsigned int GetUniformLocation(const std::string &name_);

        void SetShader(Shader *shader_);

        bool Link();

        void Use();

        bool IsValid();
    };
}

#endif

#endif //SHADERPROGRAM_HPP
