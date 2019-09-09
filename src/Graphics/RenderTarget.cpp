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

#include "RenderTarget.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    TRenderTarget::TRenderTarget() {}

    TRenderTarget::TRenderTarget(const int width_, const int height_) : TRenderTarget() {
        Width = width_;
        Height = height_;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        InternalFramebuffer = std::make_shared<OpenGL::GLFramebuffer>(width_, height_);
#endif
    }

    // Public Methods

    std::shared_ptr<TTexture2D> TRenderTarget::GetTexture() {
        auto tex = std::make_shared<TTexture2D>();
        tex->Width = Width;
        tex->Height = Height;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        tex->InternalTexture = InternalFramebuffer->RenderTexture;
#endif
        return tex;
    }

    bool TRenderTarget::IsValid() const {
        if (InternalFramebuffer != nullptr)
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
            if (InternalFramebuffer->ID > 0)
#endif
                return true;
        return false;
    }

    void TRenderTarget::Unload() {
        // Unload framebuffer
        Height = 0;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        InternalFramebuffer->Delete();
#endif
        Width = 0;
    }

    // Operators

    bool TRenderTarget::operator==(const TRenderTarget &b) const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        return InternalFramebuffer->ID == b.InternalFramebuffer->ID;
#endif
        return false;
    }

    bool TRenderTarget::operator!=(const TRenderTarget &b) const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        return InternalFramebuffer->ID != b.InternalFramebuffer->ID;
#endif
        return true;
    }

}
