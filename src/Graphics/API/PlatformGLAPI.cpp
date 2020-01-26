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

#include "PlatformGLAPI.hpp"

// TODO: GLES gets its own renderer.
#if defined(PLATFORM_DESKTOP)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#elif defined(EGL)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1 // UWP???
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif
#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

#if defined(PLATFORM_UWP)
#include <angle_windowsstore.h>
#include "UWP/GameApp.hpp"
#endif

#include "Graphics/Buffer.hpp"
#include "Graphics/VertexLayout.hpp"
#include "Console.hpp"

#if defined(GLAD)
static PFNGLGENVERTEXARRAYSPROC glGenVertexArraysOES;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArrayOES;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArraysOES;
#endif

namespace Ngine::Graphics::API {
    void PlatformGLAPI::_useVertexLayout(VertexLayout *layout_) {
        // Bind VAO if enabled
        if (m_featureFlags[FEATURE_VAO]) {
            if (m_GLES)
                glBindVertexArrayOES(layout_->VAO);
#if !defined(EGL)
            else glBindVertexArray(layout_->VAO);
#endif
        } else {
            // Bind buffers
            auto vBuf = layout_->GetVertexBuffer();
            auto iBuf = layout_->GetIndexBuffer();
            BindBuffer(vBuf);
            if (iBuf != nullptr)
                BindBuffer(iBuf);

            // Setup layout
            auto elements = layout_->GetElements();
            for (const auto& elm : elements) {
                GLenum type;
                switch (elm.Type) {
                    case VertexLayout::VertexElementType::Float:
                        type = GL_FLOAT;
                        break;
                }

                glEnableVertexAttribArray(elm.ElementIndex);
                glVertexAttribPointer(elm.ElementIndex, elm.Count, type, elm.Normalized ? GL_TRUE : GL_FALSE, elm.Stride, (GLvoid *) elm.Offset);
            }
        }
    }

    void PlatformGLAPI::_stopVertexLayout(VertexLayout *layout_) {
        if (m_featureFlags[FEATURE_VAO]) {
            // Unbind VAO
            if (m_GLES)
                glBindVertexArrayOES(0);
#if !defined(EGL)
            else glBindVertexArray(0);
#endif
        } else {
            // Unbind buffers
            auto vBuf = layout_->GetVertexBuffer();
            auto iBuf = layout_->GetIndexBuffer();
            UnbindBuffer(vBuf);
            if (iBuf != nullptr)
                UnbindBuffer(iBuf);

            // Unset layout
            auto elements = layout_->GetElements();
            for (const auto& elm : elements) {
                glDisableVertexAttribArray(elm.ElementIndex);
            }
        }
    }

    PlatformGLAPI::PlatformGLAPI(GraphicsDevice *graphicsDevice_)
     : PlatformGraphicsAPI(graphicsDevice_) {
        // Clear feature flags
        for (auto i = 0; i < EXT_TEX_NPOT; i++) m_featureFlags[i] = false;

#if defined(EGL)
        // Init EGL
        EGLint samples = 0;
        EGLint sampleBuffer = 0;

        // TODO: Get window config.
        //if (config_.MSAA_4X) {
#if !defined(PLATFORM_UWP) // Seems to crash UWP, so don't
            //samples = 4;
            //sampleBuffer = 1;
#endif
        //}

        const EGLint framebufferAttribs[] = {
            // Type of context support -> Required on RPI?
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,

            // RED color bit depth (alternative: 5)
            EGL_RED_SIZE, 8,

            // GREEN color bit depth (alternative: 6)
            EGL_GREEN_SIZE, 8,

            // BLUE color bit depth (alternative: 5)
            EGL_BLUE_SIZE, 8,

            // ALPHA bit depth (required for transparent framebuffer)
            EGL_ALPHA_SIZE, 8,

            // Depth buffer size (Required to use Depth testing!)
            EGL_DEPTH_SIZE, 16,

            // Stencil buffer size
            EGL_STENCIL_SIZE, 8,

            // Activate MSAA
            EGL_SAMPLE_BUFFERS, sampleBuffer,

            // 4x Antialiasing if activated (Free on MALI GPUs)
            EGL_SAMPLES, samples,

            // No idea what this does
            EGL_NONE
        };

        const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
        };

#if defined(PLATFORM_UWP)
        const EGLint surfaceAttributes[] = {
            // EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER is part of the same optimization as EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER (see above).
            // If you have compilation issues with it then please update your Visual Studio templates.
            EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_NONE
        };

