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

namespace Ngine::Types {
    /*
     * A 2D Texture stored in the GPU memory
     */
    struct TTexture2D {
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

        TTexture2D()
            : ID(0), Width(0), Height(0), Mipmaps(1), Format(0) {}

        // TODO: File load constructor?

        // Public Methods
        #ifdef INCLUDE_RAYLIB
        /*
         * Convert to raylib texture
         */
        Texture2D ToRaylibTex() const {
            return { ID, Width, Height, Mipmaps, Format };
        }

        /*
         * Convert from raylib texture
         */
        static TTexture2D FromRaylibTex(Texture2D tex) {
            TTexture2D ttex;

            ttex.ID = tex.id;
            ttex.Width = tex.width;
            ttex.Height = tex.height;
            ttex.Mipmaps = tex.mipmaps;
            ttex.Format = tex.format;

            return ttex;
        }
        #endif
    };
}

#endif //TEXTURE2D_H
