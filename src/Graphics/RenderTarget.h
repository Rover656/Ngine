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

#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
#include "OpenGL/Framebuffer.h"
#endif

namespace NerdThings::Ngine::Graphics {
    /*
     * A 2D Texture in the GPU that can be rendered to
     */
    struct NEAPI TRenderTarget {
        // Public Fields

        /*
         * Internal framebuffer on the GPU
         */
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        std::shared_ptr<OpenGL::GLFramebuffer> InternalFramebuffer;
#endif

        /*
         * Render target height
         */
        unsigned int Height;

        /*
         * Render target width
         */
        unsigned int Width;

        // Public Constructor(s)

        /*
         * Create a null render target
         */
        TRenderTarget()
            : InternalFramebuffer(nullptr) {}

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

        /*
         * Get rendered texture
         */
        TTexture2D GetTexture();

        /*
         * Whether or not the render target is valid and usable
         */
        bool IsValid();

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
