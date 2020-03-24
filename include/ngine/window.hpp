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

#ifndef NGINE_WINDOW_HPP
#define NGINE_WINDOW_HPP

#include "ngine/config.hpp"

#include "graphics/context_descriptor.hpp"
#include "graphics/graphics_device.hpp"

#include <string>

namespace ngine {
    /**
     * Window creation config.
     */
    struct WindowConfig {
        /**
         * Window context descriptor.
         */
        graphics::ContextDescriptor ContextDescriptor = graphics::ContextDescriptor::Default;

        /**
         * Window initial width.
         */
        int Width = 0;

        /**
         * Window initial height.
         */
        int Height = 0;

        /**
         * Window title.
         * @note Only works on Desktop.
         */
        std::string Title;
    };

    /**
     * A window.
     */
    class NEAPI Window {
    public:
        /**
         * Create a new window.
         */
        Window(WindowConfig config);

        /**
         * Destroy window.
         */
        ~Window();

        /**
         * Get the window's context descriptor.
         */
        const graphics::ContextDescriptor getContextDescriptor() const;

        /**
         * Poll window lifecycle events.
         */
        void pollEvents();

        /**
         * Whether or not the window is pending closure.
         */
        bool pendingClose();

        /**
         * Make this window current on the given thread.
         */
        void makeCurrent();

        /**
         * Swap the window's buffers.
         */
        void swapBuffers();

    private:
        /**
         * Window count.
         * For managing GLFW initialization and UWP restrictions.
         */
        static int m_windowCount;

        /**
         * Window handle.
         */
        void *m_handle = nullptr;

        /**
         * The window's context descriptor.
         */
        graphics::ContextDescriptor m_contextDescriptor;

        /**
         * The graphics device which control's the window context.
         */
        graphics::GraphicsDevice *m_graphicsDevice;

        /**
         * Window title.
         */
        std::string m_title;

        /**
         * Window width.
         */
        int m_windowWidth = 0;

        /**
         * Window height.
         */
        int m_windowHeight = 0;

        /**
         * Whether or not the window is ready.
         */
        bool m_initialized = false;
    };
};

#endif //NGINE_WINDOW_HPP