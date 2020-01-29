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

#include "../Filesystem/Filesystem.hpp"
#include "../Resource.hpp"

namespace Ngine::Graphics {
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
    struct NEAPI Image : public IResource {
    private:
        /**
         * Create the image with pixel data.
         *
         * @param pixelData_ The data to use.
         * @param width_
         * @param height_
         * @param format_
         */
        void _create(unsigned char *pixelData_, unsigned int width_, unsigned int height_, PixelFormat format_);
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
         * @param original_ The image to be copied
         */
        Image(const Image &original_) = default;

        /**
         * Move constructor.
         *
         * @param old_ The old image.
         */
        Image(Image &&old_);

        /**
         * Load an image file.
         *
         * @param path_ Image file to load.
         */
        Image(const Filesystem::Path &path_);

        /**
         * Create an image from raw pixel data.
         *
         * @note The pixel data will be copied.
         * @param pixelData_ The pixel date for the image.
         * @param width_ The image width.
         * @param height_ The image height.
         * @param format_ The image pixel format.
         */
        Image(unsigned char *pixelData_, unsigned int width_, unsigned int height_, PixelFormat format_);

        ~Image();

        /**
         * Test whether or not the image is valid
         *
         * @return Whether or not the image is valid.
         */
        bool IsValid() const override;

        /**
         * Unload image from memory.
         */
        void Unload() override;
    };
}

#endif //IMAGE_HPP