        const EGLint defaultDisplayAttributes[] = {
            // These are the default display attributes, used to request ANGLE's D3D11 renderer.
            // eglInitialize will only succeed with these attributes if the hardware supports D3D11 Feature Level 10_0+.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,

            // EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER is an optimization that can have large performance benefits on mobile devices.
            // Its syntax is subject to change, though. Please update your Visual Studio templates if you experience compilation issues with it.
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,

            // EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE is an option that enables ANGLE to automatically call
            // the IDXGIDevice3::Trim method on behalf of the application when it gets suspended.
            // Calling IDXGIDevice3::Trim when an application is suspended is a Windows Store application certification requirement.
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        const EGLint fl9_3DisplayAttributes[] = {
            // These can be used to request ANGLE's D3D11 renderer, with D3D11 Feature Level 9_3.
            // These attributes are used if the call to eglInitialize fails with the default display attributes.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE, 9,
            EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE, 3,
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        const EGLint warpDisplayAttributes[] = {
            // These attributes can be used to request D3D11 WARP.
            // They are used if eglInitialize fails with both the default display attributes and the 9_3 display attributes.
            EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
            EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_DEVICE_TYPE_WARP_ANGLE,
            EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,
            EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
            EGL_NONE,
        };

        EGLConfig config = nullptr;

        // eglGetPlatformDisplayEXT is an alternative to eglGetDisplay. It allows us to pass in Display attributes, used to configure D3D11.
        PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)(eglGetProcAddress("eglGetPlatformDisplayEXT"));
        if (!eglGetPlatformDisplayEXT) {
            Console::Fail("PlatformGLAPI", "Failed to get function eglGetPlatformDisplayEXT");
        }

        // To initialize the Display, we make three sets of calls to eglGetPlatformDisplayEXT and eglInitialize, with varying
        // parameters passed to eglGetPlatformDisplayEXT:
        // 1) The first calls uses "defaultDisplayAttributes" as a parameter. This corresponds to D3D11 Feature Level 10_0+.
        // 2) If eglInitialize fails for step 1 (e.g. because 10_0+ isn't supported by the default GPU), then we try again
        //    using "fl9_3DisplayAttributes". This corresponds to D3D11 Feature Level 9_3.
        // 3) If eglInitialize fails for step 2 (e.g. because 9_3+ isn't supported by the default GPU), then we try again
        //    using "warpDisplayAttributes".  This corresponds to D3D11 Feature Level 11_0 on WARP, a D3D11 software rasterizer.

        // This tries to initialize EGL to D3D11 Feature Level 10_0+. See above comment for details.
        m_display = (void*)eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, defaultDisplayAttributes);
        if (m_display == EGL_NO_DISPLAY) {
            Console::Fail("PlatformGLAPI", "Failed to initialize EGL Display");
        }

        if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
            // This tries to initialize EGL to D3D11 Feature Level 9_3, if 10_0+ is unavailable (e.g. on some mobile devices).
            m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, fl9_3DisplayAttributes);
            if (m_display == EGL_NO_DISPLAY) {
                Console::Fail("PlatformGLAPI", "Failed to initialize EGL Display");
            }

            if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
                // This initializes EGL to D3D11 Feature Level 11_0 on WARP, if 9_3+ is unavailable on the default GPU.
                m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, warpDisplayAttributes);
                if (m_display == EGL_NO_DISPLAY) {
                    Console::Fail("PlatformGLAPI", "Failed to initialize EGL Display");
                }

                if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
                    // If all of the calls to eglInitialize returned EGL_FALSE then an error has occurred.
                    Console::Fail("PlatformGLAPI", "Failed to initialize EGL");
                }
            }
        }

        EGLint numConfigs = 0;
        if ((eglChooseConfig(m_display, framebufferAttribs, &config, 1, &numConfigs) == EGL_FALSE) || (numConfigs == 0)) {
            Console::Fail("PlatformGLAPI", "Failed to choose first EGLConfig");
        }

        // Create a PropertySet and initialize with the EGLNativeWindowType.
        PropertySet^ surfaceCreationProperties = ref new PropertySet();
        surfaceCreationProperties->Insert(ref new String(EGLNativeWindowTypeProperty), CoreWindow::GetForCurrentThread());     // CoreWindow^ window

        // Create surface
        m_surface = eglCreateWindowSurface(m_display, config, (EGLNativeWindowType)CoreWindow::GetForCurrentThread(), surfaceAttributes);

        if (m_surface == EGL_NO_SURFACE) {
            Console::Fail("PlatformGLAPI", "Failed to create EGL fullscreen surface");
        }

        m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
        if (m_context == EGL_NO_CONTEXT) {
            Console::Fail("PlatformGLAPI", "Failed to create EGL context.");
        }
