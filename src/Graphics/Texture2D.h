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

namespace NerdThings::Ngine::Graphics {
    /*
     * A 2D Texture stored in the GPU memory
     */
    struct NEAPI TTexture2D {
        // Public Fields

        /*
         * OpenGL Texture ID
         */
        unsigned int ID;

        /*
         * Texture width
         */
        int Width;

        /*
         * Texture Height
         */
        int Height;

        /*
         * Mipmap levels
         */
        int Mipmaps;

        /*
         * Data format
         */
        int Format;

        // Public Constructor(s)

        /*
         * Create a null texture
         */
        TTexture2D()
            : ID(0), Width(0), Height(0), Mipmaps(1), Format(0) {}

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
        #ifdef INCLUDE_RAYLIB

        /*
         * Convert to raylib texture
         */
        [[nodiscard]] Texture2D ToRaylibTex() const;

        /*
         * Convert from raylib texture
         */
        static std::shared_ptr<TTexture2D> FromRaylibTex(Texture2D tex_);

        #endif

        /*
         * Generate texture mipmaps
         */
        void GenerateMipmaps() const;

        // TODO: Add LoadTexture pixel data support
        //static TTexture2D LoadPixels()

        /*
         * Load a texture and get a pointer
         */
        static std::shared_ptr<TTexture2D> LoadTexture(const std::string &filename_);

        // Operators

        /*
         * Move a texture
         */
        TTexture2D &operator=(TTexture2D &&tex_) noexcept {
            ID = tex_.ID;
            Width = tex_.Width;
            Height = tex_.Height;
            Mipmaps = tex_.Mipmaps;
            Format = tex_.Format;

            tex_.ID = 0;
            tex_.Width = 0;
            tex_.Height = 0;
            tex_.Mipmaps = 0;
            tex_.Format = 0;

            return *this;
        }

        /*
         * Set the texture filter mode
         */
        void SetTextureFilter(ETextureFilterMode filterMode_) const;

        /*
         * Set the texture wrap mode
         */
        void SetTextureWrap(ETextureWrapMode wrapMode_) const;

        /*
         * Copy a texture (Reference, if one is deleted, both will stop working correctly.)
         * Use with caution.
         */
        TTexture2D &operator=(const TTexture2D &tex_) = default;

    private:
        // Private Constructor(s)

        TTexture2D(unsigned int id_, int width_, int height_, int mipmaps_, int format_)
            : ID(id_), Width(width_), Height(height_), Mipmaps(mipmaps_), Format(format_) {}
    };
}

#endif //TEXTURE2D_H
