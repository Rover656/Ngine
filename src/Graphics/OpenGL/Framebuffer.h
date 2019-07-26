#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../../ngine.h"

#include "Texture.h"

namespace NerdThings::Ngine::Graphics::OpenGL {
    class NEAPI GLFramebuffer {
        /*
         * The depth buffer ID.
         * rlgl avoided this on GL 3.3, but we kept it
         */
        unsigned int _DepthBufferID;

        /*
         * Whether or not a depth buffer is present
         */
        bool _HasDepthBuffer;
    public:

        /*
         * Framebuffer ID
         */
        unsigned int ID;

        /*
         * The attached render texture
         */
        std::shared_ptr<GLTexture> RenderTexture;

        /*
         * Create a new framebuffer
         */
        GLFramebuffer(int width_, int height_, bool useDepth_ = true, GLPixelFormat format_ = UNCOMPRESSED_R8G8B8A8);

        /*
         * Destruct and delete the framebuffer
         */
        ~GLFramebuffer();

        /*
         * Bind the framebuffer
         */
        void Bind();

        /*
         * Whether or not a depth buffer is present
         */
        bool HasDepthBuffer();

        /*
         * Unbind any bound framebuffer
         */
        static void Unbind();
    };
}

#endif //FRAMEBUFFER_H
