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

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "../../Ngine.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
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
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        /**
         * Shader program ID.
         */
        unsigned int ID = 0;
#endif
        /**
         * The attached shader.
         */
        Shader *_Shader = nullptr;

        /**
         * Whether or not the shader program has linked.
         */
        bool _Linked = false;
    public:
        ShaderProgram();
        explicit ShaderProgram(Shader *shader_);
        ~ShaderProgram() = default;

        unsigned int GetAttribLocation(const std::string &name_) const;

        unsigned int GetUniformLocation(const std::string &name_) const;

        void SetShader(Shader *shader_);

        bool Link();

        void Use();

        bool IsValid();
    };
}

#endif //SHADERPROGRAM_H
