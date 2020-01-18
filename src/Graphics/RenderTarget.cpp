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

#include "RenderTarget.hpp"

#include "../Console.hpp"

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#include "OpenGL.hpp"
#endif

namespace NerdThings::Ngine::Graphics {
    RenderTarget::RenderTarget(GraphicsDevice *graphicsDevice_, unsigned int width_, unsigned int height_) {
        Width = width_;
        Height = height_;

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
#ifdef USE_EXPERIMENTAL_RENDERER
        // Create color attachment
        m_texture = new Texture2D(graphicsDevice_, nullptr, width_, height_, UNCOMPRESSED_R8G8B8A8, 1);

        // Verify texture
        if (!m_texture->IsValid()) {
            Console::Error("RenderTarget", "Failed to create color attachment for render target.");

            // Delete texture
            delete m_texture;
            m_texture = nullptr;

            // Unset dimensions
            Width = 0;
            Height = 0;
            return;
        }

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Depth buffer
        glGenRenderbuffers(1, &m_depthBufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width_, height_);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Create FBO
        glGenFramebuffers(1, &m_ID);

        // Bind
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        // Set depth and color attachment
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->m_ID, 0);

        // Check framebuffer status
        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            Console::Error("RenderTarget", "Failed to create framebuffer.");

            // Unbind
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Unset dimensions
            Width = 0;
            Height = 0;

            // Delete texture
            delete m_texture;
            m_texture = nullptr;

            // Delete depth buffer
            glDeleteRenderbuffers(1, &m_depthBufferID);
            m_depthBufferID = 0;

            // Delete framebuffer
            glDeleteFramebuffers(1, &m_ID);
            m_ID = 0;
            return;
        }

        Console::Notice("RenderTarget", "Successfully created framebuffer with ID %i", m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
#else
        // Create framebuffer
        InternalFramebuffer = std::make_shared<OpenGL::GLFramebuffer>(width_, height_);

        // Create texture
        m_texture = new Texture2D();
        m_texture->Width = Width;
        m_texture->Height = Height;
        m_texture->InternalTexture = InternalFramebuffer->RenderTexture;
#endif
#endif
    }

    RenderTarget::~RenderTarget() {
        // Delete self
        Unload();
    }

    Texture2D *RenderTarget::GetTexture() {
        return m_texture;
    }

    bool RenderTarget::IsValid() const {
#ifdef USE_EXPERIMENTAL_RENDERER
        if (m_ID > 0)
            return true;
#else
        if (InternalFramebuffer != nullptr)
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
            if (InternalFramebuffer->ID > 0)
#endif
#endif
                return true;
        return false;
    }

    void RenderTarget::Unload() {
        Width = 0;
        Height = 0;
#ifdef USE_EXPERIMENTAL_RENDERER
        // Delete texture
        delete m_texture;
        m_texture = nullptr;

        // Delete depth buffer
        if (m_depthBufferID > 0) {
            glDeleteRenderbuffers(1, &m_depthBufferID);
            m_depthBufferID = 0;
        }

        // Delete framebuffer
        if (m_ID > 0) {
            glDeleteFramebuffers(1, &m_ID);
            m_ID = 0;
        }
#else
        // Unload framebuffer
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        InternalFramebuffer = nullptr;
#endif
#endif
    }

    bool RenderTarget::operator==(const RenderTarget &b) const {
#ifdef USE_EXPERIMENTAL_RENDERER
        return m_ID == b.m_ID;
#else
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
        return InternalFramebuffer->ID == b.InternalFramebuffer->ID;
#endif
#endif
        return false;
    }

    bool RenderTarget::operator!=(const RenderTarget &b) const {
#ifdef USE_EXPERIMENTAL_RENDERER
        return m_ID != b.m_ID;
#else
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        return InternalFramebuffer->ID != b.InternalFramebuffer->ID;
#endif
#endif
        return true;
    }

}
