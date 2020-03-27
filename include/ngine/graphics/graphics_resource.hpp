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

#ifndef NGINE_GRAPHICS_RESOURCE_HPP
#define NGINE_GRAPHICS_RESOURCE_HPP

#include "ngine/config.hpp"

namespace ngine::graphics {
    class GraphicsDevice;

    enum class ResourceType {
        Buffer,
        Shader,
        ShaderProgram,
        Texture2D,
        VertexArray
    };

    class NEAPI GraphicsResource {
    public:
        union {
            /**
             * Resource OpenGL ID.
             * @warning DO NOT MODIFY.
             */
            unsigned int GLID;

            struct {
                /**
                 * Platform handle.
                 * @warning DO NOT MODIFY.
                 */
                void *Handle;

                /**
                 * Platform handle.
                 * @warning DO NOT MODIFY.
                 */
                void *Handle1;

                // TEMP FOR TESTING
                void *TempHandle;
            };
        };

        /**
         * Free the resource.
         */
        void free();

        /**
         * Get the resource type.
         */
        ResourceType getResourceType() const;
    protected:
        /**
         * The graphics device
         */
        GraphicsDevice *m_graphicsDevice;

        /**
         * My resource type.
         */
        ResourceType m_type;

        /**
         * Create a new graphics resource.
         */
        GraphicsResource(GraphicsDevice *graphicsDevice, ResourceType type);
        virtual ~GraphicsResource();
    };
}

#endif //NGINE_GRAPHICS_RESOURCE_HPP
