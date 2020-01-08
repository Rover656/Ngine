/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef GRAPHICSDEVICE_HPP
#define GRAPHICSDEVICE_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "../Window.hpp"
#include "Image.hpp"

namespace NerdThings::Ngine::Graphics {
    class Renderer;
    struct RenderTarget;

    /**
     * The graphics device.
     * Manages the OpenGL Context and it's features.
     * Once feature complete this will replace `GraphicsManager`.
     *
     * @note This is very WIP. Only features that are required will be implemented until the new `Renderer` works.
     */
    class NEAPI GraphicsDevice {
        friend class Renderer;
    public:
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
    public:

        /**
         * Create a graphics device.
         */
        explicit GraphicsDevice(Window *window_);
        ~GraphicsDevice() = default;

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
    };
}

#endif //GRAPHICSDEVICE_HPP
