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

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../../Ngine.h"

#include "Texture.h"

namespace NerdThings::Ngine::Graphics::OpenGL {
    class NEAPI GLFramebuffer {
        /*
         * The depth buffer ID.
         * rlgl avoided this on GL 3.3, but we kept it
         */
        unsigned int _DepthBufferID = 0;

        /*
         * Whether or not a depth buffer is present
         */
        bool _HasDepthBuffer = false;
    public:

        /*
         * Framebuffer ID
         */
        unsigned int ID = 0;

        /*
         * The attached render texture
         */
        std::shared_ptr<GLTexture> RenderTexture = nullptr;

        /*
         * Create a new framebuffer
         */
        GLFramebuffer(int width_, int height_, bool useDepth_ = true, GLPixelFormat format_ = UNCOMPRESSED_R8G8B8A8);

        /*
         * Destruct and delete the framebuffer
         */
        ~GLFramebuffer();

        /*
         * Bind the framebuffer
         */
        void Bind();

        /*
         * Delete framebuffer
         */
        void Delete();

        /*
         * Whether or not a depth buffer is present
         */
        bool HasDepthBuffer();

        /*
         * Unbind any bound framebuffer
         */
        static void Unbind();
    };
}

#endif //FRAMEBUFFER_H
#endif
