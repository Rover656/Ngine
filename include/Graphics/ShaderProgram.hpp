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

#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "../Config.hpp"

namespace NerdThings::Ngine::Graphics {
    class Shader;

    /**
     * Shader program which contains a shader and manages it.
     *
     * Predefined attributes:
     *  * vec3 - NG_VertexPos
     *  * vec2 - NG_VertexTexCoord
     *  * vec4 - NG_VertexColor
     *
     * Predefined uniforms:
     *  * mat4 - NGU_MATRIX_MVP
     *  * sampler2D - NGU_TEXTURE
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

#endif //SHADERPROGRAM_HPP
