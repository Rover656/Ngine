#include "OpenGL.h"

// Platform specifics
#if defined(GRAPHICS_OPENGL33)
#include <glad/glad.h>
#elif defined(GRAPHICS_OPENGLES2)
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
#endif

#if defined(PLATFORM_DESKTOP)
    #include <GLFW/glfw3.h>
#elif defined(PLATFORM_UWP)
    #include <angle_windowsstore.h>
#endif

#include <cstring>

// Fix missing definitions (Straight from rlgl)
#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION         0x8B8C
#endif

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT     0x83F0
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT    0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT    0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT    0x83F3
#endif
#ifndef GL_ETC1_RGB8_OES
#define GL_ETC1_RGB8_OES                    0x8D64
#endif
#ifndef GL_COMPRESSED_RGB8_ETC2
#define GL_COMPRESSED_RGB8_ETC2             0x9274
#endif
#ifndef GL_COMPRESSED_RGBA8_ETC2_EAC
#define GL_COMPRESSED_RGBA8_ETC2_EAC        0x9278
#endif
#ifndef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG  0x8C00
#endif
#ifndef GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8C02
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_4x4_KHR
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR     0x93b0
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_8x8_KHR
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR     0x93b7
#endif

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT   0x84FF
#endif

#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT       0x84FE
#endif

#if defined(GRAPHICS_OPENGLES2)
#define glClearDepth                glClearDepthf
    #define GL_READ_FRAMEBUFFER         GL_FRAMEBUFFER
    #define GL_DRAW_FRAMEBUFFER         GL_FRAMEBUFFER
#endif

// GLES2 extensions
#if defined(GRAPHICS_OPENGLES2)
// NOTE: VAO functionality is exposed through extensions (OES)
static PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
static PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
static PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
#endif

namespace NerdThings::Ngine::Graphics::OpenGL {
    ////////
    // GL Class
    ////////

    // Matrix Related Fields

    TMatrix *GL::_CurrentMatrix = nullptr;
    MatrixMode GL::_CurrentMatrixMode = MATRIX_NULL;
    TMatrix GL::_MatrixStack[];
    int GL::_MatrixStackCounter = 0;
    TMatrix GL::_ModelView = TMatrix::Identity;
    TMatrix GL::_Projection = TMatrix::Identity;
    TMatrix GL::_TransformMatrix = TMatrix::Identity;
    bool GL::_UseTransformMatrix = false;

    // Management Methods

    void GL::Init() {
        // Init GLAD
#if defined(GRAPHICS_OPENGL33)
        // Init glad for the first time
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwTerminate();
            throw std::runtime_error("Failed to init GLAD.");
        }
#endif

        // Load extensions
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        int numExt = 0;

#if defined(GRAPHICS_OPENGL33)
        // Supported by default
        VAOSupported = true;

        // Default supported
        TexNPOTSupported = true;
        TexFloatSupported = true;
        TexDepthSupported = true;

        // Get extension count
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

        // Allocate extensions list
        const char **extList = new const char*[numExt];

        // Get strings
        for (auto i = 0; i < numExt; i++) extList[i] = (char *)glGetStringi(GL_EXTENSIONS, i);
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
#if defined (GRAPHICS_OPENGLES2)
            // Check for VAO support

            if (strcmp(extList[i], "GL_OES_vertex_array_object") == 0)
            {
                // The extension is supported by our hardware and driver, try to get related functions pointers
                glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
                glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
                glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");

                if ((glGenVertexArrays != NULL) && (glBindVertexArray != NULL) && (glDeleteVertexArrays != NULL)) VAOSupported = true;
            }

            // Check NPOT textures support
            if (strcmp(extList[i], "GL_OES_texture_npot") == 0) TexNPOTSupported = true;

            // Check texture float support
            if (strcmp(extList[i], "GL_OES_texture_float") == 0) TexFloatSupported = true;

            // Check depth texture support
            if ((strcmp(extList[i], "GL_OES_depth_texture") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_depth_texture") == 0)) TexDepthSupported = true;

            if (strcmp(extList[i], "GL_OES_depth24") == 0) MaxDepthBits = 24;
            if (strcmp(extList[i], "GL_OES_depth32") == 0) MaxDepthBits = 32;
#endif
            // DDS texture compression support
            if ((strcmp(extList[i], "GL_EXT_texture_compression_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_s3tc") == 0) ||
                (strcmp(extList[i], "GL_WEBKIT_WEBGL_compressed_texture_s3tc") == 0)) TexCompDXTSupported = true;

            // ETC1 texture compression support
            if ((strcmp(extList[i], "GL_OES_compressed_ETC1_RGB8_texture") == 0) ||
                (strcmp(extList[i], "GL_WEBGL_compressed_texture_etc1") == 0)) TexCompETC1Supported = true;

            // ETC2/EAC texture compression support
            if (strcmp(extList[i], "GL_ARB_ES3_compatibility") == 0) TexCompETC2Supported = true;

            // PVR texture compression support
            if (strcmp(extList[i], "GL_IMG_texture_compression_pvrtc") == 0) TexCompPVRTSupported = true;

            // ASTC texture compression support
            if (strcmp(extList[i], "GL_KHR_texture_compression_astc_hdr") == 0) TexCompASTCSupported = true;
        }
#endif

        // Load shader
        LoadDefaultShader();
        _CurrentShader = _DefaultShader;

        unsigned char pixels[4] = {255, 255, 255, 255};   // 1 pixel RGBA (4 bytes)
        _DefaultTexture = std::make_unique<Texture2D>(1, 1, pixels, UNCOMPRESSED_R8G8B8A8, 1);

        // Init draw calls tracking system
        _DrawCalls = std::make_unique<DrawCall[]>(MAX_DRAWCALL_REGISTERED);

        for (int i = 0; i < MAX_DRAWCALL_REGISTERED; i++) {
            _DrawCalls[i].Mode = PRIMITIVE_QUADS;
            _DrawCalls[i].VertexCount = 0;
            _DrawCalls[i].VertexAlignment = 0;
            _DrawCalls[i].TextureID = _DefaultTexture->ID;
        }

        _DrawCounter = 1;

        LoadBuffersDefault();

        _Projection = TMatrix::Identity;
        _ModelView = TMatrix::Identity;
        _CurrentMatrix = &_ModelView;

        // Init OpenGL states
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        // Clear
        glClearDepth(1.0f);
        ClearColor(TColor(0, 0, 0, 1));
        Clear();
    }

    // Other OpenGL Methods

    OpenGLVersion GL::GetGLVersion() {
#if defined(GRAPHICS_OPENGL33)
        return OPENGL_33;
#elif defined(GRAPHICS_OPENGLES2)
        return OPENGL_ES2;
#endif
        return OPENGL_UNKNOWN;
    }
}