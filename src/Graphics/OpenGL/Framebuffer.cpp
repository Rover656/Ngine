#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#include "Framebuffer.h"

// Platform specifics
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#include <glad/glad.h>
#elif defined(GRAPHICS_OPENGLES2)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1 // UWP???
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#endif

#if defined(PLATFORM_DESKTOP)
#include <GLFW/glfw3.h>
#elif defined(PLATFORM_UWP)
#include <angle_windowsstore.h>
#endif

namespace NerdThings::Ngine::Graphics::OpenGL {
    GLFramebuffer::GLFramebuffer(int width_, int height_, bool useDepth_, GLPixelFormat format_) {
        // Check inputs
        if (format_ >= COMPRESSED_DXT1_RGB)
            throw std::runtime_error("Cannot use a compressed pixel format.");

        // Check if we use the depth texture
        if (useDepth_) _HasDepthBuffer = true;

        // Color attachment
        RenderTexture = std::make_shared<GLTexture>(width_, height_, nullptr, 1, UNCOMPRESSED_R8G8B8A8);

        // Verify, stopping here to save wasting of more time
        if (RenderTexture->ID <= 0) {
            // Message about failure
            ConsoleMessage("Framebuffer failed to create a texture attachment.", "WARN", "GLFramebuffer");

            // Delete
            RenderTexture = nullptr;

            // Stop
            return;
        }

        // Set parameters
        glBindTexture(GL_TEXTURE_2D, 0);

        // Depth buffer
        glGenRenderbuffers(1, &_DepthBufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, _DepthBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width_, height_);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Create FBO
        glGenFramebuffers(1, &ID);
        Bind();

        // Set depth attachment
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _DepthBufferID);

        // Set color attachment
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RenderTexture->ID, 0);

        // Check this worked
        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            // Message about failure
            ConsoleMessage("Failed to create framebuffer.", "WARN", "GLFramebuffer");

            // Unbind framebuffer
            Unbind();

            // Delete texture
            RenderTexture = nullptr;

            // Delete depth buffer
            glDeleteRenderbuffers(1, &_DepthBufferID);

            // Delete framebuffer
            glDeleteFramebuffers(1, &ID);

            // Set ID to 0
            ID = 0;

            // Finished
            return;
        }
        ConsoleMessage("Successfully created framebuffer with ID " + std::to_string(ID) + ".", "NOTICE", "GLFramebuffer");

        // Unbind framebuffer
        Unbind();
    }

    GLFramebuffer::~GLFramebuffer() {
        Delete();
    }

    void GLFramebuffer::Bind() {
        // Bind
        glBindFramebuffer(GL_FRAMEBUFFER, ID);
    }

    void GLFramebuffer::Delete() {
        if (RenderTexture != nullptr && RenderTexture->ID > 0) {
            // Delete texture
            RenderTexture->Delete();
            RenderTexture = nullptr;
        }

        if (_DepthBufferID > 0) {
            // Delete depth buffer
            glDeleteRenderbuffers(1, &_DepthBufferID);

            // Set ID to 0
            _DepthBufferID = 0;
        }

        if (ID > 0) {
            // Delete framebuffer
            glDeleteFramebuffers(1, &ID);

            // Set ID to 0
            ID = 0;
        }
    }

    void GLFramebuffer::Unbind() {
        // Unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
#endif
