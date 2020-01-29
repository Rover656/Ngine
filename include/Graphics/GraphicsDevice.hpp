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
#include "../Window.hpp"
#include "Image.hpp"

namespace Ngine::Graphics {
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
         * @todo Use of this on Desktop and UWP will be deprecated once DirectX11 API is added as ANGLE will be obsolete. This will be left purely for GLES support for possible other platforms.
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
     * Manages the OpenGL Context and it's features.
     * Once feature complete this will replace `GraphicsManager`.
     *
     * @todo Remove all 'GL' things, this will be controlled by platform APIs instead.
     */
    class NEAPI GraphicsDevice {
        friend class Renderer;
        friend class Window;

        /**
         * The target graphics API.
         */
        static GraphicsAPI m_targetAPI;

        /**
         * The default target API major version.
         */
        static int m_targetMajorVersion;

        /**
         * The default target API minor version.
         */
        static int m_targetMinorVersion;

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

        /**
         * OpenGL Feature flags
         */
        enum OpenGLFeature {
            /**
             * Anisotropic filtering support.
             */
            GL_ANISOTROPIC_TEXTURE_FILTER = 0,

            /**
             * DDS texture compression support.
             */
            GL_COMP_DXT,

            /**
             * ETC1 texture compression support.
             */
            GL_COMP_ETC1,

            /**
             * ETC2/EAC texture compression support.
             */
            GL_COMP_ETC2,

            /**
             * PVR texture compression support.
             */
            GL_COMP_PVRT,

            /**
             * ASTC texture compression support.
             */
            GL_COMP_ASTC,

            /**
             * Depth texture support.
             */
            GL_TEX_DEPTH,

            /**
             * Texture float support.
             */
            GL_TEX_FLOAT,

            /**
             * Clamp mirror wrap mode support.
             */
            GL_TEX_MIRROR_CLAMP,

            /**
             * NPOT Texture support.
             */
            GL_TEX_NPOT,

            /**
             * VAO Support.
             */
            GL_VAO
        };
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

        Matrix m_modelViewStack[MAX_MATRICES];

        int m_modelViewCounter = 0;

        // OpenGL Features
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        /**
         * Maximum anisotropic filtering level.
         */
        float m_GLMaxAnisotropicLevel;

        /**
         * Maximum number of bits in a depth buffer/texture.
         */
        int m_GLMaxDepthBits;

        /**
         * All OpenGL Support flags.
         */
        bool m_GLSupportFlags[GL_VAO + 1];
#endif

        /**
         * Create a graphics device.
         */
        explicit GraphicsDevice(Window *window_);
        ~GraphicsDevice();
    public:

        /**
         * Set the target API.
         *
         * Default values:
         * Platform | API     | Version
         * -------- | ------- | -------
         * Desktop | OpenGL   | 3.3
         * UWP     | OpenGLES | 2.0
         */
        static void SetTargetAPI(GraphicsAPI api_, int majorVersion_, int minorVersion_);

        static GraphicsAPI GetTargetAPI();

        static int GetTargetAPIMajorVersion();

        static int GetTargetAPIMinorVersion();

        /**
         * Get the window attached to this device.
         */
        Window *GetWindow();

        /**
         * Get the current render target.
         *
         * @return The current render target or null if rendering straight to window.
         */
        RenderTarget *GetCurrentTarget();

        /**
         * Set the current render target.
         *
         * @warning Do not provide null, use `PopTarget` instead.
         * @warning This will force all renderers to draw.
         * @param target_ The new target.
         */
        void PushTarget(RenderTarget *target_);

        /**
         * Pop the current target off of the stack.
         *
         * @warning This will force all renderers to draw
         */
        void PopTarget();

        /**
         * Get the current projection matrix.
         *
         * @return The current projection matrix.
         */
        Matrix GetProjectionMatrix() const;

        /**
         * Get the current model view matrix.
         *
         * @note This is basically just for camera support.
         * @return The current model view matrix.
         */
        Matrix GetModelViewMatrix() const;

        void PushModelViewMatrix();

        void PopModelViewMatrix();

        void LoadModelViewIdentity();

        void MultModelView(const Matrix &matrix_);

        /**
         * Setup the framebuffer.
         *
         * This will create an orthographic projection matrix.
         * This will be created for the framebuffer if there is no render target attached, otherwise it will be for the render target.
         */
        void SetupFramebuffer();

        /**
          * Get an OpenGL feature support flag (to determine feature set).
          *
          * @return Whether or not the device has support for the selected flag.
          */
        bool GetGLSupportFlag(OpenGLFeature feature_);

        /**
         * Get the max anisotropic filtering level.
         *
         * @return The max anisotropic filtering level.
         */
        float GetGLMaxAnisotropicLevel();

        /**
         * Get the max depth buffer size (in bits).
         *
         * @return The max depth buffer size.
         */
        int GetGLMaxDepthBits();

        /**
         * Get OpenGL Texture formats.
         *
         * @param format_ Our format.
         * @param glInternalFormat_ The GL Internal format.
         * @param glFormat_ The GL format.
         * @param glType_ The GL type.
         */
        void GetGLTextureFormats(int format_, unsigned int *glInternalFormat_, unsigned int *glFormat_, unsigned int *glType_);

        /**
         * Get the graphics API.
         *
         * @warning This is intended for internal use, please use the classes provided by the `Graphics` or `Graphics::API` namespaces instead.
         */
        API::PlatformGraphicsAPI *GetAPI();
    };
}

#endif //GRAPHICSDEVICE_HPP
