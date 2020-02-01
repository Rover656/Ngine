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

#include "GraphicsDevice.hpp"

namespace Ngine::Graphics {
    class Shader;

    namespace API {
        class PlatformGraphicsAPI;
    }

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
    class NEAPI ShaderProgram final : public IResource {
    private:
        /**
         * The graphics API.
         */
        API::PlatformGraphicsAPI *m_API;

        /**
         * Attribute location cache.
         */
        std::unordered_map<std::string, unsigned int> _AttribLocationCache;

        /**
         * Uniform location cache.
         */
        std::unordered_map<std::string, unsigned int> _UniformLocationCache;
    public:
        union {
            /**
             * Shader program ID.
             */
            unsigned int ID = 0;
        };

        /**
         * The attached vertex shader.
         */
        const Shader *VertexShader;

        /**
         * The attached fragment/pixel shader.
         */
        const Shader *FragmentShader;

        /**
         * Create a shader program.
         *
         * @todo Rework this constructor if we want to support more kinds of shader.
         * @param graphicsDevice_ The graphics device.
         * @param vertexShader_ The vertex shader.
         * @param fragmentShader_ The fragment shader.
         */
        ShaderProgram(GraphicsDevice *graphicsDevice_, Shader *vertexShader_, Shader *fragmentShader_);
        ~ShaderProgram();

        // These need a universal system.
        unsigned int GetAttribLocation(const std::string &name_);
        unsigned int GetUniformLocation(const std::string &name_);

        /**
         * Determine if the shader program is valid.
         */
        bool IsValid() const override;

        /**
         * Free the shader program.
         */
        void Free() override;
    };
}

#endif //SHADERPROGRAM_HPP
