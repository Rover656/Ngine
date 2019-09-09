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

#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
#include "OpenGL/Framebuffer.h"
#include "OpenGL/OpenGL.h"
#endif

namespace NerdThings::Ngine::Graphics {
    // Render Target Related Private Fields

    std::vector<std::shared_ptr<TRenderTarget>> GraphicsManager::_RenderTargetStack;

    // General Rendering Fields

    unsigned int GraphicsManager::_CurrentHeight = 0;
    unsigned int GraphicsManager::_CurrentWidth = 0;

    // Render Target Related Private Methods

    void GraphicsManager::EndRenderTarget() {
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        // Force draw
        OpenGL::GL::Draw();

        // Stop using target
        OpenGL::GLFramebuffer::Unbind();

        // Setup framebuffer
        _CurrentWidth = Window::GetWidth();
        _CurrentHeight = Window::GetHeight();
        SetupFramebuffer();
#endif
    }

    void GraphicsManager::UseRenderTarget(std::shared_ptr<TRenderTarget> target_) {
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        // Force draw
        OpenGL::GL::Draw();

        // Use target
        target_->InternalFramebuffer->Bind();

        // Setup framebuffer
        _CurrentWidth = target_->Width;
        _CurrentHeight = target_->Height;
        SetupFramebuffer();
#endif
    }

    // General Rendering Methods

    unsigned int GraphicsManager::GetCurrentWidth() {
        return _CurrentWidth;
    }

    unsigned int GraphicsManager::GetCurrentHeight() {
        return _CurrentHeight;
    }

    void GraphicsManager::SetupFramebuffer() {
        if (_RenderTargetStack.empty()) {
            // We are rendering straight to the window, update width and height
            _CurrentWidth = Window::GetWidth();
            _CurrentHeight = Window::GetHeight();
        }

        // Set viewport
        OpenGL::GL::Viewport(0, 0, (float)_CurrentWidth, (float)_CurrentHeight);

        // Start new matrix with orthographic applied
        OpenGL::GL::MatrixMode(OpenGL::MATRIX_PROJECTION);
        OpenGL::GL::LoadIdentity();
        OpenGL::GL::Ortho(0, (float)_CurrentWidth, (float)_CurrentHeight, 0, -1, 1);
    }

    // Render Target Related Methods

    std::shared_ptr<TRenderTarget> GraphicsManager::PopTarget(bool &popped_) {
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
        return nullptr;
    }

    void GraphicsManager::PushTarget(std::shared_ptr<TRenderTarget> target_) {
        // Stop using current target
        if (!_RenderTargetStack.empty())
            EndRenderTarget();

        // Add to target stack
        _RenderTargetStack.emplace_back(target_);

        // Use the target
        UseRenderTarget(target_);
    }

    void GraphicsManager::ReplaceTarget(std::shared_ptr<TRenderTarget> old_, std::shared_ptr<TRenderTarget> new_) {
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
