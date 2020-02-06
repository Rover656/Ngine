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

#include "../Resource.hpp"
#include "GraphicsDevice.hpp"

namespace ngine::graphics {
    class ShaderProgram;

    namespace API {
        class PlatformGraphicsAPI;
    }

    /**
     * Shader type.
     *
     * @todo Should we support more?
     */
    enum class ShaderType {
        Vertex,
        Fragment
    };

    /**
     * A program that runs on the graphics card.
     */
    class NEAPI Shader final : public IResource {
        /**
         * Graphics API.
         */
        API::PlatformGraphicsAPI *m_API;
    public:
        union {
            /**
             * Shader ID.
             */
            unsigned int ID;
        };

        /**
         * This shader's type.
         */
        const ShaderType Type;

        /**
         * Create a shader with source strings.
         *
         * @todo Universal constructor.
         * @param src_ GLSL source
         */
        Shader(GraphicsDevice *graphicsDevice_, ShaderType type_, const char *src_);
        ~Shader();

        /**
         * Determine if the shader is valid.
         *
         * @note If the shader has not compiled, or failed to compile it will not be valid.
         * @return Whether or not the shader is valid.
         */
        bool isValid() const override;

        /**
         * Delete the shader.
         */
        void free() override;
    };
}

#endif //NEW_SHADER_H
