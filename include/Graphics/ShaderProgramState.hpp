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

#ifndef SHADERPROGRAMSTATE_HPP
#define SHADERPROGRAMSTATE_HPP

#include "../Config.hpp"

#include "ShaderProgram.hpp"

namespace Ngine::Graphics {
    /**
     * Shader uniform data handler.
     * This holds all information that is to be sent to the shader in GPU readable format, and exposes this in a simple API.
     */
    class NEAPI ShaderProgramState {
        /**
         * The data to be passed to the GPU.
         * This is manually allocated and managed.
         */
        void *m_data = nullptr;
    public:
        /**
         * The program that this state applies to.
         */
        const ShaderProgram *AttachedProgram;

        /**
         * Create a shader program state for the given program.
         *
         * @param program_ The program to create a state for.
         */
        ShaderProgramState(ShaderProgram *program_);
        ~ShaderProgramState();

        /**
         * Get uniform data (as void pointer.
         *
         * @param name_ The uniform to get.
         * @return The pointer to the data in our buffer
         */
        const void *GetUniform(const std::string &name_);

        /**
         * Get a uniform as a given type.
         *
         * @tparam Type Type to cast to.
         * @param name_ The name of the uniform to get.
         * @return The uniform data as the desired type.
         */
        template<typename Type>
        const Type *GetUniformAs(const std::string &name_) {
            return (Type *)GetUniform(name_);
        }

        /**
         * Set data for the given uniform.
         *
         * @param name_ Uniform for which to set the data.
         * @param data_ The date to write for the uniform. This expects `data_` to be the same size as the target uniform.
         */
        void SetUniform(const std::string &name_, void *data_);

        /**
         * Set a uniform (advanced).
         *
         * @param nameTree_ The tree of names to navigate. A number (in the form of a string) is expected if the target is an array. Ex. `{"TestArray", "5"}` to target `TestArray[5]`.
         * @param data_ Data to write to the target.
         */
        void SetUniformEx(std::vector<std::string> nameTree_, void *data_);

        /**
         * Get the data that is being stored.
         *
         * @return The data pointer.
         */
        const void *GetData() const;
    };
}

#endif //SHADERPROGRAMSTATE_HPP
