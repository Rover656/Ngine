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

#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "../ngine.h"

#include "Texture2D.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * A 2D Texture in the GPU that can be rendered to
     */
    struct NEAPI TRenderTarget {
        // Public Fields

        /*
         * OpenGL ID
         */
        unsigned int ID;

        /*
         * Texture buffer
         */
        TTexture2D Texture;

        /*
         * Depth buffer
         */
        TTexture2D DepthBuffer;

        /*
         * Is depth attachment is a texture or renderbuffer
         */
        bool DepthTexture;

        // Public Constructor(s)

        /*
         * Create a null render target
         */
        TRenderTarget()
            : ID(0), DepthTexture(false) {}

        /*
         * Create a render target
         */
        TRenderTarget(int width_, int height_);

        /*
         * Move a render target
         */
        TRenderTarget(TRenderTarget &&target_);

        /*
         * Copy a render target (reference)
         */
        TRenderTarget(const TRenderTarget &target_) = default;

        // Destructor

        ~TRenderTarget();

        // Public Methods

        #ifdef INCLUDE_RAYLIB

        RenderTexture2D ToRaylibTarget() const;

        static TRenderTarget FromRaylibTarget(RenderTexture2D target_);

        #endif

        // Operators

        /*
         * Move a render target
         */
        TRenderTarget &operator=(TRenderTarget &&target_);

        /*
         * Copy a render target (reference)
         */
        TRenderTarget &operator=(const TRenderTarget &target_) = default;

        bool operator==(const TRenderTarget &b) const;

        bool operator!=(const TRenderTarget &b) const;
    };
}

#endif //RENDERTARGET_H
