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

#ifndef NGINE_SHADER_PROGRAM_HPP
#define NGINE_SHADER_PROGRAM_HPP

#include "ngine/config.hpp"

#include "shader.hpp"

#include <vector>

namespace ngine::graphics {
    class GraphicsDevice;

    class NEAPI ShaderProgram {
    public:
        union {
            /**
             * OpenGL program ID.
             * @warning DO NOT MODIFY.
             */
            unsigned int GLID;
        };

        ShaderProgram(GraphicsDevice *graphicsDevice);
        ~ShaderProgram();

        void attachShader(Shader *shader);
        void link();
        void use();
    private:
        GraphicsDevice *m_graphicsDevice;
        std::vector<Shader *> m_shaders;
    };
}

#endif //NGINE_SHADER_PROGRAM_HPP
