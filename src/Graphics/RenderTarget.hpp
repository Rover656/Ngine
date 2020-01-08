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
        friend class GraphicsDevice;
    private:
        /**
         * The texture that the render target renders to.
         */
        Texture2D *m_texture;

#ifdef USE_EXPERIMENTAL_RENDERER
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        /**
         * Render target ID (OpenGL)
         */
        unsigned int m_ID = 0;

        /**
         * Depth buffer ID (OpenGL)
         *
         * @todo Do we need this, we don't use the Z parameter atm.
         */
        unsigned int m_depthBufferID = 0;
#endif
#endif
    public:
        /**
         * Render target width.
         */
        unsigned int Width = 0;

        /**
         * Render target height.
         */
        unsigned int Height = 0;


#ifndef USE_EXPERIMENTAL_RENDERER
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        /**
         * Internal framebuffer on the GPU
         */
        std::shared_ptr<OpenGL::GLFramebuffer> InternalFramebuffer = nullptr;
#endif
#endif
        /**
         * Create a render target.
         *
         * @param width_ Render target width.
         * @param height_ Render target height.
         */
        RenderTarget(GraphicsDevice *graphicsDevice_, unsigned int width_, unsigned int height_);
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
