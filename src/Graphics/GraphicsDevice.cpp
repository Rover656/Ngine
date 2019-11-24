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

#include "GraphicsDevice.h"

#include "Rewrite/OpenGLDefines.h"

namespace NerdThings::Ngine::Graphics {
    GraphicsDevice::GraphicsDevice(Window *window_) : m_attachedWindow(window_) {
        // Check window
        if (m_attachedWindow == nullptr) throw std::runtime_error("Window cannot be null.");

#if defined(PLATFORM_DESKTOP) && defined(GRAPHICS_OPENGL33)
        // Init GLAD loader
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            // Terminate GLFW
            glfwTerminate();
            ConsoleMessage("Failed to init GLAD.", "FATAL", "GraphicsDevice");
            throw std::runtime_error("Failed to init GLAD.");
        }
        ConsoleMessage("Successfully initialised GLAD.", "NOTICE", "GraphicsDevice");
#endif

        // Make window current while we configure OpenGL
        m_attachedWindow->MakeCurrent();

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        // Send OpenGL Version to console
        const char *ver = (const char *) glGetString(GL_VERSION);
        ConsoleMessage("OpenGL Version: " + std::string(ver), "NOTICE", "GraphicsDevice");

        // Init support flags
        for (auto i = 0; i <= GL_VAO; i++) m_GLSupportFlags[i] = false;

        // Load extensions
        int numExt = 0;
#if defined(GRAPHICS_OPENGL33)
        // Supported by default
        m_GLSupportFlags[GL_VAO] = true;
        m_GLSupportFlags[GL_TEX_NPOT] = true;
        m_GLSupportFlags[GL_TEX_FLOAT] = true;
        m_GLSupportFlags[GL_TEX_DEPTH] = true;

        // Get extension count
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

        // Allocate extensions list
        const char **extList = new const char *[numExt];

        // Get extensions
        for (auto i = 0; i < numExt; i++) extList[i] = (char *) glGetStringi(GL_EXTENSIONS, i);
#elif defined(GRAPHICS_OPENGLES2)
        // Allocate 2kb of strings
        const char **extList = new const char*[512];

        // Get extension strings
        char *extensions = (char *)glGetString(GL_EXTENSIONS);
        int len = strlen(extensions);