#else
        // For future GLES2 platforms
        EGLint numConfigs;

        // Get an EGL display connection
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (m_display == EGL_NO_DISPLAY) {
            throw std::runtime_error("Failed to initialize EGL display");
        }

        // Initialize the EGL display connection
        if (eglInitialize(m_display, NULL, NULL) == EGL_FALSE) {
            // If all of the calls to eglInitialize returned EGL_FALSE then an error has occurred.
            throw std::runtime_error("Failed to initialize EGL");
        }

        // Get an appropriate EGL framebuffer configuration
        eglChooseConfig(m_display, framebufferAttribs, &config, 1, &numConfigs);

        // Set rendering API
        eglBindAPI(EGL_OPENGL_ES_API);

        // Create an EGL rendering context
        m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
        if (m_context == EGL_NO_CONTEXT) {
            throw std::runtime_error("Failed to create EGL context");
        }

        // TODO: For other EGL platforms, initialize window surface
#endif
#endif

        // Make the attached window current so our GL context is active
#if !defined(EGL) // EGL uses this API, which is still being created. TODO: Make this into an Init function instead to tidy this
        m_graphicsDevice->GetWindow()->MakeCurrent();
#else
        MakeEGLCurrent();
#endif

#if defined(GLAD)
        // Init GLAD loader
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            // Terminate, we can't load
            glfwTerminate();
            Console::Fail("PlatformGLAPI", "Failed to init GLAD.");
        }
        Console::Notice("PlatformGLAPI", "Successfully initialized GLAD.");
#endif

        // Determine if we're running GLES
#if defined(GLAD)
        if (GraphicsDevice::GetTargetAPI() == GraphicsAPI::OpenGLES)
            m_GLES = true;
#elif defined(EGL)
        // EGL has to be GLES, no matter what.
        m_GLES = true;
#endif

        // Setup default extensions
#if defined(PLATFORM_DESKTOP) // Desktop supports GL and GLES, so we need to distinguish between them
        if (!m_GLES && GLAD_GL_VERSION_3_0) {
            m_featureFlags[FEATURE_VAO] = true;
            m_featureFlags[EXT_TEX_NPOT] = true;
            m_featureFlags[EXT_TEX_FLOAT] = true;
            m_featureFlags[EXT_TEX_DEPTH] = true;
        }
#endif

        // Build extension list
        int numExt = 0;
        const char **extList = nullptr;
        if (m_GLES) {
            // Create array
            extList = new const char*[512];

            const char *extensions = (const char *) glGetString(GL_EXTENSIONS);

            int len = strlen(extensions) + 1;
            char *exts = (char *) new char[len];
            strcpy(exts, extensions);

            extList[numExt] = exts;

            for (int i = 0; i < len; i++) {
                if (exts[i] == ' ') {
                    exts[i] = '\0';
                    numExt++;
                    extList[numExt] = &exts[i + 1];
                }
            }
        } else {
#if defined(GLAD)
            // Get extension count
            glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

            // Get extensions
            extList = new const char*[numExt];
            for (auto i = 0; i < numExt; i++) extList[i] = (char *) glGetStringi(GL_EXTENSIONS, i);
#endif
        }

        // Process extensions
        for (auto i = 0; i < numExt; i++) {
            // GLES2 Specific Extensions
            if (m_GLES) {
                // Check for VAO support
                if (strcmp(extList[i], "GL_OES_vertex_array_object") == 0) {
#if defined(GLAD)
#if defined(PLATFORM_DESKTOP)
                    // GLFW does not provide the OES version, try to find it.
                    glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSPROC) glfwGetProcAddress("glGenVertexArraysOES");
                    glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYPROC) glfwGetProcAddress("glBindVertexArrayOES");
                    glDeleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSPROC) glfwGetProcAddress("glDeleteVertexArraysOES");

                    if ((glGenVertexArraysOES != nullptr) && (glBindVertexArrayOES != nullptr) &&
                        (glDeleteVertexArraysOES != nullptr))
                        m_featureFlags[FEATURE_VAO] = true;
#endif
#elif defined(EGL)
                    // gl2ext.h provides the functions already.
                    m_featureFlags[FEATURE_VAO] = true;
