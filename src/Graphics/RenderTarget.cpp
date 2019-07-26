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

    TRenderTarget::TRenderTarget(const int width_, const int height_) {
        Width = width_;
        Height = height_;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        InternalFramebuffer = std::make_shared<OpenGL::GLFramebuffer>(width_, height_);
#endif
    }

    TRenderTarget::TRenderTarget(TRenderTarget &&target_) {
        InternalFramebuffer = target_.InternalFramebuffer;
        Width = target_.Width;
        Height = target_.Height;

        target_.InternalFramebuffer = nullptr;
        target_.Width = 0;
        target_.Height = 0;
    }

    // Destructor

    TRenderTarget::~TRenderTarget() {
        // Unreference (smart pointer will delete in due time)
        InternalFramebuffer = nullptr;
    }

    // Public Methods

    TTexture2D TRenderTarget::GetTexture() {
        auto tex = TTexture2D();
        tex.Width = Width;
        tex.Height = Height;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        tex.InternalTexture = InternalFramebuffer->RenderTexture;
#endif
        return tex;
    }

    bool TRenderTarget::IsValid() {
        if (InternalFramebuffer != nullptr)
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
            if (InternalFramebuffer->ID > 0)
#endif
                return true;
        return false;
    }

    // Operators

    TRenderTarget &TRenderTarget::operator=(TRenderTarget &&target_) {
        InternalFramebuffer = target_.InternalFramebuffer;
        Width = target_.Width;
        Height = target_.Height;

        target_.InternalFramebuffer = nullptr;
        target_.Width = 0;
        target_.Height = 0;
        return *this;
    }

    bool TRenderTarget::operator==(const TRenderTarget &b) const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
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
