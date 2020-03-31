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

#ifndef NGINE_GL_CONTEXT_HPP
#define NGINE_GL_CONTEXT_HPP

#include "ngine/config.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#include "ngine/window.hpp"

#if !defined(PLATFORM_UWP)
#define GLAD
#endif

#if defined(PLATFORM_DESKTOP)
#define GLFW
#endif

#if defined(PLATFORM_UWP)
#define EGL
#endif

#if defined(EGL)
typedef void* EGLDisplay;
typedef void* EGLSurface;
typedef void* EGLContext;
#endif

namespace ngine::platform::graphics::gl {
    // Do this for the sake of cleaner code
    using namespace ngine;
    using namespace ngine::graphics;

    // Forward declare
    class GLGraphicsDevice;

    /**
     * OpenGL context manager.
     * Handles utilisation of the GLFW and EGL contexts.
     */
    class NEAPI GLContext {
        friend class GLGraphicsDevice;
    public:
        /**
         * Make this context current.
         */
        void makeCurrent();

        /**
         * Swap the buffers.
         */
        void swapBuffers();
    private:
#if defined(EGL)
        /**
         * EGL Context
         */
        EGLContext m_context;

        /**
         * EGL Display
         */
        EGLDisplay m_display;

        /**
         * EGL Surface
         */
        EGLSurface m_surface;
#endif
        /**
         * Create an OpenGL Context
         */
        GLContext(IWindow *window);
        ~GLContext();

        /**
         * The window the OpenGL context is owned by.
         */
        IWindow *m_window = nullptr;

        /**
         * Whether or not GLAD has been initialized once.
         */
        static bool m_gladInit;

        /**
         * The context type GLAD was initialized for.
         * This stops cross-context use.
         */
        static ContextType m_gladInitContextType;
    };
}

#endif // defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#endif //NGINE_GL_CONTEXT_HPP
