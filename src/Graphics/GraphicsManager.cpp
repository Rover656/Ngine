/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#include "Graphics/GraphicsManager.hpp"

#include "Window.hpp"

#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
#include "Graphics/OpenGL/Framebuffer.hpp"
#include "Graphics/OpenGL/OpenGL.hpp"
#endif

namespace NerdThings::Ngine::Graphics {
    std::vector<RenderTarget *> GraphicsManager::m_renderTargetStack;
    unsigned int GraphicsManager::m_currentHeight = 0;
    unsigned int GraphicsManager::m_currentWidth = 0;

    void GraphicsManager::_useRenderTarget(RenderTarget *target_) {
#ifdef USE_EXPERIMENTAL_RENDERER
        throw std::runtime_error("GraphicsManager is no longer available. Use GraphicsDevice instead.");
#else
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        // Force draw
        OpenGL::GL::Draw();

        // Use target
        target_->InternalFramebuffer->Bind();

        // Setup framebuffer
        m_currentWidth = target_->Width;
        m_currentHeight = target_->Height;
        SetupFramebuffer();
#endif
#endif
    }

    void GraphicsManager::_endRenderTarget() {
#ifdef USE_EXPERIMENTAL_RENDERER
        throw std::runtime_error("GraphicsManager is no longer available. Use GraphicsDevice instead.");
#else
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        // Force draw
        OpenGL::GL::Draw();

        // Stop using target
        OpenGL::GLFramebuffer::Unbind();

        // Setup framebuffer
        m_currentWidth = Window::GetCurrent()->GetWidth();
        m_currentHeight = Window::GetCurrent()->GetHeight();
        SetupFramebuffer();
#endif
#endif
    }

    unsigned int GraphicsManager::GetCurrentWidth() {
        return m_currentWidth;
    }

    unsigned int GraphicsManager::GetCurrentHeight() {
        return m_currentHeight;
    }

    void GraphicsManager::SetupFramebuffer() {
#ifdef USE_EXPERIMENTAL_RENDERER
        throw std::runtime_error("GraphicsManager is no longer available. Use GraphicsDevice instead.");
#endif
        if (m_renderTargetStack.empty()) {
            // We are rendering straight to the window, update width and height
            m_currentWidth = Window::GetCurrent()->GetWidth();
            m_currentHeight = Window::GetCurrent()->GetHeight();
        }

        // Set viewport
        OpenGL::GL::Viewport(0, 0, m_currentWidth, m_currentHeight);

        // Start new matrix with orthographic applied
        OpenGL::GL::MatrixMode(OpenGL::MATRIX_PROJECTION);
        OpenGL::GL::LoadIdentity();
        OpenGL::GL::Ortho(0, (float)m_currentWidth, (float)m_currentHeight, 0, -1, 1);
    }

    void GraphicsManager::PushTarget(RenderTarget *target_) {
#ifdef USE_EXPERIMENTAL_RENDERER
        throw std::runtime_error("GraphicsManager is no longer available. Use GraphicsDevice instead.");
#endif
        // Stop using current target
        if (!m_renderTargetStack.empty())
            _endRenderTarget();

        // Add to target stack
        m_renderTargetStack.emplace_back(target_);

        // Use the target
        _useRenderTarget(target_);
    }

    void GraphicsManager::PopTarget() {
#ifdef USE_EXPERIMENTAL_RENDERER
        throw std::runtime_error("GraphicsManager is no longer available. Use GraphicsDevice instead.");
#endif
        if (!m_renderTargetStack.empty()) {
            // Remove target
            m_renderTargetStack.pop_back();

            // Stop Render Target
            _endRenderTarget();

            // Start using another if it is available
            if (!m_renderTargetStack.empty()) {
                _useRenderTarget(m_renderTargetStack.back());
            }
        }
    }

    void GraphicsManager::ReplaceTarget(RenderTarget *old_, RenderTarget *new_) {
#ifdef USE_EXPERIMENTAL_RENDERER
        throw std::runtime_error("GraphicsManager is no longer available. Use GraphicsDevice instead.");
#endif
        const auto oldPos = std::find(m_renderTargetStack.begin(), m_renderTargetStack.end(), old_) - m_renderTargetStack.
            begin();

        // If this is the currently active target, replace it too
        if (oldPos == m_renderTargetStack.size() - 1) {
            _endRenderTarget();
            _useRenderTarget(new_);
        }

        // Send to stack
        m_renderTargetStack[oldPos] = new_;
    }
}