#endif
                }

                // Check NPOT textures support
                if (strcmp(extList[i], "GL_OES_texture_npot") == 0) m_featureFlags[EXT_TEX_NPOT] = true;

                // Check texture float support
                if (strcmp(extList[i], "GL_OES_texture_float") == 0) m_featureFlags[EXT_TEX_FLOAT] = true;

                // Check depth texture support
                if ((strcmp(extList[i], "GL_OES_depth_texture") == 0) ||
                    (strcmp(extList[i], "GL_WEBGL_depth_texture") == 0))
                    m_featureFlags[EXT_TEX_DEPTH] = true;

                if (strcmp(extList[i], "GL_OES_depth24") == 0) m_maxDepthBits = 24;
                if (strcmp(extList[i], "GL_OES_depth32") == 0) m_maxDepthBits = 32;
            }

            // DDS texture compression support
            if ((strcmp(extList[i], "GL_EXT_texture_compression_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBKIT_WEBGL_compressed_texture_s3tc") == 0))
                m_featureFlags[EXT_COMP_DXT] = true;

            // ETC1 texture compression support
            if ((strcmp(extList[i], "GL_OES_compressed_ETC1_RGB8_texture") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_etc1") == 0))
                m_featureFlags[EXT_COMP_ETC1] = true;

            // ETC2/EAC texture compression support
            if (strcmp(extList[i], "GL_ARB_ES3_compatibility") == 0) m_featureFlags[EXT_COMP_ETC2] = true;

            // PVR texture compression support
            if (strcmp(extList[i], "GL_IMG_texture_compression_pvrtc") == 0) m_featureFlags[EXT_COMP_PVRT] = true;

            // ASTC texture compression support
            if (strcmp(extList[i], "GL_KHR_texture_compression_astc_hdr") == 0) m_featureFlags[EXT_COMP_ASTC] = true;

            // Anisotropic texture filter
            if (strcmp(extList[i], (const char *) "GL_EXT_texture_filter_anisotropic") == 0) {
                m_featureFlags[EXT_ANISOTROPIC_TEXTURE_FILTER] = true;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropicLevel);
            }

            // Clamp mirror wrap mode supported
            if (strcmp(extList[i], (const char *) "GL_EXT_texture_mirror_clamp") == 0)
                m_featureFlags[EXT_TEX_MIRROR_CLAMP] = true;

            // TODO: Determine m_maxDepthBits for OpenGL
        }
    }

    PlatformGLAPI::~PlatformGLAPI() {
#if defined(EGL)
        // Close surface, context and Display
        if (m_display != EGL_NO_DISPLAY) {
            eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

            if (m_surface != EGL_NO_SURFACE) {
                eglDestroySurface(m_display, m_surface);
                m_surface = EGL_NO_SURFACE;
            }

            if (m_context != EGL_NO_CONTEXT) {
                eglDestroyContext(m_display, m_context);
                m_context = EGL_NO_CONTEXT;
            }

            eglTerminate(m_display);
            m_display = EGL_NO_DISPLAY;
        }
#endif
    }

    void PlatformGLAPI::Clear(const Color &color_) {
        glClearColor(color_.R, color_.G, color_.B, color_.A);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void PlatformGLAPI::BindTexture(Texture2D *texture_) {
        if (texture_ != m_currentTexture) {
            glBindTexture(GL_TEXTURE_2D, texture_ != nullptr ? texture_->ID : 0);
            m_currentTexture = texture_;
        }
    }

    void PlatformGLAPI::BindShader(ShaderProgram *shader_) {
        if (shader_ != m_currentShader) {
            glUseProgram(shader_ != nullptr ? shader_->ID : 0);
            m_currentShader = shader_;
        }
    }

    void PlatformGLAPI::BindBuffer(Buffer *buffer_) {
        // Check not null
        if (buffer_ == nullptr)
            Console::Fail("PlatformGLAPI", "Cannot bind null buffer.");

        // Bind buffer
        switch (buffer_->Type) {
            case BufferType::Vertex:
                glBindBuffer(GL_ARRAY_BUFFER, buffer_->ID);
                break;
            case BufferType::Index:
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_->ID);
                break;
        }
    }

    void PlatformGLAPI::UnbindBuffer(Buffer *buffer_) {
        // Check not null
        if (buffer_ == nullptr)
            Console::Fail("PlatformGLAPI", "Cannot unbind null buffer.");

        // Bind buffer
        switch (buffer_->Type) {
            case BufferType::Vertex:
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                break;
            case BufferType::Index:
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                break;
        }
    }

    void PlatformGLAPI::InitializeBuffer(Buffer *buffer_) {
        // Create buffer
        buffer_->ID = 0;
        glGenBuffers(1, &buffer_->ID);

        // Bind
        BindBuffer(buffer_);
    }

    void PlatformGLAPI::CleanupBuffer(Buffer *buffer_) {
        // Unbind buffer
        UnbindBuffer(buffer_);

        // Delete
        glDeleteBuffers(1, &buffer_->ID);
        buffer_->ID = 0;
    }

    void PlatformGLAPI::WriteBuffer(Buffer *buffer_, void *data_, int count_, int size_, bool update_) {
        // Bind
        BindBuffer(buffer_);

        // Get type
        GLenum bufType = 0;
        switch (buffer_->Type) {
            case BufferType::Vertex:
                bufType = GL_ARRAY_BUFFER;
                break;
            case BufferType::Index:
                bufType = GL_ELEMENT_ARRAY_BUFFER;
                break;
        }

        // Get usage
        GLenum bufUsage = 0;
        switch (buffer_->Usage) {
            case BufferUsage::Static:
                bufUsage = GL_STATIC_DRAW;
                break;
            case BufferUsage::Dynamic:
                bufUsage = GL_DYNAMIC_DRAW;
                break;
        }

        // Write
        if (update_) {
            glBufferSubData(bufType, 0, size_ * count_, data_);
        } else {
            glBufferData(bufType, size_ * count_, data_, bufUsage);
        }
    }

    void PlatformGLAPI::InitializeVertexLayout(VertexLayout *layout_) {
        // Create VAO if enabled
        if (m_featureFlags[FEATURE_VAO]) {
            layout_->VAO = 0;
            if (m_GLES)
                glGenVertexArraysOES(1, &layout_->VAO);
#if !defined(EGL)
            else glGenVertexArrays(1, &layout_->VAO);
#endif
        }
    }

    void PlatformGLAPI::CleanupVertexLayout(VertexLayout *layout_) {
        // Delete VAO if enabled
        if (m_featureFlags[FEATURE_VAO]) {
            if (m_GLES)
                glDeleteVertexArraysOES(1, &layout_->VAO);
#if !defined(EGL)
            else glDeleteVertexArrays(1, &layout_->VAO);
#endif
            layout_->VAO = 0;
        }
    }

    void PlatformGLAPI::ConfigureVertexLayout(VertexLayout *layout_) {
        // Get buffers
        auto vBuf = layout_->GetVertexBuffer();
        auto iBuf = layout_->GetIndexBuffer();

        if (m_featureFlags[FEATURE_VAO]) {
            // Unbind any existing buffers
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // Bind VAO
            if (m_GLES)
                glBindVertexArrayOES(layout_->VAO);
#if !defined(EGL)
            else glBindVertexArray(layout_->VAO);
#endif

            // Bind buffers
            BindBuffer(vBuf);
            if (iBuf != nullptr)
                BindBuffer(iBuf);
        }

        auto elements = layout_->GetElements();
        for (const auto& elm : elements) {
            GLenum type;
            switch (elm.Type) {
                case VertexLayout::VertexElementType::Float:
                    type = GL_FLOAT;
                    break;
            }

            glEnableVertexAttribArray(elm.ElementIndex);
            glVertexAttribPointer(elm.ElementIndex, elm.Count, type, elm.Normalized ? GL_TRUE : GL_FALSE, elm.Stride, (GLvoid *) elm.Offset);
        }

        // Unbind VAO then buffers (if present)
        if (m_featureFlags[FEATURE_VAO]) {
            if (m_GLES)
                glBindVertexArrayOES(0);
#if !defined(EGL)
            else glBindVertexArray(0);
#endif
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    void PlatformGLAPI::Draw(int count_, int start_) {
        glDrawArrays(GL_TRIANGLES, start_, count_);
    }

    void PlatformGLAPI::DrawIndexed(int count_, int start_) {
        glDrawElements(GL_TRIANGLES, count_, GL_UNSIGNED_SHORT, (GLvoid*) start_);
    }

#if defined(EGL)
    EGLContext PlatformGLAPI::GetEGLContext() {
        return m_context;
    }

    EGLDisplay PlatformGLAPI::GetEGLDisplay() {
        return m_display;
    }

    EGLSurface PlatformGLAPI::GetEGLSurface() {
        return m_surface;
    }

    void PlatformGLAPI::QueryEGLSurfaceSize(int *width_, int *height_) {
        eglQuerySurface(m_display, m_surface, EGL_WIDTH, width_);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT, height_);
    }

    void PlatformGLAPI::MakeEGLCurrent() {
        if (eglMakeCurrent(m_display, m_surface, m_surface, m_context) == EGL_FALSE)
            Console::Fail("PlatformGLAPI", "Failed to make EGL context current.");
    }

    void PlatformGLAPI::SwapEGLBuffers() {
        eglSwapBuffers(m_display, m_surface);
    }

    void PlatformGLAPI::SetEGLSwapInterval(int interval_) {
        eglSwapInterval(m_display, interval_);
    }
#endif
}