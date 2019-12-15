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

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#ifndef OPENGL_H
#define OPENGL_H

#include "../../Config.hpp"

#include "../../Math.hpp"
#include "../Color.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"

/*
 * This file is sorted by function then alphabetically to make it more clear.
 * This will be done engine wide soon.
 */

namespace NerdThings::Ngine::Graphics::OpenGL {
    // Enums

    /*
     * OpenGL Matrix Mode
     */
    enum GLMatrixMode {
        /*
         * Null matrix mode, not supported
         */
        MATRIX_NULL = -1,

        /*
         * Model view matrix
         */
        MATRIX_MODELVIEW = 0x1700,

        /*
         * Projection matrix
         */
        MATRIX_PROJECTION = 0x1701
    };

    /*
     * OpenGL Version
     */
    enum GLVersion {
        /*
         * Unknown OpenGL Version.
         * Probably bad.
         */
        OPENGL_UNKNOWN = -1,

        /*
         * OpenGL 1.1 (Will probably not add)
         */
        OPENGL_11 = 0,

        /*
         * OpenGL 2.1
         */
        OPENGL_21 = 1,

        /*
         * OpenGL 3.3
         */
        OPENGL_33 = 2,

        /*
         * OpenGL ES2.0
         */
        OPENGL_ES2 = 3
    };

    /*
     * OpenGL Primitive Rendering Mode
     */
    enum GLPrimitiveMode {
        /*
         * Lines
         */
        PRIMITIVE_LINES = 0x0001,

        /*
         * Triangles
         */
        PRIMITIVE_TRIANGLES = 0x0004,

        /*
         * Quads
         */
        PRIMITIVE_QUADS = 0x0007
    };

    // Structures

    struct GLDynamicBuffer {
        int VCounter;
        int TCCounter;
        int CCounter;
        std::unique_ptr<float[]> Vertices;
        std::unique_ptr<float[]> TexCoords;
        std::unique_ptr<float[]> Colors;
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
        std::unique_ptr<unsigned int[]> Indices;
#elif defined(GRAPHICS_OPENGLES2)
        std::unique_ptr<unsigned short[]> Indices;
#endif
        std::unique_ptr<GLVertexArray> VAO;
        std::unique_ptr<GLBuffer> VBO[4];
    };

    /*
     * Information regarding a draw call
     */
    struct GLDrawCall {
        /*
         * Primitive mode used
         */
        GLPrimitiveMode Mode;

        /*
         * Number of vertices used
         */
        int VertexCount;

        /*
         * The vertex alignment
         */
        int VertexAlignment;

        /*
         * The attached texture
         */
        std::shared_ptr<GLTexture> Texture;
    };

    // Defines

#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)
#define MAX_BATCH_ELEMENTS 8192
#elif defined(GRAPHICS_OPENGLES2)
#define MAX_BATCH_ELEMENTS 2048
#endif

#define MAX_BATCH_BUFFERING 1
#define MAX_MATRIX_STACK_SIZE 32
#define MAX_DRAWCALL_REGISTERED 256

    /*
     * OpenGL API.
     * A C++ clone of rlgl, without the 3D methods.
     */
    class NEAPI GL {
        // Shader Related Fields

        /*
         * The current shader program
         */
        static std::shared_ptr<GLShaderProgram> _CurrentShaderProgram;

        /*
         * The default shader program
         */
        static std::shared_ptr<GLShaderProgram> _DefaultShaderProgram;

        // Draw Batching Related Fields

        /*
         * The current buffer index
         */
        static int _CurrentBuffer;

        /*
         * The current draw depth
         */
        static float _CurrentDepth; // TODO: Do we need this??

        /*
         * The draw call batch
         */
        static GLDrawCall _DrawCalls[];

        /*
         * The current draw call index
         */
        static int _DrawCounter;

        /*
         * The buffer batch
         */
        static GLDynamicBuffer _VertexData[MAX_BATCH_BUFFERING]; // TODO: Will we ever use these other batches?

        // Matrix Related Fields

        /*
         * The currently active matrix
         */
        static Matrix *_CurrentMatrix;

        /*
         * Current Matrix Mode
         */
        static GLMatrixMode _CurrentMatrixMode;

        /*
         * The Matrix stack
         */
        static Matrix _MatrixStack[MAX_MATRIX_STACK_SIZE];

        /*
         * The matrix stack counter
         */
        static int _MatrixStackCounter;

        /*
         * The modelview matrix
         */
        static Matrix _ModelView;

        /*
         * The projection matrix
         */
        static Matrix _Projection;

        /*
         * The transform matrix
         */
        static Matrix _TransformMatrix;

        /*
         * Whether or not to use the transformation matrix
         */
        static bool _UseTransformMatrix;

        // Internal Methods

        /*
         * Draw the internal buffers
         */
        static void DrawBuffersDefault();

        /*
         * Initialize the internal buffers
         */
        static void LoadBuffersDefault();

        /*
         * Initialize the default shader program
         */
        static void LoadDefaultShader();

