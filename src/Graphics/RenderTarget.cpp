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

    RenderTarget::RenderTarget() {}

    RenderTarget::RenderTarget(const int width_, const int height_) : RenderTarget() {
        Width = width_;
        Height = height_;

        // Create framebuffer
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        InternalFramebuffer = std::make_shared<OpenGL::GLFramebuffer>(width_, height_);
#endif

        // Create texture
        _Texture = std::make_shared<Texture2D>();
        _Texture->Width = Width;
        _Texture->Height = Height;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        _Texture->InternalTexture = InternalFramebuffer->RenderTexture;
#endif
    }

    // Public Methods

    Texture2D *RenderTarget::GetTexture() {
        return _Texture.get();
    }

    bool RenderTarget::IsValid() const {
        if (InternalFramebuffer != nullptr)
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
            if (InternalFramebuffer->ID > 0)
#endif
                return true;
        return false;
    }

    void RenderTarget::Unload() {
        // Unload framebuffer
        Height = 0;
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        InternalFramebuffer = nullptr;
#endif
        Width = 0;
    }

    // Operators

    bool RenderTarget::operator==(const RenderTarget &b) const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        return InternalFramebuffer->ID == b.InternalFramebuffer->ID;
#endif
        return false;
    }

    bool RenderTarget::operator!=(const RenderTarget &b) const {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        return InternalFramebuffer->ID != b.InternalFramebuffer->ID;
#endif
        return true;
    }

}
