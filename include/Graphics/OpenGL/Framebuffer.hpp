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

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../../Config.hpp"

#include "Texture.hpp"

namespace NerdThings::Ngine::Graphics::OpenGL {
    class NEAPI GLFramebuffer {
        /*
         * The depth buffer ID.
         * rlgl avoided this on GL 3.3, but we kept it
         */
        unsigned int _DepthBufferID = 0;

        /*
         * Whether or not a depth buffer is present
         */
        bool _HasDepthBuffer = false;
    public:

        /*
         * Framebuffer ID
         */
        unsigned int ID = 0;

        /*
         * The attached render texture
         */
        std::shared_ptr<GLTexture> RenderTexture = nullptr;

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
         * Delete framebuffer
         */
        void Delete();

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
#endif
