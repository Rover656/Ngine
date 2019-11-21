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

#include "../../Ngine.h"

#include "../../Matrix.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
    /**
     * The graphics device.
     * Manages the OpenGL Context and it's features.
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
             * ASTC texture compression support
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
             * Clamp mirror wrap mode support,
             */
                    GL_TEX_MIRROR_CLAMP,

            /**
             * NPOT Texture support
             */
                    GL_TEX_NPOT,

            /**
             * VAO Support.
             */
                    GL_VAO
        };
    private:
        /**
         * The current projection matrix.
         */
        Matrix _ProjectionMatrix;

        // OpenGL Features
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        /**
         * Maximum anisotropic filtering level.
         */
        float _GLMaxAnisotropicLevel;

        /**
         * Maximum number of bits in a depth buffer/texture.
         */
        int _GLMaxDepthBits;

        /**
         * All OpenGL Support flags.
         */
        bool _GLSupportFlags[GL_VAO + 1];
#endif
    public:

        /**
         * Create a graphics device.
         */
        GraphicsDevice();

        ~GraphicsDevice() = default;

        /**
         * Get the current projection matrix.
         *
         * @return The current projection matrix.
         */
        Matrix GetProjectionMatrix() const;

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
    };
}

#endif //GRAPHICSDEVICE_H
