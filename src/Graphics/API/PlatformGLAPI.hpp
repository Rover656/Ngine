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

#include "Config.hpp"

#include "graphics/API/PlatformGraphicsAPI.hpp"

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
        void _getTextureFormats(PixelFormat format_, unsigned int *glInternalFormat_, unsigned int *glFormat_, unsigned int *glType_);

        /**
         * Calculate the size of the pixel data.
         */
        int _calculatePixelDataSize(int width_, int height_, PixelFormat format_);

        /**
         * Set a uniform value inside of the current program.
         *
         * @param program_ The program we are writing to.
         * @param name_ The current name (this is for traversing the OpenGL naming setup)
         * @param structure_ The current structure being processed.
         * @param data_ The data (at the current offset).
         */
        void _setUniform(const ShaderProgram *program_, std::string name_, ShaderDataStructure structure_, const void *data_);

        /**
         * Write a simple uniform (int, uint, float, matrix).
         *
         * @param program_ Program to write to.
         * @param name_ The name to write to.
         * @param type_ The data type.
         * @param count_ The number of values.
         * @param data_ The data to write.
         */
        void _writeSimpleUniform(const ShaderProgram *program_, const char *name_, ShaderDataType type_, int count_, const void* data_);
    protected:
        void _useVertexLayout(VertexLayout *layout_) override;
        void _stopVertexLayout(VertexLayout *layout_) override;
    public:
        PlatformGLAPI(GraphicsDevice *graphicsDevice_);
        ~PlatformGLAPI() override;

        void configureViewport(int x_, int y_, int width_, int height_) override;
        void clear(const Color &color_) override;
        void createTexture(Texture2D *texture_, unsigned char *data_) override;
        void deleteTexture(Texture2D *texture_) override;
        void bindTexture(Texture2D *texture_) override;
        void setTextureFilterMode(Texture2D *texture_, TextureFilterMode mode_) override;
        void setTextureWrapMode(Texture2D *texture_, TextureWrapMode mode_) override;
        bool isTextureValid(const Texture2D *texture_) override;
        bool compareTextures(const Texture2D *a_, const Texture2D *b_) override;
        bool createRenderTarget(RenderTarget *renderTarget_) override;
        void deleteRenderTarget(RenderTarget *renderTarget_) override;
        void bindRenderTarget(RenderTarget *renderTarget_) override;
        bool isRenderTargetValid(const RenderTarget *renderTarget_) override;
        bool compareRenderTargets(const RenderTarget *a_, const RenderTarget *b_) override;
        void createShader(Shader *shader_, void *sourceData_) override;
        void deleteShader(Shader *shader_) override;
        bool isShaderValid(const Shader *shader_) override;
        void createShaderProgram(ShaderProgram *program_) override;
        void deleteShaderProgram(ShaderProgram *program_) override;
        void bindShaderProgram(const ShaderProgram *program_) override;
        bool isShaderProgramValid(const ShaderProgram *program_) override;
        void bindShaderProgramState(ShaderProgramState *state_) override;
        void bindBuffer(Buffer *buffer_) override;
        void unbindBuffer(Buffer *buffer_) override;
        void createBuffer(Buffer *buffer_) override;
        void deleteBuffer(Buffer *buffer_) override;
        void writeBuffer(Buffer *buffer_, void *data_, int count_, int size_, bool update_) override;
        void createVertexLayout(VertexLayout *layout_) override;
        void deleteVertexLayout(VertexLayout *layout_) override;
        void configureVertexLayout(VertexLayout *layout_) override;
        void prepareFor2D() override;
        void draw(int count_, int start_) override;
        void drawIndexed(int count_, int start_) override;

#if defined(EGL)
        EGLContext GetEGLContext();
        EGLDisplay GetEGLDisplay();
        EGLSurface GetEGLSurface();
        void QueryEGLSurfaceSize(int *width_, int *height_);
        void MakeEGLCurrent();
        void SwapEGLBuffers();
        void SetEGLSwapInterval(int interval_);
#endif
    };
}

#endif

#endif //GLRENDERER_HPP
