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

#ifndef NEW_SHADER_H
#define NEW_SHADER_H

#include "../Config.hpp"

#ifdef USE_EXPERIMENTAL_RENDERER

#include "../Resource.hpp"

namespace NerdThings::Ngine::Graphics {
    class ShaderProgram;

    class NEAPI Shader : public IResource {
        // Set ShaderProgram as a friend.
        friend class ShaderProgram;

        /**
         * Vertex shader ID.
         */
        unsigned int _VertexShader;

        /**
         * Fragment shader ID.
         */
        unsigned int _FragmentShader;

        /**
         * Whether or not the shader is compiled
         */
        bool _Compiled = false;

        /**
         * Compile the given shader.
         */
        bool __Compile(unsigned int id_);

    public:
        /**
         * Create a shader with source strings.
         *
         * @param vertSrc_
         * @param fragSrc_
         */
        Shader(std::string vertSrc_, std::string fragSrc_);
        ~Shader();

        /**
         * Compile the shader.
         *
         * @return Whether or not the shader compiled successfully.
         */
        bool Compile();

        /**
         * Determine if the shader is valid.
         *
         * @note If the shader has not compiled, or failed to compile it will not be valid.
         * @return Whether or not the shader is valid.
         */
        bool IsValid() const override;

        /**
         * Delete the shader.
         */
        void Unload() override;
    };
}

#endif

#endif //NEW_SHADER_H
