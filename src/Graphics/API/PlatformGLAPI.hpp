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

#include "Graphics/API/PlatformGraphicsAPI.hpp"

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

namespace Ngine::Graphics::API {
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

        void _getTextureFormats(PixelFormat format_, unsigned int *glInternalFormat_, unsigned int *glFormat_, unsigned int *glType_);
        int _calculatePixelDataSize(int width_, int height_, PixelFormat format_);
        void _setUniform(ShaderProgramState *state_, std::string name_, ShaderDataStructure structure_, const void *data_);

        /**
         * Write a simple uniform (int, uint, float, matrix).
         */
        void _writeSimpleUniform(const ShaderProgram *program_, const char *name_, ShaderDataType type_, int count_, const void* data_);
    protected:
        void _useVertexLayout(VertexLayout *layout_) override;
        void _stopVertexLayout(VertexLayout *layout_) override;
    public:
        PlatformGLAPI(GraphicsDevice *graphicsDevice_);
        ~PlatformGLAPI() override;

        void ConfigureViewport(int x_, int y_, int width_, int height_) override;
        void Clear(const Color &color_) override;
        void CreateTexture(Texture2D *texture_, unsigned char *data_) override;
        void DeleteTexture(Texture2D *texture_) override;
        void BindTexture(Texture2D *texture_) override;
        void SetTextureFilterMode(Texture2D *texture_, TextureFilterMode mode_) override;
        void SetTextureWrapMode(Texture2D *texture_, TextureWrapMode mode_) override;
        bool IsTextureValid(const Texture2D *texture_) override;
        bool CompareTextures(const Texture2D *a_, const Texture2D *b_) override;
        bool CreateRenderTarget(RenderTarget *renderTarget_) override;
        void DeleteRenderTarget(RenderTarget *renderTarget_) override;
        void BindRenderTarget(RenderTarget *renderTarget_) override;
        bool IsRenderTargetValid(const RenderTarget *renderTarget_) override;
        bool CompareRenderTargets(const RenderTarget *a_, const RenderTarget *b_) override;
        void CreateShader(Shader *shader_, void *sourceData_) override;
        void DeleteShader(Shader *shader_) override;
        bool IsShaderValid(const Shader *shader_) override;
        void CreateShaderProgram(ShaderProgram *program_) override;
        void DeleteShaderProgram(ShaderProgram *program_) override;
        void BindShaderProgram(const ShaderProgram *program_) override;
        bool IsShaderProgramValid(const ShaderProgram *program_) override;
        void BindShaderProgramState(ShaderProgramState *state_) override;
        void BindBuffer(Buffer *buffer_) override;
        void UnbindBuffer(Buffer *buffer_) override;
        void CreateBuffer(Buffer *buffer_) override;
        void DeleteBuffer(Buffer *buffer_) override;
        void WriteBuffer(Buffer *buffer_, void *data_, int count_, int size_, bool update_) override;
        void CreateVertexLayout(VertexLayout *layout_) override;
        void DeleteVertexLayout(VertexLayout *layout_) override;
        void ConfigureVertexLayout(VertexLayout *layout_) override;
        void PrepareFor2D() override;
        void Draw(int count_, int start_) override;
        void DrawIndexed(int count_, int start_) override;

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
