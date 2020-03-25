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

#ifndef NGINE_SHADER_HPP
#define NGINE_SHADER_HPP

#include "ngine/config.hpp"

#include <string>

namespace ngine::graphics {
    class GraphicsDevice;

    enum class ShaderType {
        Vertex,
        Fragment
    };

    class NEAPI Shader {
    public:
        union {
            /**
             * The OpenGL ID.
             * @warning DO NOT MODIFY.
             */
            unsigned int GLID;
        };

        /**
         * The shader type.
         */
        ShaderType Type;

        Shader(GraphicsDevice *graphicsDevice, const std::string &src, ShaderType type);
        ~Shader();
    private:
        /**
         * The graphics device.
         */
        graphics::GraphicsDevice *m_graphicsDevice;
    };
}

#endif //NGINE_SHADER_HPP
