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
    class NEAPI IWindow {
    public:
        /**
         * Destroy window.
         */
        virtual ~IWindow();

        /**
         * Create a new window.
         * @param config The window config.
         */
        static IWindow *createWindow(WindowConfig config);

        /**
         * Get the window's context descriptor.
         */
        const graphics::ContextDescriptor getContextDescriptor() const;

        /**
         * Get the window graphics device.
         */
        graphics::IGraphicsDevice *getGraphicsDevice();

        /**
         * Get the window graphics device.
         */
        const graphics::IGraphicsDevice *getGraphicsDevice() const;

        /**
         * Get window width.
         */
        float getWidth() const;

        /**
         * Get window height.
         */
        float getHeight() const;

        /**
         * Poll window lifecycle events.
         */
        virtual void pollEvents() = 0;

        /**
         * Whether or not the window is pending closure.
         */
        virtual bool shouldClose() const = 0;

        /**
         * Get the window handle.
         * @warning Will return null on platforms that don't have a window handle (e.g. UWP). Use the platform's way instead.
         */
        virtual void *getHandle() const = 0;

    protected:
        /**
         * Create a new window.
         */
        IWindow(WindowConfig config);

        /**
         * Window count.
         * For managing GLFW initialization and UWP restrictions.
         */
        static int m_windowCount;

        /**
         * The window's context descriptor.
         */
        graphics::ContextDescriptor m_contextDescriptor;

        /**
         * The graphics device which control's the window context.
         */
        graphics::IGraphicsDevice *m_graphicsDevice;

        /**
         * Window title.
         */
        std::string m_title;

        /**
         * Window width.
         * @note Should be updated by implementation in pollEvents() stage.
         */
        int m_windowWidth = 0;

        /**
         * Window height.
         * @note Should be updated by implementation in pollEvents() stage.
         */
        int m_windowHeight = 0;

        /**
         * Verify the context descriptor.
         */
        void _verifyContextDescriptor();

        /**
         * Create the graphics device for this window.
         */
        void _createGraphicsDevice();

        /**
         * Check if the window changed size, if it did, we tell the graphics device.
         */
        void _checkResized(int oldWidth, int oldHeight);
    };
};

#endif //NGINE_WINDOW_HPP