        for (auto i = 0; i < len; i++) {
            if (i == ' ') {
                extList[numExt] = &extensions[i + 1];
                numExt++;
            }
        }
#endif
        // Search for extensions
        for (auto i = 0; i < numExt; i++) {
            // GLES2 Specific Extensions
#if defined (GRAPHICS_OPENGLES2)
            // Check for VAO support
            if (strcmp(extList[i], "GL_OES_vertex_array_object") == 0)
            {
                // The extension is supported by our hardware and driver, try to get related functions pointers
                glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
                glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
                glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");

                if ((glGenVertexArrays != NULL) && (glBindVertexArray != NULL) && (glDeleteVertexArrays != NULL)) m_GLSupportFlags[GL_VAO] = true;
            }

            // Check NPOT textures support
            if (strcmp(extList[i], "GL_OES_texture_npot") == 0) m_GLSupportFlags[GL_TEX_NPOT] = true;

            // Check texture float support
            if (strcmp(extList[i], "GL_OES_texture_float") == 0) m_GLSupportFlags[GL_TEX_FLOAT] = true;

            // Check depth texture support
            if ((strcmp(extList[i], "GL_OES_depth_texture") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_depth_texture") == 0)) m_GLSupportFlags[GL_TEX_DEPTH] = true;

            if (strcmp(extList[i], "GL_OES_depth24") == 0) m_GLMaxDepthBits = 24;
            if (strcmp(extList[i], "GL_OES_depth32") == 0) m_GLMaxDepthBits = 32;
#endif
            // DDS texture compression support
            if ((strcmp(extList[i], "GL_EXT_texture_compression_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBKIT_WEBGL_compressed_texture_s3tc") == 0))
                m_GLSupportFlags[GL_COMP_DXT] = true;

            // ETC1 texture compression support
            if ((strcmp(extList[i], "GL_OES_compressed_ETC1_RGB8_texture") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_etc1") == 0))
                m_GLSupportFlags[GL_COMP_ETC1] = true;

            // ETC2/EAC texture compression support
            if (strcmp(extList[i], "GL_ARB_ES3_compatibility") == 0) m_GLSupportFlags[GL_COMP_ETC2] = true;

            // PVR texture compression support
            if (strcmp(extList[i], "GL_IMG_texture_compression_pvrtc") == 0) m_GLSupportFlags[GL_COMP_PVRT] = true;

            // ASTC texture compression support
            if (strcmp(extList[i], "GL_KHR_texture_compression_astc_hdr") == 0) m_GLSupportFlags[GL_COMP_ASTC] = true;

            // Anisotropic texture filter
            if (strcmp(extList[i], (const char *) "GL_EXT_texture_filter_anisotropic") == 0) {
                m_GLSupportFlags[GL_ANISOTROPIC_TEXTURE_FILTER] = true;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_GLMaxAnisotropicLevel);
            }

            // Clamp mirror wrap mode supported
            if (strcmp(extList[i], (const char *) "GL_EXT_texture_mirror_clamp") == 0)
                m_GLSupportFlags[GL_TEX_MIRROR_CLAMP] = true;
        }
#endif
        ConsoleMessage("Successfully loaded extensions.", "NOTICE", "GraphicsDevice");

        // Load default matrices
        m_projectionMatrix = Matrix::Identity;
    }

    Window *GraphicsDevice::GetWindow() {
        return m_attachedWindow;
    }

    Matrix GraphicsDevice::GetProjectionMatrix() const {
        return m_projectionMatrix;
    }

    bool GraphicsDevice::GetGLSupportFlag(GraphicsDevice::OpenGLFeature feature_) {
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        return m_GLSupportFlags[feature_];
#endif
        return false;
    }

    float GraphicsDevice::GetGLMaxAnisotropicLevel() {
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        return m_GLMaxAnisotropicLevel;
#endif
        return 0;
    }

    int GraphicsDevice::GetGLMaxDepthBits() {
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        return m_GLMaxDepthBits;
#endif
        return 0;
    }

    void GraphicsDevice::GetGLTextureFormats(int format_, unsigned int *glInternalFormat_, unsigned int *glFormat_,
                                             unsigned int *glType_) {
        *glInternalFormat_ = -1;
        *glFormat_ = -1;
        *glType_ = -1;
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        switch (format_) {
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGLES2)
            // NOTE: on OpenGL ES 2.0 (WebGL), internalFormat must match format and options allowed are: GL_LUMINANCE, GL_RGB, GL_RGBA
        case UNCOMPRESSED_GRAYSCALE:
            *glInternalFormat_ = GL_LUMINANCE;
            *glFormat_ = GL_LUMINANCE;
            *glType_ = GL_UNSIGNED_BYTE;
            break;
        case UNCOMPRESSED_GRAY_ALPHA:
            *glInternalFormat_ = GL_LUMINANCE_ALPHA;
            *glFormat_ = GL_LUMINANCE_ALPHA;
            *glType_ = GL_UNSIGNED_BYTE;
            break;
        case UNCOMPRESSED_R5G6B5:
            *glInternalFormat_ = GL_RGB;
            *glFormat_ = GL_RGB;
            *glType_ = GL_UNSIGNED_SHORT_5_6_5;
            break;
        case UNCOMPRESSED_R8G8B8:
            *glInternalFormat_ = GL_RGB;
            *glFormat_ = GL_RGB;
            *glType_ = GL_UNSIGNED_BYTE;
            break;
        case UNCOMPRESSED_R5G5B5A1:
            *glInternalFormat_ = GL_RGBA;
            *glFormat_ = GL_RGBA;
            *glType_ = GL_UNSIGNED_SHORT_5_5_5_1;
            break;
        case UNCOMPRESSED_R4G4B4A4:
            *glInternalFormat_ = GL_RGBA;
            *glFormat_ = GL_RGBA;
            *glType_ = GL_UNSIGNED_SHORT_4_4_4_4;
            break;
        case UNCOMPRESSED_R8G8B8A8:
            *glInternalFormat_ = GL_RGBA;
            *glFormat_ = GL_RGBA;
            *glType_ = GL_UNSIGNED_BYTE;
            break;
        case UNCOMPRESSED_R32:
            // NOTE: Requires extension OES_texture_float
            if (m_GLSupportFlags[GL_TEX_FLOAT]) {
                *glInternalFormat_ = GL_LUMINANCE;
                *glFormat_ = GL_LUMINANCE;
                *glType_ = GL_FLOAT;
            }
            break;
        case UNCOMPRESSED_R32G32B32:
            // NOTE: Requires extension OES_texture_float
            if (m_GLSupportFlags[GL_TEX_FLOAT]) {
                *glInternalFormat_ = GL_RGB;
                *glFormat_ = GL_RGB;
                *glType_ = GL_FLOAT;
            }
            break;
        case UNCOMPRESSED_R32G32B32A32:
            // NOTE: Requires extension OES_texture_float
            if (m_GLSupportFlags[GL_TEX_FLOAT]) {
                *glInternalFormat_ = GL_RGBA;
                *glFormat_ = GL_RGBA;
                *glType_ = GL_FLOAT;
            }
            break;
#elif defined(GRAPHICS_OPENGL33)
            case UNCOMPRESSED_GRAYSCALE:
                *glInternalFormat_ = GL_R8;
                *glFormat_ = GL_RED;
                *glType_ = GL_UNSIGNED_BYTE;
                break;
            case UNCOMPRESSED_GRAY_ALPHA:
                *glInternalFormat_ = GL_RG8;
                *glFormat_ = GL_RG;
                *glType_ = GL_UNSIGNED_BYTE;
                break;
            case UNCOMPRESSED_R5G6B5:
                *glInternalFormat_ = GL_RGB565;
                *glFormat_ = GL_RGB;
                *glType_ = GL_UNSIGNED_SHORT_5_6_5;
                break;
            case UNCOMPRESSED_R8G8B8:
                *glInternalFormat_ = GL_RGB8;
                *glFormat_ = GL_RGB;
                *glType_ = GL_UNSIGNED_BYTE;
                break;
            case UNCOMPRESSED_R5G5B5A1:
                *glInternalFormat_ = GL_RGB5_A1;
                *glFormat_ = GL_RGBA;
                *glType_ = GL_UNSIGNED_SHORT_5_5_5_1;
                break;
            case UNCOMPRESSED_R4G4B4A4:
                *glInternalFormat_ = GL_RGBA4;
                *glFormat_ = GL_RGBA;
                *glType_ = GL_UNSIGNED_SHORT_4_4_4_4;
                break;
            case UNCOMPRESSED_R8G8B8A8:
                *glInternalFormat_ = GL_RGBA8;
                *glFormat_ = GL_RGBA;
                *glType_ = GL_UNSIGNED_BYTE;
                break;
            case UNCOMPRESSED_R32:
                if (m_GLSupportFlags[GL_TEX_FLOAT]) *glInternalFormat_ = GL_R32F;
                *glFormat_ = GL_RED;
                *glType_ = GL_FLOAT;
                break;
            case UNCOMPRESSED_R32G32B32:
                if (m_GLSupportFlags[GL_TEX_FLOAT]) *glInternalFormat_ = GL_RGB32F;
                *glFormat_ = GL_RGB;
                *glType_ = GL_FLOAT;
                break;
            case UNCOMPRESSED_R32G32B32A32:
                if (m_GLSupportFlags[GL_TEX_FLOAT]) *glInternalFormat_ = GL_RGBA32F;
                *glFormat_ = GL_RGBA;
                *glType_ = GL_FLOAT;
                break;
#endif
            case COMPRESSED_DXT1_RGB:
                if (m_GLSupportFlags[GL_COMP_DXT]) *glInternalFormat_ = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
                break;
            case COMPRESSED_DXT1_RGBA:
                if (m_GLSupportFlags[GL_COMP_DXT]) *glInternalFormat_ = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                break;
            case COMPRESSED_DXT3_RGBA:
                if (m_GLSupportFlags[GL_COMP_DXT]) *glInternalFormat_ = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                break;
            case COMPRESSED_DXT5_RGBA:
                if (m_GLSupportFlags[GL_COMP_DXT]) *glInternalFormat_ = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                break;
            case COMPRESSED_ETC1_RGB:
                if (m_GLSupportFlags[GL_COMP_ETC1]) *glInternalFormat_ = GL_ETC1_RGB8_OES;
                break; // NOTE: Requires OpenGL ES 2.0 or OpenGL 4.3
            case COMPRESSED_ETC2_RGB:
                if (m_GLSupportFlags[GL_COMP_ETC2]) *glInternalFormat_ = GL_COMPRESSED_RGB8_ETC2;
                break; // NOTE: Requires OpenGL ES 3.0 or OpenGL 4.3
            case COMPRESSED_ETC2_EAC_RGBA:
                if (m_GLSupportFlags[GL_COMP_ETC2]) *glInternalFormat_ = GL_COMPRESSED_RGBA8_ETC2_EAC;
                break; // NOTE: Requires OpenGL ES 3.0 or OpenGL 4.3
            case COMPRESSED_PVRT_RGB:
                if (m_GLSupportFlags[GL_COMP_PVRT]) *glInternalFormat_ = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
                break; // NOTE: Requires PowerVR GPU
            case COMPRESSED_PVRT_RGBA:
                if (m_GLSupportFlags[GL_COMP_PVRT]) *glInternalFormat_ = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
                break; // NOTE: Requires PowerVR GPU
            case COMPRESSED_ASTC_4x4_RGBA:
                if (m_GLSupportFlags[GL_COMP_ASTC]) *glInternalFormat_ = GL_COMPRESSED_RGBA_ASTC_4x4_KHR;
                break; // NOTE: Requires OpenGL ES 3.1 or OpenGL 4.3
            case COMPRESSED_ASTC_8x8_RGBA:
                if (m_GLSupportFlags[GL_COMP_ASTC]) *glInternalFormat_ = GL_COMPRESSED_RGBA_ASTC_8x8_KHR;
                break; // NOTE: Requires OpenGL ES 3.1 or OpenGL 4.3
            default:
                throw std::runtime_error("Unsupported format.");
        }
#endif
    }
}