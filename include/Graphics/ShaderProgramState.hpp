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

        ShaderProgramState(ShaderProgram *program_);
        ~ShaderProgramState();

        /**
         * Get uniform data (as void pointer.
         *
         * @param name_ The uniform to get.
         * @return The pointer to the data in our buffer
         */
        const void *GetUniform(const std::string &name_);

        template<typename Type>
        const Type *GetUniformAs(const std::string &name_) {
            return (Type *)GetUniform(name_);
        }

        /**
         * Set data for the given uniform.
         *
         * @param name_ Uniform for which to set the data.
         * @param data_ The date to write for the uniform.
         */
        void SetUniform(const std::string &name_, void *data_);

        /**
         * Get the data that is being stored.
         */
        const void *GetData() const;
    };
}

#endif //SHADERPROGRAMSTATE_HPP
