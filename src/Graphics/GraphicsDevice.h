/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include "../Ngine.h"

#include "../Matrix.h"
#include "../Window.h"
#include "Image.h"

namespace NerdThings::Ngine::Graphics {
    /**
     * The graphics device.
     * Manages the OpenGL Context and it's features.
     * Once feature complete this will replace `GraphicsManager`.
     *
     * @note This is very WIP. Only features that are required will be implemented until the new `Renderer` works.
     */
    class NEAPI GraphicsDevice {
    public:
        /**
         * The maximum number of matrices permitted per stack.
         */
        static const int MATRIX_STACK_SIZE = 32;

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
         * The current projection matrix.
         */
        Matrix m_projectionMatrix;

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

#endif //GRAPHICSDEVICE_H
