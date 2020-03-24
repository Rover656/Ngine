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

#ifndef NGINE_OPENGL_GRAPHICS_DEVICE_HPP
#define NGINE_OPENGL_GRAPHICS_DEVICE_HPP

#include "ngine/config.hpp"

#include "../graphics_device.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

namespace ngine::graphics::platform {
    /**
     * Graphics device for OpenGL(ES) platform.
     */
    class NEAPI OpenGLGraphicsDevice : public GraphicsDevice {
        friend class Window;
    private:
        OpenGLGraphicsDevice(Window *window);
        ~OpenGLGraphicsDevice();

        /**
         * Whether or not the context is GLES2.
         */
        bool m_isGLES2 = false;

        /**
         * Whether or not the context is GLES3.
         */
        bool m_isGLES3 = false;
    };
}

#endif // defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#endif //NGINE_OPENGL_GRAPHICS_DEVICE_HPP
