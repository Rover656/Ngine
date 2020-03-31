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

#include "graphics_resource.hpp"

#include <string>

namespace ngine::graphics {
    enum class ShaderStage {
        Vertex,
        Fragment
    };

    class NEAPI Shader : public GraphicsResource {
    public:
        /**
         * The shader type.
         */
        ShaderStage Type;

        Shader(IGraphicsDevice *graphicsDevice, const std::string &src, ShaderStage stage);
    };
}

#endif //NGINE_SHADER_HPP
