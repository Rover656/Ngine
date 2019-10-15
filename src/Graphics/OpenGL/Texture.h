#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../Ngine.h"

namespace NerdThings::Ngine::Graphics::OpenGL {
    /*
     * Texture pixel format.
     * This is straight from rlgl.
     */
    enum GLPixelFormat {
        UNCOMPRESSED_GRAYSCALE = 1,     // 8 bit per pixel (no alpha)
        UNCOMPRESSED_GRAY_ALPHA,
        UNCOMPRESSED_R5G6B5,            // 16 bpp
        UNCOMPRESSED_R8G8B8,            // 24 bpp
        UNCOMPRESSED_R5G5B5A1,          // 16 bpp (1 bit alpha)
        UNCOMPRESSED_R4G4B4A4,          // 16 bpp (4 bit alpha)
        UNCOMPRESSED_R8G8B8A8,          // 32 bpp
        UNCOMPRESSED_R32,               // 32 bpp (1 channel - float)
        UNCOMPRESSED_R32G32B32,         // 32*3 bpp (3 channels - float)
        UNCOMPRESSED_R32G32B32A32,      // 32*4 bpp (4 channels - float)
        COMPRESSED_DXT1_RGB,            // 4 bpp (no alpha)
        COMPRESSED_DXT1_RGBA,           // 4 bpp (1 bit alpha)
        COMPRESSED_DXT3_RGBA,           // 8 bpp
        COMPRESSED_DXT5_RGBA,           // 8 bpp
        COMPRESSED_ETC1_RGB,            // 4 bpp
        COMPRESSED_ETC2_RGB,            // 4 bpp
        COMPRESSED_ETC2_EAC_RGBA,       // 8 bpp
        COMPRESSED_PVRT_RGB,            // 4 bpp
        COMPRESSED_PVRT_RGBA,           // 4 bpp
        COMPRESSED_ASTC_4x4_RGBA,       // 8 bpp
        COMPRESSED_ASTC_8x8_RGBA        // 2 bpp
    };

    /*
     * Texture filtering function
     * Use as a parameter value.
     */
    enum GLTextureFilterFunction {
        FILTER_FUNC_NEAREST = 0x2600,
        FILTER_FUNC_LINEAR = 0x2601,
        FILTER_FUNC_MIP_NEAREST = 0x2700,
        FILTER_FUNC_NEAREST_MIP_LINEAR = 0x2702,
        FILTER_FUNC_LINEAR_MIP_NEAREST = 0x2701,
        FILTER_FUNC_MIP_LINEAR = 0x2703,
    };

    /*
     * Texture parameters
     */
    enum GLTextureParameter {
        TEXPARAM_WRAP_S = 0x2802,
        TEXPARAM_WRAP_T = 0x2803,
        TEXPARAM_MAG_FILTER = 0x2800,
        TEXPARAM_MIN_FILTER = 0x2801,
        TEXPARAM_ANISOTROPIC_FILTER = 0x3000
    };

    /*
     * Texture wrap mode.
     * Use as a parameter value.
     */
    enum GLTextureWrap {
        /*
         * Repeats the texture
         */
        WRAP_REPEAT = 0x2901,

        /*
         * Coordinates will be clamped between 0 and 1
         */
        WRAP_CLAMP = 0x812F,

        /*
         * Repeats the texture but will be mirrored
         */
        WRAP_MIRROR_REPEAT = 0x8370,

        /*
         * GL_MIRROR_CLAMP_EXT (Unable to find documentation)
         */
        WRAP_MIRROR_CLAMP = 0x8742
    };

    /*
     * A texture held on the GPU
     */
    class NEAPI GLTexture {
        /*
         * The pixel format
         */
        GLPixelFormat _Format;
    public:
        // Public Fields

        /*
         * The OpenGL ID
         */
        unsigned int ID = 0;

        /*
         * The number of mipmaps used
         */
        int MipmapCount = 0;

        // Public Constructors

        /*
         * Create a null texture
         */
        GLTexture();

        /*
         * Create a texture stored on the GPU
         */
        GLTexture(unsigned int width_, unsigned int height_, void *data_, int mipmapCount_ = 1, GLPixelFormat format_ = UNCOMPRESSED_R8G8B8A8);

        // Destructor

        /*
         * Destruct and delete texture
         */
        ~GLTexture();

        // Public Methods

        /*
         * Bind the texture
         */
        void Bind();

        /*
         * Delete the texture
         */
        void Delete();

        /*
         * Set a texture parameter.
         */
        void SetParameter(GLTextureParameter param_, int value_);

        /*
         * Get the size of pixel data.
         */
        static int GetPixelDataSize(int width_, int height_, GLPixelFormat format_);
    };
}

#endif //TEXTURE_H
#endif
