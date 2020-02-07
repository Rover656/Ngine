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

#ifndef RENDERTARGET_HPP
#define RENDERTARGET_HPP

#include "../Config.hpp"

#include "../Resource.hpp"
#include "API/PlatformGraphicsAPI.hpp"
#include "Texture2D.hpp"

namespace ngine::graphics {
    /**
     * A 2D Texture in the GPU that can be rendered to.
     */
    class NEAPI RenderTarget final : public IResource {
        friend class GraphicsDevice;

        /**
         * The texture that the render target renders to.
         */
        Texture2D *m_texture;

        /**
         * The graphics API.
         */
        API::PlatformGraphicsAPI *m_API;
    public:
        union {
            /**
             * List of GPU ID's in OpenGL (2).
             * [0] - Framebuffer
             * [1] - Depth attachment
             */
            unsigned int ID[2] = {0, 0};
        };

        /**
         * Render target width.
         */
        const unsigned int Width = 0;

        /**
         * Render target height.
         */
        const unsigned int Height = 0;

        /**
         * Create a render target.
         *
         * @param width Render target width.
         * @param height Render target height.
         */
        RenderTarget(GraphicsDevice *graphicsDevice, unsigned int width, unsigned int height);
        ~RenderTarget();

        /**
         * Get rendered texture.
         *
         * @return Get the texture that rendered to.
         */
        Texture2D *getTexture();

        /**
         * Whether or not the render target is valid and usable.
         *
         * @return Whether the target is valid or not.
         */
        bool isValid() const override;

        /**
         * Free the render target.

         * @warning This will delete the texture too, ensure it is not going to be rendered this frame.
         */
        void free() override;

        bool operator==(const RenderTarget &b) const;
        bool operator!=(const RenderTarget &b) const;
    };
}

#endif //RENDERTARGET_HPP
