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

#ifndef NGINE_IMAGE_HPP
#define NGINE_IMAGE_HPP

#include "ngine/config.hpp"

namespace ngine::graphics {
    // TODO: Add all supported formats.
    enum class PixelFormat {
        R8G8B8A8,
    };

    class NEAPI Image {
    public:
        /**
         * Create an image from raw pixel data.
         * @note The pixel data will be copied.
         * @param width Image width.
         * @param height Image height.
         * @param data Image pixel data.
         * @param format Image pixel data format.
         * @param mipmapCount Image mipmap count. 1 for most cases.
         */
        Image(int width, int height, void *data, PixelFormat format, unsigned int mipmapCount = 1);

        // TODO: Create from file

        /**
         * Destroy image
         */
        ~Image();

        /**
         * Get image pixel data.
         */
        const void *getData() const;

        int getWidth() const;

        int getHeight() const;

        PixelFormat getFormat() const;

        unsigned int getMipmapCount() const;

        static int getBitsPerPixel(PixelFormat format);

        /**
         * Get the size of some pixel data in bits.
         */
        static int getDataSize(int width, int height, PixelFormat format);
    private:
        /**
         * Image pixel data.
         */
        void *m_data;

        /**
         * Image width.
         */
        int m_width;

        /**
         * Image height.
         */
        int m_height;

        /**
         * Image format.
         */
        PixelFormat m_format;

        /**
         * Image mipmap count.
         */
        unsigned int m_mipmapCount;
    };
}

#endif //NGINE_IMAGE_HPP
