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
    // Shader Related

    std::shared_ptr<GLShaderProgram> GL::_CurrentShaderProgram = nullptr;
    std::shared_ptr<GLShaderProgram> GL::_DefaultShaderProgram = nullptr;

    // Draw Batching Related Fields

    int GL::_CurrentBuffer = 0;
    float GL::_CurrentDepth = -1.0f;
    std::unique_ptr<GLDrawCall[]> _DrawCalls = nullptr; // TODO: Is this the best type to use??
    int GL::_DrawCounter = 0;
    GLDynamicBuffer GL::_VertexData[];

    // Matrix Related Fields

    TMatrix *GL::_CurrentMatrix = nullptr;
    GLMatrixMode GL::_CurrentMatrixMode = MATRIX_NULL;
    TMatrix GL::_MatrixStack[];
    int GL::_MatrixStackCounter = 0;
    TMatrix GL::_ModelView = TMatrix::Identity;
    TMatrix GL::_Projection = TMatrix::Identity;
    TMatrix GL::_TransformMatrix = TMatrix::Identity;
    bool GL::_UseTransformMatrix = false;

    // Feature Related Fields

    int GL::MaxDepthBits = 16;
    bool GL::TexCompDXTSupported = false;
    bool GL::TexCompETC1Supported = false;
    bool GL::TexCompETC2Supported = false;
    bool GL::TexCompPVRTSupported = false;
    bool GL::TexCompASTCSupported = false;
    bool GL::TexDepthSupported = false;
    bool GL::TexFloatSupported = false;
    bool GL::TexNPOTSupported = false;
    bool GL::VAOSupported = false;

    // Internal Methods

    void GL::LoadDefaultShader() {
        // Shader sources
        std::string vertexShaderSrc =
#if defined(GRAPHICS_OPENGLES2)
                "#version 100\n"
                "attribute vec3 vertexPosition;\n"
                "attribute vec2 vertexTexCoord;\n"
                "attribute vec4 vertexColor;\n"
                "varying vec2 fragTexCoord;\n"
                "varying vec4 fragColor;\n"
#elif defined(GRAPHICS_OPENGL33)
                "#version 330\n"
                "in vec3 vertexPosition;\n"
                "in vec2 vertexTexCoord;\n"
                "in vec4 vertexColor;\n"
                "out vec2 fragTexCoord;\n"
                "out vec4 fragColor;\n"
                #endif
                "uniform mat4 mvp;\n"
                "void main()\n"
                "{\n"
                "    fragTexCoord = vertexTexCoord;\n"
                "    fragColor = vertexColor;\n"
                "    gl_Position = mvp*vec4(vertexPosition, 1.0);\n"
                "}\n";
        std::string fragmentShaderSrc =
#if defined(GRAPHICS_OPENGLES2)
                "#version 100\n"
                "precision mediump float;\n"
                "varying vec2 fragTexCoord;\n"
                "varying vec4 fragColor;\n"
#else
                "#version 330\n"
                "in vec2 fragTexCoord;\n"
                "in vec4 fragColor;\n"
                "out vec4 finalColor;\n"
                #endif
                "uniform sampler2D texture;\n"
                "void main()\n"
                "{\n"
                #if defined(GRAPHICS_OPENGLES2)
                "    vec4 texelColor = texture2D(texture, fragTexCoord);\n" // NOTE: texture2D() is deprecated on OpenGL 3.3 and ES 3.0
                "    gl_FragColor = texelColor*fragColor;\n"
                #elif defined(GRAPHICS_OPENGL33)
                "    vec4 texelColor = texture2D(texture, fragTexCoord);\n"
                "    finalColor = texelColor*fragColor;\n"
                #endif
                "}\n";

        // Load shaders
        auto vertexShader = new GLShader(vertexShaderSrc, SHADER_VERTEX);
        if (vertexShader->IsDirty()) {
            delete vertexShader;
            throw std::runtime_error("ERROR, INTERNAL SHADER FAILED TO COMPILE!");
        }

        auto fragmentShader = new GLShader(fragmentShaderSrc, SHADER_FRAGMENT);
        if (vertexShader->IsDirty()) {
            delete vertexShader;
            delete fragmentShader;
            throw std::runtime_error("ERROR, INTERNAL SHADER FAILED TO COMPILE!");
        }

        // Load program
        _DefaultShaderProgram = std::make_shared<GLShaderProgram>(fragmentShader, vertexShader);

        // Delete shaders
        delete vertexShader;
        delete fragmentShader;

        if (_DefaultShaderProgram->IsDirty())
            throw std::runtime_error("ERROR, INTERNAL SHADER FAILED TO COMPILE!");
    }

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
        _CurrentShaderProgram = _DefaultShaderProgram;

        unsigned char pixels[4] = {255, 255, 255, 255};   // 1 pixel RGBA (4 bytes)
        _DefaultTexture = std::make_unique<Texture2D>(1, 1, pixels, UNCOMPRESSED_R8G8B8A8, 1);

        // Init draw calls tracking system
        _DrawCalls = std::make_unique<GLDrawCall[]>(MAX_DRAWCALL_REGISTERED);

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

    GLVersion GL::GetGLVersion() {
#if defined(GRAPHICS_OPENGL33)
        return OPENGL_33;
#elif defined(GRAPHICS_OPENGLES2)
        return OPENGL_ES2;
#endif
        return OPENGL_UNKNOWN;
    }
}