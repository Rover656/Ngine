#ifndef IMAGE_H
#define IMAGE_H

#include "../Ngine.h"

#include "../Filesystem/Filesystem.h"
#include "../Resource.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * Image/Texture Pixel Format.
     * Corresponds with OpenGL Abstraction values.
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

    /*
     * An image stored in CPU memory.
     */
    struct NEAPI Image : public IResource {
        // Public Fields

        /*
         * Pixel format
         */
        PixelFormat Format = UNCOMPRESSED_GRAYSCALE;

        /*
         * Image height
         */
        int Height = 0;

        /*
         * Image mipmaps
         */
        int Mipmaps = 0;

        /*
         * The raw pixel data pointer
         */
        unsigned char *PixelData = nullptr;

        /*
         * Image width
         */
        int Width = 0;

        // Public Constructors

        /*
         * Create a null image
         */
        Image();

        /*
         * Load an image file
         */
        Image(const Filesystem::Path &path_);

        /*
         * Create an image from raw pixel data.
         * The pixel data will be copied.
         */
        Image(unsigned char *pixelData_, int width_, int height_, PixelFormat format_);

        // Public Methods

        /*
         * Test whether or not the image is valid
         */
        bool IsValid() const override;

        /*
         * Load an image
         */
        static Image *LoadImage(const Filesystem::Path &path_);

        /*
         * Load raw pixel data.
         * The pixel data will be copied.
         */
        static Image *LoadPixels(unsigned char *pixelData_, int width_, int height_, PixelFormat format_);

        /*
         * Unload image from memory.
         */
        void Unload() override;
    };
}

#endif //IMAGE_H
