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

#ifndef GRAPHICSDEVICE_HPP
#define GRAPHICSDEVICE_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "Color.hpp"
#include "Image.hpp"

namespace ngine {
    class Window;

    namespace graphics {
        class Renderer;

        struct RenderTarget;

        namespace API {
            class PlatformGraphicsAPI;
        }

        /**
         * This defines the available graphics APIs in Ngine.
         */
        enum class GraphicsAPI {
            /**
             * Select the default API for the current platform.
             */
            Default = 0,

            /**
             * The OpenGL API.
             *
             * Supported versions:
             * * 2.X
             * * 3.X
             *
             * Supported on:
             * * Desktop
             */
            OpenGL,

            /**
             * The OpenGL ES API.
             *
             * Supported versions:
             * * 2.X
             *
             * Supported on:
             * * Desktop (Windows Only)
             * * UWP
             *
             * @warning Use of this on Desktop and UWP will be deprecated once DirectX11 API is added as ANGLE will be obsolete. This will be left purely for GLES support for possible other platforms.
             */
            OpenGLES,

            /**
             * The DirectX API.
             *
             * Placeholder. Not implemented yet, will throw if selected.
             *
             * Supported on:
             * * Desktop (Windows Only)
             * * UWP
             */
            DirectX
        };

        /**
         * The graphics device.
         */
        class NEAPI GraphicsDevice {
            friend class Renderer;

            friend class ngine::Window;

            /**
             * Whether or not the target is accessed.
             * Once accessed, it is used and therefore can no longer change.
             */
            static bool m_targetAccessed;
        public:
            /**
             * Maximum targets in a stack.
             */
            static const int MAX_TARGETS = 32;

            /**
             * The maximum number of matrices permitted per stack.
             */
            static const int MAX_MATRICES = 32;
        private:
            /**
             * The current platform API.
             */
            API::PlatformGraphicsAPI *m_platformAPI;

            /**
             * The window this graphics device is attached to.
             */
            Window *m_attachedWindow;

            /**
             * List of all attached renderers.
             */
            std::vector<Renderer *> m_attachedRenderers;

            /**
             * The current projection matrix.
             */
            Matrix m_projectionMatrix;

            /**
             * The render target stack.
             */
            RenderTarget *m_targetStack[MAX_TARGETS];

            /**
             * The render target stack counter.
             */
            int m_targetCounter = 0;

            Matrix m_viewMatrixStack[MAX_MATRICES];

            int m_viewCounter = 0;

            /**
             * Create a graphics device.
             */
            explicit GraphicsDevice(Window *window);

            ~GraphicsDevice();

        public:

            /**
             * Get the window attached to this device.
             */
            Window *getWindow();

            /**
             * Clear the current framebuffer with the given color.
             */
            void clear(Color color);

            /**
             * Get the current render target.
             *
             * @return The current render target or null if rendering straight to window.
             */
            RenderTarget *getCurrentTarget();

            /**
             * Set the current render target.
             *
             * @warning Do not provide null, use `PopTarget` instead.
             * @warning This will force all renderers to draw.
             * @param target The new target.
             */
            void pushTarget(RenderTarget *target);

            /**
             * Pop the current target off of the stack.
             *
             * @warning This will force all renderers to draw
             */
            void popTarget();

            /**
             * Get the current projection matrix.
             *
             * @return The current projection matrix.
             */
            Matrix getProjectionMatrix() const;

            /**
             * Get the current model view matrix.
             *
             * @note This is basically just for camera support.
             * @return The current model view matrix.
             */
            Matrix getViewMatrix() const;

            /**
             * Push a view matrix onto the stack.
             */
            void pushViewMatrix();

            /**
             * Pop a view matrix off the stack.
             */
            void popViewMatrix();

            /**
             * Load identity onto the current view matrix.
             */
            void loadViewIdentity();

            /**
             * Multiply the current matrix.
             *
             * @param matrix Multiplier.
             */
            void multView(const Matrix &matrix);

            /**
             * Setup the framebuffer.
             *
             * This will create an orthographic projection matrix.
             * This will be created for the framebuffer if there is no render target attached, otherwise it will be for the render target.
             */
            void setupFramebuffer();

            /**
             * Get the graphics API.
             *
             * @warning This is intended for internal use, please use the classes provided by the `graphics` namespace instead.
             */
            API::PlatformGraphicsAPI *getAPI();
        };
    }
}

#endif // GRAPHICSDEVICE_HPP
