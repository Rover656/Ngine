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

#ifndef GLRENDERER_HPP
#define GLRENDERER_HPP

#if defined(API_OPENGL_ENABLED) || defined(API_OPENGLES_ENABLED)

#include "ngine/Config.hpp"

#include "ngine/graphics/API/PlatformGraphicsAPI.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLAD
#define GLFW
#elif defined(PLATFORM_UWP)
#define EGL
#endif

#if defined(EGL)
typedef void *EGLDisplay;
typedef void *EGLSurface;
typedef void *EGLContext;
#endif

namespace ngine::graphics::API {
    enum GLFeatureFlag {
        /**
         * VAO Support.
         */
        FEATURE_VAO = 0,

        /**
         * Anisotropic filtering support.
         */
        EXT_ANISOTROPIC_TEXTURE_FILTER,

        /**
         * DDS texture compression support.
         */
        EXT_COMP_DXT,

        /**
         * ETC1 texture compression support.
         */
        EXT_COMP_ETC1,

        /**
         * ETC2/EAC texture compression support.
         */
        EXT_COMP_ETC2,

        /**
         * PVR texture compression support.
         */
        EXT_COMP_PVRT,

        /**
         * ASTC texture compression support.
         */
        EXT_COMP_ASTC,

        /**
         * Depth texture support.
         */
        EXT_TEX_DEPTH,

        /**
         * Texture float support.
         */
        EXT_TEX_FLOAT,

        /**
         * Clamp mirror wrap mode support.
         */
        EXT_TEX_MIRROR_CLAMP,

        /**
         * NPOT Texture support.
         */
        EXT_TEX_NPOT,
    };

    /**
     * The GL renderer layer.
     */
    class PlatformGLAPI : public PlatformGraphicsAPI {
        Texture2D *m_currentTexture = nullptr;
        RenderTarget *m_currentRenderTarget = nullptr;
        bool m_GLES2 = false;
        bool m_GLES3 = false;

        bool m_featureFlags[EXT_TEX_NPOT + 1];
        int m_maxDepthBits = 0;
        float m_maxAnisotropicLevel = 0;

        // We control the context for EGL platforms.
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
         * Get OpenGL texture formats for a specified pixel format.
         */
        void _getTextureFormats(PixelFormat format, unsigned int *glInternalFormat, unsigned int *glFormat, unsigned int *glType);

        /**
         * Calculate the size of the pixel data.
         */
        int _calculatePixelDataSize(int width, int height, PixelFormat format);

        /**
         * Set a uniform value inside of the current program.
         *
         * @param program The program we are writing to.
         * @param name The current name (this is for traversing the OpenGL naming setup)
         * @param structure The current structure being processed.
         * @param data The data (at the current offset).
         */
        void _setUniform(const ShaderProgram *program, std::string name, ShaderDataStructure structure, const void *data);

        /**
         * Write a simple uniform (int, uint, float, matrix).
         *
         * @param program Program to write to.
         * @param name The name to write to.
         * @param type The data type.
         * @param count The number of values.
         * @param data The data to write.
         */
        void _writeSimpleUniform(const ShaderProgram *program, const char *name, ShaderDataType type, int count, const void* data);
    protected:
        void _useVertexLayout(VertexLayout *layout) override;
        void _stopVertexLayout(VertexLayout *layout) override;
    public:
        PlatformGLAPI(GraphicsDevice *graphicsDevice);
        ~PlatformGLAPI() override;

        void configureViewport(int x, int y, int width, int height) override;
        void clear(const Color &color) override;
        void createTexture(Texture2D *texture, unsigned char *data) override;
        void deleteTexture(Texture2D *texture) override;
        void bindTexture(Texture2D *texture) override;
        void setTextureFilterMode(Texture2D *texture, TextureFilterMode mode) override;
        void setTextureWrapMode(Texture2D *texture, TextureWrapMode mode) override;
        bool isTextureValid(const Texture2D *texture) override;
        bool compareTextures(const Texture2D *a, const Texture2D *b) override;
        bool createRenderTarget(RenderTarget *renderTarget) override;
        void deleteRenderTarget(RenderTarget *renderTarget) override;
        void bindRenderTarget(RenderTarget *renderTarget) override;
        bool isRenderTargetValid(const RenderTarget *renderTarget) override;
        bool compareRenderTargets(const RenderTarget *a, const RenderTarget *b) override;
        void createShader(Shader *shader, void *sourceData) override;
        void deleteShader(Shader *shader) override;
        bool isShaderValid(const Shader *shader) override;
        void createShaderProgram(ShaderProgram *program) override;
        void deleteShaderProgram(ShaderProgram *program) override;
        void bindShaderProgram(const ShaderProgram *program) override;
        bool isShaderProgramValid(const ShaderProgram *program) override;
        void bindShaderProgramState(ShaderProgramState *state) override;
        void bindBuffer(Buffer *buffer) override;
        void unbindBuffer(Buffer *buffer) override;
        void createBuffer(Buffer *buffer) override;
        void deleteBuffer(Buffer *buffer) override;
        void writeBuffer(Buffer *buffer, void *data, int count, int size, bool update) override;
        void createVertexLayout(VertexLayout *layout) override;
        void deleteVertexLayout(VertexLayout *layout) override;
        void configureVertexLayout(VertexLayout *layout) override;
        void prepareFor2D() override;
        void draw(int count, int start) override;
        void drawIndexed(int count, int start) override;

#if defined(EGL)
        EGLContext GetEGLContext();
        EGLDisplay GetEGLDisplay();
        EGLSurface GetEGLSurface();
        void QueryEGLSurfaceSize(int *width, int *height);
        void MakeEGLCurrent();
        void SwapEGLBuffers();
        void SetEGLSwapInterval(int interval);
#endif
    };
}

#endif

#endif //GLRENDERER_HPP
