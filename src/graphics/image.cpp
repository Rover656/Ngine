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

#include "ngine/graphics/image.hpp"

#include <cstdlib>
#include <cstring>

namespace ngine::graphics {
    Image::Image(int width, int height, void *data, PixelFormat format, unsigned int mipmapCount)
     : m_width(width), m_height(height), m_format(format), m_mipmapCount(mipmapCount) {
        // Copy pixel data
        int size = getDataSize(width, height, format);
        m_data = malloc(size);
        memcpy(m_data, data, size);
    }

    Image::~Image() {
        // Free data
        free(m_data);
    }

    const void *Image::getData() const {
        return m_data;
    }

    int Image::getWidth() const {
        return m_width;
    }

    int Image::getHeight() const {
        return m_height;
    }

    PixelFormat Image::getFormat() const {
        return m_format;
    }

    unsigned int Image::getMipmapCount() const {
        return m_mipmapCount;
    }

    int Image::getBitsPerPixel(PixelFormat format) {
        int bpp;
        switch (format) {
            case PixelFormat::R8G8B8A8:
                bpp = 8 * 4;
                break;
        }
        return bpp;
    }

    int Image::getDataSize(int width, int height, PixelFormat format) {
        int bytesPerPixel = getBitsPerPixel(format) / 8;
        return width * height * bytesPerPixel;
    }
}