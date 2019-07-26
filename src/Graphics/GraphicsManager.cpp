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

#include "../Window.h"
#include "GraphicsManager.h"

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
#include "OpenGL/Framebuffer.h"
#include "OpenGL/OpenGL.h"
#endif

namespace NerdThings::Ngine::Graphics {
    // Render Target Related Private Fields

    std::vector<TRenderTarget> GraphicsManager::_RenderTargetStack;

    // Render Target Related Private Methods

    void GraphicsManager::EndRenderTarget() {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        // Force draw
        OpenGL::GL::Draw();

        // Stop using target
        OpenGL::GLFramebuffer::Unbind();

        // Set viewport
        OpenGL::GL::Viewport(0, 0, Window::GetWidth(), Window::GetHeight());
#endif
    }

    void GraphicsManager::UseRenderTarget(const TRenderTarget &target_) {
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        // Use target
        target_.InternalFramebuffer->Bind();

        // Force draw
        OpenGL::GL::Draw();

        // Stop using target
        OpenGL::GLFramebuffer::Unbind();

        // Set viewport
        OpenGL::GL::Viewport(0, 0, target_.Width, target_.Height);
#endif
    }

    // Render Target Related Methods

    TRenderTarget GraphicsManager::PopTarget(bool &popped_) {
        if (!_RenderTargetStack.empty()) {
            // Get target
            auto pop = _RenderTargetStack.back();

            // Remove target
            _RenderTargetStack.pop_back();

            // Stop Render Target
            EndRenderTarget();

            // Start using another if it is available
            if (!_RenderTargetStack.empty()) {
                //BeginTextureMode(_RenderTargetStack.back());
                UseRenderTarget(_RenderTargetStack.back());
            }

            popped_ = true;
            return pop;
        }

        popped_ = false;
        return TRenderTarget();
    }

    void GraphicsManager::PushTarget(const TRenderTarget &target_) {
        // Stop using current target
        if (!_RenderTargetStack.empty())
            EndRenderTarget();

        // Add to target stack
        _RenderTargetStack.emplace_back(target_);

        // Use the target
        UseRenderTarget(target_);
    }

    void GraphicsManager::ReplaceTarget(const TRenderTarget &old_, const TRenderTarget &new_) {
        const auto oldPos = std::find(_RenderTargetStack.begin(), _RenderTargetStack.end(), old_) - _RenderTargetStack.
            begin();

        // If this is the currently active target, replace it too
        if (oldPos == _RenderTargetStack.size() - 1) {
            EndRenderTarget();
            UseRenderTarget(new_);
        }

        // Send to stack
        _RenderTargetStack[oldPos] = new_;
    }
}
