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

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "../Config.hpp"

#include "../filesystem/Filesystem.hpp"
#include "../Resource.hpp"

namespace ngine::graphics {
    /**
     * Image/Texture Pixel Format.
     *
     * @todo Need to eliminate/segregate any GL only formats.
     */
    enum PixelFormat {
        UNCOMPRESSED_GRAYSCALE = 1,
        UNCOMPRESSED_GRAY_ALPHA,
        UNCOMPRESSED_R5G6B5,
        UNCOMPRESSED_R8G8B8,
        UNCOMPRESSED_R5G5B5A1,
        UNCOMPRESSED_R4G4B4A4,
        UNCOMPRESSED_R8G8B8A8,
        UNCOMPRESSED_R32,
        UNCOMPRESSED_R32G32B32,
        UNCOMPRESSED_R32G32B32A32,
        COMPRESSED_DXT1_RGB,
        COMPRESSED_DXT1_RGBA,
        COMPRESSED_DXT3_RGBA,
        COMPRESSED_DXT5_RGBA,
        COMPRESSED_ETC1_RGB,
        COMPRESSED_ETC2_RGB,
        COMPRESSED_ETC2_EAC_RGBA,
        COMPRESSED_PVRT_RGB,
        COMPRESSED_PVRT_RGBA,
        COMPRESSED_ASTC_4x4_RGBA,
        COMPRESSED_ASTC_8x8_RGBA
    };

    /**
     * An image stored in CPU memory.
     */
    struct NEAPI Image final : public IResource {
    private:
        /**
         * Create the image with pixel data.
         *
         * @param pixelData The data to use.
         * @param width
         * @param height
         * @param format
         */
        void _create(unsigned char *pixelData, unsigned int width, unsigned int height, PixelFormat format);
    public:
        /**
         * Image width.
         */
        int Width = 0;

        /**
         * Image height.
         */
        int Height = 0;

        /**
         * Pixel format.
         */
        PixelFormat Format = UNCOMPRESSED_GRAYSCALE;

        /**
         * Image mipmap count.
         */
        int MipmapCount = 0;

        /**
         * The raw pixel data pointer.
         */
        unsigned char *PixelData = nullptr;

        /**
         * Empty image initializer.
         */
        Image();

        /**
         * Copy constructor.
         *
         * @param original The image to be copied
         */
        Image(const Image &original) = default;

        /**
         * Move constructor.
         *
         * @param old The old image.
         */
        Image(Image &&old);

        /**
         * Load an image file.
         *
         * @param path Image file to load.
         */
        Image(const filesystem::Path &path);

        /**
         * Create an image from raw pixel data.
         *
         * @note The pixel data will be copied.
         * @param pixelData The pixel date for the image.
         * @param width The image width.
         * @param height The image height.
         * @param format The image pixel format.
         */
        Image(unsigned char *pixelData, unsigned int width, unsigned int height, PixelFormat format);

        ~Image();

        /**
         * Test whether or not the image is valid
         *
         * @return Whether or not the image is valid.
         */
        bool isValid() const override;

        /**
         * Free image from memory.
         */
        void free() override;
    };
}

#endif //IMAGE_HPP
