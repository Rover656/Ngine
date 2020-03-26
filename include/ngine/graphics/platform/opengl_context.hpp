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

#ifndef NGINE_OPENGL_CONTEXT_HPP
#define NGINE_OPENGL_CONTEXT_HPP

#include "ngine/config.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#include "../../window.hpp"

namespace ngine::graphics::platform {
    /**
     * OpenGL context manager.
     * Handles utilisation of the GLFW and EGL contexts.
     */
    class NEAPI OpenGLContext {
    public:
        /**
         * Create an OpenGL Context
         */
        OpenGLContext(Window *window);
        ~OpenGLContext();

        /**
         * Make this context current.
         */
        //void makeCurrent();

        /**
         * Swap the buffers.
         */
        void swapBuffers();
    private:
        /**
         * The window the OpenGL context is owned by.
         */
        Window *m_window = nullptr;

        /**
         * Whether or not GLAD has been initialized once.
         */
        static bool m_gladInit;
    };
}

#endif // defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#endif //NGINE_OPENGL_CONTEXT_HPP
