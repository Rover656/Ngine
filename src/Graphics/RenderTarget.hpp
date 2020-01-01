/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef RENDERTARGET_HPP
#define RENDERTARGET_HPP

#include "../Config.hpp"

#include "../Resource.hpp"
#include "Texture2D.hpp"

#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
#include "OpenGL/Framebuffer.hpp"
#endif

namespace NerdThings::Ngine::Graphics {
    /**
     * A 2D Texture in the GPU that can be rendered to.
     */
    struct NEAPI RenderTarget : public IResource {
    private:
        /**
         * The texture that the render target renders to.
         */
        Texture2D *m_texture;
    public:
        /**
         * RenderBatched target width.
         */
        unsigned int Width = 0;

        /**
         * RenderBatched target height.
         */
        unsigned int Height = 0;

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        /**
         * Internal framebuffer on the GPU
         */
        std::shared_ptr<OpenGL::GLFramebuffer> InternalFramebuffer = nullptr;
#endif

        /**
         * Create a null render target.
         */
        RenderTarget();

        /**
         * Create a render target.
         *
         * @param width_ RenderBatched target width.
         * @param height_ RenderBatched target height.
         */
        RenderTarget(unsigned int width_, unsigned int height_);
        ~RenderTarget();

        /**
         * Get rendered texture.
         *
         * @return Get the texture that rendered to.
         */
        Texture2D *GetTexture();

        /**
         * Whether or not the render target is valid and usable.
         *
         * @return Whether the target is valid or not.
         */
        bool IsValid() const override;

        /**
         * Unload the render target.

         * @warning This will delete the texture too, ensure it is not going to be rendered this frame.
         */
        void Unload() override;

        bool operator==(const RenderTarget &b) const;
        bool operator!=(const RenderTarget &b) const;
    };
}

#endif //RENDERTARGET_HPP
