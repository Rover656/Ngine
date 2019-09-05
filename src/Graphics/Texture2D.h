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
    struct NEAPI TTexture2D {
        /*
         * The internal texture used by the GPU
         */
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        std::shared_ptr<OpenGL::GLTexture> InternalTexture;
#endif
        /*
         * Texture width
         */
        unsigned int Width;

        /*
         * Texture Height
         */
        unsigned int Height;

        // Public Constructor(s)

        /*
         * Create a null texture
         */
        TTexture2D()
            : InternalTexture(nullptr), Width(0), Height(0) {}

        /*
         * Move a texture
         */
        TTexture2D(TTexture2D &&tex_) noexcept;

        /*
         * Copy a texture (Reference, if one is deleted, both will stop working correctly.)
         * Use with caution.
         */
        TTexture2D(const TTexture2D &tex_) = default;

        // Destructor

        ~TTexture2D();

        // Public Methods

        /*
         * Get the number of mipmaps this texture has
         */
        int GetMipmapCount() const;

        /*
         * Is the texture valid and ready for use
         */
        bool IsValid() const;

        /*
         * Load a texture from pixel data.
         * This expects 8 bits for R, G, B and A.
         */
        static TTexture2D LoadPixels(unsigned int width_, unsigned height_, void *data_, int mipmapCount_ = 1);

        /*
         * Load a texture and get a pointer
         */
        static TTexture2D LoadTexture(const Filesystem::TPath &path_);

        /*
         * Set the texture filter mode
         */
        void SetTextureFilter(ETextureFilterMode filterMode_) const;

        /*
         * Set the texture wrap mode
         */
        void SetTextureWrap(ETextureWrapMode wrapMode_) const;

        // Operators

        bool operator==(const TTexture2D &tex_) const;

        bool operator!=(const TTexture2D &tex_) const;

        /*
         * Move a texture
         */
        TTexture2D &operator=(TTexture2D &&tex_) noexcept { // TODO: can this = default?
            InternalTexture = tex_.InternalTexture;
            Width = tex_.Width;
            Height = tex_.Height;

            tex_.InternalTexture = nullptr;
            tex_.Width = 0;
            tex_.Height = 0;

            return *this;
        }

        /*
         * Copy a texture
         */
        TTexture2D &operator=(const TTexture2D &tex_) = default;
    };
}

#endif //TEXTURE2D_H
