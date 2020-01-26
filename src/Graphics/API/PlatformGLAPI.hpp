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

#include "Config.hpp"

#include "Graphics/API/PlatformGraphicsAPI.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLAD // GLAD supports GL and GLES, No other platform can do this.
#elif defined(PLATFORM_UWP) && defined(GRAPHICS_OPENGLES2)
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
        EX_TEX_DEPTH,

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
        ShaderProgram *m_currentShader = nullptr;
        bool m_GLES = false;

        bool m_featureFlags[EXT_TEX_NPOT + 1];

        // For EGL platforms
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
    protected:
        void _useVertexLayout(VertexLayout *layout_) override;
        void _stopVertexLayout(VertexLayout *layout_) override;
    public:
        PlatformGLAPI(GraphicsDevice *graphicsDevice_);
        ~PlatformGLAPI() override;

        void Clear(const Color &color_) override;
        void BindTexture(Texture2D *texture_) override;
        void BindShader(ShaderProgram *shader_) override;
        void BindBuffer(Buffer *buffer_) override;
        void UnbindBuffer(Buffer *buffer_) override;
        void InitializeBuffer(Buffer *buffer_) override;
        void CleanupBuffer(Buffer *buffer_) override;
        void WriteBuffer(Buffer *buffer_, void *data_, int count_, int size_, bool update_) override;
        void InitializeVertexLayout(VertexLayout *layout_) override;
        void CleanupVertexLayout(VertexLayout *layout_) override;
        void ConfigureVertexLayout(VertexLayout *layout_) override;
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

#endif //GLRENDERER_HPP