        /*
         * Update the data in the internal buffers
         */
        static void UpdateBuffersDefault();

    public:
        // Feature Related Fields

        /*
         * Maximum anisotropic filtering level
         */
        static float MaxAnisotropicLevel;

        /*
         * Maximum number of bits in a depth buffer/texture. TODO: Do we need this?
         */
        static int MaxDepthBits;

        /*
         * Anisotropic filtering support
         */
        static bool TexAnisotropicFilterSupported;

        /*
         * DDS texture compression support
         */
        static bool TexCompDXTSupported;

        /*
         * ETC1 texture compression support
         */
        static bool TexCompETC1Supported;

        /*
         * ETC2/EAC texture compression support
         */
        static bool TexCompETC2Supported;

        /*
         * PVR texture compression support
         */
        static bool TexCompPVRTSupported;

        /*
         * ASTC texture compression support
         */
        static bool TexCompASTCSupported;

        /*
         * Depth texture support
         */
        static bool TexDepthSupported;

        /*
         * Texture float support
         */
        static bool TexFloatSupported;

        /*
         * Clamp mirror wrap mode support
         */
        static bool TexMirrorClampSupported;

        /*
         * NPOT texture support
         */
        static bool TexNPOTSupported;

        /*
         * Whether or not vertex arrays are supported
         */
        static bool VAOSupported;

        // Textures

        /*
         * Default white texture used for rendering
         */
        static std::shared_ptr<GLTexture> DefaultTexture;

        // Vertex Array Methods
        // This is only here to fix GLES2 issues

        /*
         * Generate a number of vertex arrays.
         * Intended for internal use. See OpenGL::GLVertexArray instead.
         */
        static void GenVertexArrays(int n, unsigned int *arrays);

        /*
         * Bind a vertex array.
         * Intended for internal use. See OpenGL::GLVertexArray instead.
         */
        static void BindVertexArray(unsigned int array);

        /*
         * Delete a number of vertex arrays.
         * Intended for internal use. See OpenGL::GLVertexArray instead.
         */
        static void DeleteVertexArrays(int n, unsigned int *arrays);

        // Matrix Related Methods

        /*
         * Load the identity matrix
         */
        static void LoadIdentity();

        /*
         * Set the matrix mode
         */
        static void MatrixMode(enum GLMatrixMode mode_);

        /*
         * Multiply the current matrix by another
         */
        static void MultMatrix(Matrix matrix_);

        /*
         * Create an orthographic matrix
         */
        static void Ortho(float left_, float right_, float bottom_, float top_, float znear_, float zfar_);

        /*
         * Pop a matrix from the stack
         */
        static void PopMatrix();

        /*
         * Push the current matrix to the stack
         */
        static void PushMatrix();

        /*
         * Apply a rotation to the current matrix
         */
        static void Rotate(float angle_, Vector3 axis_);

        /*
         * Apply a scale to the current matrix
         */
        static void Scale(Vector3 scale_);

        /*
         * Set the value of the current matrix
         */
        static void SetMatrix(Matrix matrix_);

        /*
         * Apply a translation to the current matrix
         */
        static void Translate(Vector3 translation_);

        // Vertex Methods

        /*
         * Begin a set of vertices
         */
        static void Begin(GLPrimitiveMode mode_);

        /*
         * Set vertex color
         */
        static void Color(Color color_);

        /*
         * End a set of vertices
         */
        static void End();

        /*
         * Stop using the current texture
         */
        static void StopUsingTexture();

        /*
         * Set vertex texture coordinate
         */
        static void TexCoord(Vector2 coord_);

        /*
         * Use a texture with this set of vertices
         */
        static void UseTexture(std::shared_ptr<GLTexture> texture_);

        /*
         * Set vertex position
         */
        static void Vertex(Vector2 pos_);

        /*
         * Set vertex position TODO: Do we remove this and go for the above only?
         */
        static void Vertex(Vector3 pos_);

        // Buffering Methods

        /*
         * Whether or not the buffer has reached it's batch limit
         */
        static bool AtBufferLimit(int vertexCount_);

        /*
         * Draw and empty the buffers
         */
        static void Draw();

        // Management Methods

        /*
         * Clean up OpenGL state
         */
        static void Cleanup();

        /*
         * Initialize OpenGL
         */
        static void Init();

        // Other OpenGL Methods

        /*
         * Clear the screen/framebuffer
         */
        static void Clear();

        /*
         * Set the clear color
         */
        static void ClearColor(Graphics::Color color_);

        /*
         * Get OpenGL equivalent texture formats
         */
        static void GetGLTextureFormats(int format_, unsigned int *glInternalFormat_, unsigned int *glFormat_, unsigned int *glType_);

        /*
         * Get the OpenGL Version
         */
        static GLVersion GetGLVersion();

        /*
         * Set the viewport
         */
        static void Viewport(int x_, int y_, int width_, int height_);
    };
}

#endif //OPENGL_H
#endif