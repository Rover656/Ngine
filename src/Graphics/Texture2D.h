/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "../Ngine.h"

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#include "OpenGL/Texture.h"
#endif

#include "../Filesystem/Filesystem.h"
#include "../Resource.h"
#include "Image.h"

namespace NerdThings::Ngine::Graphics {
    /**
     * Texture filter mode.
     */
    enum TextureFilterMode {
        /**
         * No filter, just approximation.
         */
        FILTER_POINT = 0,

        /**
         * Linear filtering.
         */
        FILTER_BILINEAR,

        /**
         * Trilinear filtering (with mipmaps).
         */
        FILTER_TRILINEAR,

        /**
         * Anisotropic filtering 4x.
         */
        FILTER_ANISOTROPIC_4X,

        /**
         * Anisotropic filtering 8x.
         */
        FILTER_ANISOTROPIC_8X,

        /**
         * Anisotropic filtering 16x.
         */
        FILTER_ANISOTROPIC_16X
    };

    /**
     * Texture wrap mode.
     */
    enum TextureWrapMode {
        /**
         * Repeats texture
         */
        WRAP_REPEAT = 0,

        /**
         * Clamps texture to edge pixel.
         */
        WRAP_CLAMP,

        /**
         * Mirrors and repeats the texture.
         */
        WRAP_MIRROR_REPEAT,

        /**
         * Mirrors and clamps to the border.
         */
        WRAP_MIRROR_CLAMP
    };

    /**
     * A 2D Texture stored in the GPU memory.
     */
    struct NEAPI Texture2D : public IResource {
        /**
         * Texture width
         */
        unsigned int Width = 0;

        /**
         * Texture Height
         */
        unsigned int Height = 0;

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        /**
         * Internal texture (OpenGL)
         */
        std::shared_ptr<OpenGL::GLTexture> InternalTexture = nullptr;
#endif

        /**
         * Create a null texture
         */
        Texture2D();

        /**
         * Copy constructor.
         */
        Texture2D(const Texture2D &original_) = default;

        /**
         * Create a texture from raw pixel data.
         *
         * @note The pixel data is copied.
         * @param data_ Pixel data.
         * @param width_ Width of the texture.
         * @param height_ Height of the texture.
         * @param format_ Data pixel format.
         * @param mipmapCount_ Number of mipmaps to generate.
         */
        Texture2D(unsigned char *data_, unsigned int width_, unsigned height_, PixelFormat format_ = UNCOMPRESSED_R8G8B8A8, int mipmapCount_ = 1);

        /**
         * Load a texture file.
         *
         * @param path_ Texture file to load.
         */
        Texture2D(const Filesystem::Path &path_);

        /**
         * Create a texture from an image.
         *
         * @param img_ Image to load onto GPU.
         */
        Texture2D(const Image *img_);

        /**
         * Get the number of mipmaps this texture has
         *
         * @return The number of mipmaps generated.
         */
        int GetMipmapCount() const;

        /**
         * Is the texture valid and ready for use.
         *
         * @return Whether or not the texture is valid.
         */
        bool IsValid() const override;

        /**
         * Set the texture filter mode.
         *
         * @param filterMode_ The filter mode to set.
         */
        void SetTextureFilter(TextureFilterMode filterMode_) const;

        /**
         * Set the texture wrap mode.
         *
         * @param wrapMode_ The texture wrap mode.
         */
        void SetTextureWrap(TextureWrapMode wrapMode_) const;

        /**
         * Unload the texture.
         */
        void Unload() override;

        bool operator==(const Texture2D &tex_) const;
        bool operator!=(const Texture2D &tex_) const;

        /*
         * Copy a texture
         */
        Texture2D &operator=(const Texture2D &tex_) = default;
    };
}

#endif //TEXTURE2D_H
