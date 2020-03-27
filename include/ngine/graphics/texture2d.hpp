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

#ifndef NGINE_TEXTURE2D_HPP
#define NGINE_TEXTURE2D_HPP

#include "ngine/config.hpp"

#include "graphics_resource.hpp"

namespace ngine::graphics {
    // TODO: Move to Image class once we get one. This will control the pixel data (including flippage).
    // TODO: Add all supported formats.
    enum class PixelFormat {
        R8G8B8A8,
    };

    /**
     * Texture properties
     */
    struct TextureProperties {
        // TODO
    };

    class NEAPI Texture2D : public GraphicsResource {
    public:
        /**
         * Create a texture with existing pixel data.
         */
        Texture2D(GraphicsDevice *graphicsDevice, int width, int height, void *pixelData, PixelFormat format);

        PixelFormat getPixelFormat() const;
        int getWidth() const;
        int getHeight() const;
    private:
        PixelFormat m_format;
        // TODO: Store the image
        int m_width;
        int m_height;
    };
}

#endif //NGINE_TEXTURE2D_HPP
