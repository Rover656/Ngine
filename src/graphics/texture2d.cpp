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

#include "ngine/graphics/texture2d.hpp"

#include "ngine/graphics/graphics_device.hpp"

namespace ngine::graphics {
    Texture2D::Texture2D(IGraphicsDevice *graphicsDevice, Image *image)
            : IGraphicsResource(graphicsDevice, ResourceType::Texture2D) {
        // Get image data and save it
        m_width = image->getWidth();
        m_height = image->getHeight();
        m_format = image->getFormat();
        m_mipmapCount = image->getMipmapCount();

        m_graphicsDevice->_initTexture(this, image->getData());
    }

    PixelFormat Texture2D::getPixelFormat() const {
        return m_format;
    }

    int Texture2D::getWidth() const {
        return m_width;
    }

    int Texture2D::getHeight() const {
        return m_height;
    }

    unsigned int Texture2D::getMipmapCount() const {
        return m_mipmapCount;
    }

    void Texture2D::bind(unsigned int unit) {
        m_graphicsDevice->_bindTexture(unit, this);
    }

    void Texture2D::generateMipmaps() {
        m_mipmapCount = m_graphicsDevice->_generateTextureMipmaps(this);
    }
}