#ifndef IMAGE_H
#define IMAGE_H

#include "../ngine.h"

#include "../Filesystem/Filesystem.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * Image/Texture Pixel Format.
     * Corresponds with OpenGL Abstraction values.
     */
    enum EPixelFormat {
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
    struct NEAPI TImage {
        // Public Constructors

        /*
         * Create a null image
         */
        TImage();

        // Public Methods

        /*
         * Get image format.
         */
        EPixelFormat GetFormat();

        /*
         * Get image height
         */
        int GetHeight();

        /*
         * Get mipmap count
         */
        int GetMipmapCount();

        /*
         * Get pixel data
         */
        void *GetPixelData();

        /*
         * Get image width
         */
        int GetWidth();

        /*
         * Load an image
         */
        static TImage LoadImage(const Filesystem::TPath &path_);

        /*
         * Load raw pixel data.
         * The pixel data will be copied.
         */
        static TImage LoadPixels(unsigned char *pixelData_, int width_, int height_, EPixelFormat format_);

    private:
        // Private Structs

        /*
         * Contains the pixel data safely.
         */
        struct InternalDataContainer {
            /*
             * The raw pixel data pointer
             */
            void *PixelData = nullptr;

            ~InternalDataContainer();
        };

        // Private Fields

        /*
         * Pixel format
         */
        EPixelFormat _Format = UNCOMPRESSED_GRAYSCALE;

        /*
         * Image height
         */
        int _Height = 0;

        /*
         * Mipmap count
         */
        int _Mipmaps = 0;

        /*
         * Image pixel data
         */
        std::shared_ptr<InternalDataContainer> _PixelDataContainer = nullptr;

        /*
         * Image width
         */
        int _Width = 0;
    };
}

#endif //IMAGE_H
