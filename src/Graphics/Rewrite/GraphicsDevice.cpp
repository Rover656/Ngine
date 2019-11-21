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

#include "OpenGLDefines.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
    GraphicsDevice::GraphicsDevice() {
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

#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        // Send OpenGL Version to console
        const char *ver = (const char *)glGetString(GL_VERSION);
        ConsoleMessage("OpenGL Version: " + std::string(ver), "NOTICE", "GraphicsDevice");

        // Init support flags
        for (auto i = 0; i <= GL_VAO; i++) _GLSupportFlags[i] = false;

        // Load extensions
        int numExt = 0;
#if defined(GRAPHICS_OPENGL33)
        // Supported by default
        _GLSupportFlags[GL_VAO] = true;
        _GLSupportFlags[GL_TEX_NPOT] = true;
        _GLSupportFlags[GL_TEX_FLOAT] = true;
        _GLSupportFlags[GL_TEX_DEPTH] = true;

        // Get extension count
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

        // Allocate extensions list
        const char **extList = new const char*[numExt];

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

                if ((glGenVertexArrays != NULL) && (glBindVertexArray != NULL) && (glDeleteVertexArrays != NULL)) _GLSupportFlags[GL_VAO] = true;
            }

            // Check NPOT textures support
            if (strcmp(extList[i], "GL_OES_texture_npot") == 0) _GLSupportFlags[GL_TEX_NPOT] = true;

            // Check texture float support
            if (strcmp(extList[i], "GL_OES_texture_float") == 0) _GLSupportFlags[GL_TEX_FLOAT] = true;

            // Check depth texture support
            if ((strcmp(extList[i], "GL_OES_depth_texture") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_depth_texture") == 0)) _GLSupportFlags[GL_TEX_DEPTH] = true;

            if (strcmp(extList[i], "GL_OES_depth24") == 0) _GLMaxDepthBits = 24;
            if (strcmp(extList[i], "GL_OES_depth32") == 0) _GLMaxDepthBits = 32;
#endif
            // DDS texture compression support
            if ((strcmp(extList[i], "GL_EXT_texture_compression_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBKIT_WEBGL_compressed_texture_s3tc") == 0))
                _GLSupportFlags[GL_COMP_DXT] = true;

            // ETC1 texture compression support
            if ((strcmp(extList[i], "GL_OES_compressed_ETC1_RGB8_texture") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_etc1") == 0))
                _GLSupportFlags[GL_COMP_ETC1] = true;

            // ETC2/EAC texture compression support
            if (strcmp(extList[i], "GL_ARB_ES3_compatibility") == 0) _GLSupportFlags[GL_COMP_ETC2] = true;

            // PVR texture compression support
            if (strcmp(extList[i], "GL_IMG_texture_compression_pvrtc") == 0) _GLSupportFlags[GL_COMP_PVRT] = true;

            // ASTC texture compression support
            if (strcmp(extList[i], "GL_KHR_texture_compression_astc_hdr") == 0) _GLSupportFlags[GL_COMP_ASTC] = true;

            // Anisotropic texture filter
            if (strcmp(extList[i], (const char *) "GL_EXT_texture_filter_anisotropic") == 0) {
                _GLSupportFlags[GL_ANISOTROPIC_TEXTURE_FILTER] = true;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &_GLMaxAnisotropicLevel);
            }

            // Clamp mirror wrap mode supported
            if (strcmp(extList[i], (const char *) "GL_EXT_texture_mirror_clamp") == 0) _GLSupportFlags[GL_TEX_MIRROR_CLAMP] = true;
        }
#endif
        ConsoleMessage("Successfully loaded extensions.", "NOTICE", "GraphicsDevice");

        // Load default matrices
        _ProjectionMatrix = Matrix::Identity;
    }

    Matrix GraphicsDevice::GetProjectionMatrix() const {
        return _ProjectionMatrix;
    }

    bool GraphicsDevice::GetGLSupportFlag(GraphicsDevice::OpenGLFeature feature_) {
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        return _GLSupportFlags[feature_];
#endif
        return false;
    }

    float GraphicsDevice::GetGLMaxAnisotropicLevel() {
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        return _GLMaxAnisotropicLevel;
#endif
        return 0;
    }

    int GraphicsDevice::GetGLMaxDepthBits() {
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        return _GLMaxDepthBits;
#endif
        return 0;
    }
}