/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "../ngine.h"

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#include "OpenGL/Texture.h"
#endif

#include "../Filesystem/Filesystem.h"
#include "../Resource.h"
#include "Image.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * Texture filter mode
     */
    enum ETextureFilterMode {
        /*
         * No filter, just approximation
         */
        FILTER_POINT = 0,

        /*
         * Linear filtering
         */
        FILTER_BILINEAR,

        /*
         * Trilinear filtering (with mipmaps)
         */
        FILTER_TRILINEAR,

        /*
         * Anisotropic filtering 4x
         */
        FILTER_ANISOTROPIC_4X,

        /*
         * Anisotropic filtering 8x
         */
        FILTER_ANISOTROPIC_8X,

        /*
         * Anisotropic filtering 16x
         */
        FILTER_ANISOTROPIC_16X
    };

    /*
     * Texture wrap mode
     */
    enum ETextureWrapMode {
        /*
         * Repeats texture
         */
        WRAP_REPEAT = 0,

        /*
         * Clamps texture to edge pixel
         */
        WRAP_CLAMP,

        /*
         * Mirrors and repeats the texture
         */
        WRAP_MIRROR_REPEAT,

        /*
         * Mirrors and clamps to the border
         */
        WRAP_MIRROR_CLAMP
    };

    /*
     * A 2D Texture stored in the GPU memory
     */
    struct NEAPI TTexture2D : public TResource {
        // Public Fields

        /*
         * Texture Height
         */
        unsigned int Height = 0;

        /*
         * Internal texture (Graphics API dependant)
         */
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        std::shared_ptr<OpenGL::GLTexture> InternalTexture = nullptr;
#endif

        /*
         * Texture width
         */
        unsigned int Width = 0;

        // Public Constructor(s)

        /*
         * Create a null texture
         */
        TTexture2D();

        /*
         * Create a texture from raw pixel data.
         */
        TTexture2D(unsigned char *data_, unsigned int width_, unsigned height_, EPixelFormat format_ = UNCOMPRESSED_R8G8B8A8, int mipmapCount_ = 1);

        /*
         * Load a texture file.
         */
        TTexture2D(const Filesystem::TPath &path_);

        /*
         * Create a texture from an image.
         */
        TTexture2D(const std::shared_ptr<TImage> &img_);

        // Public Methods

        /*
         * Create a texture from an image.
         */
        static std::shared_ptr<TTexture2D> FromImage(const std::shared_ptr<TImage> &img_);

        /*
         * Get the number of mipmaps this texture has
         */
        int GetMipmapCount() const;

        /*
         * Is the texture valid and ready for use
         */
        bool IsValid() const override;

        /*
         * Load a texture from pixel data.
         * This expects 8 bits for R, G, B and A.
         */
        static std::shared_ptr<TTexture2D> LoadPixels(unsigned char *data_, unsigned int width_, unsigned height_, EPixelFormat format_ = UNCOMPRESSED_R8G8B8A8, int mipmapCount_ = 1);

        /*
         * Load a texture and get a pointer
         */
        static std::shared_ptr<TTexture2D> LoadTexture(const Filesystem::TPath &path_);

        /*
         * Set the texture filter mode
         */
        void SetTextureFilter(ETextureFilterMode filterMode_) const;

        /*
         * Set the texture wrap mode
         */
        void SetTextureWrap(ETextureWrapMode wrapMode_) const;

        /*
         * Unload the texture
         */
        void Unload() override;

        // Operators

        bool operator==(const TTexture2D &tex_) const;

        bool operator!=(const TTexture2D &tex_) const;

        /*
         * Copy a texture
         */
        TTexture2D &operator=(const TTexture2D &tex_) = default;
    };
}

#endif //TEXTURE2D_H
