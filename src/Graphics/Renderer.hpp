/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "Color.hpp"
#include "Font.hpp"

#ifdef USE_EXPERIMENTAL_RENDERER
#include "GraphicsDevice.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#endif

#include "Texture2D.hpp"

namespace NerdThings::Ngine::Graphics {
    /**
     * Vertex data.
     * `Vector3` for Position, `Vector2` for tex coords and `Color` for color.
     */
    struct VertexData {
        Vector3 Position;
        Color Color;
        Vector2 TexCoords;
    };

    /**
     * Primitive type for graphics rendering.
     */
    enum PrimitiveType {
        /**
         * Render the vertices as triangles.
         */
        PRIMITIVE_TRIANGLES = 0,

        /**
         * Draw a fan of triangles.
         * The first vertex is used as the first vertex of every triangle.
         * A triangle is formed for each *adjacent* vertex pair.
         * For example, the output of triangles would be `(0, 1, 2), (0, 2, 3), (0, 3, 4)` etc.
         */
        PRIMITIVE_TRIANGLE_FAN,

        /**
         * Render the vertices as quads.
         */
        PRIMITIVE_QUADS
    };

#ifdef USE_EXPERIMENTAL_RENDERER
    /**
     * The rewritten Ngine renderer.
     * This properly implements all required features for much easier batching and depth sorting.
     * @note This will probably not be completed by the end of the year. This must be enabled with the CMake FEATURE_EXPERIMENTAL_RENDERER flag.
     */
#else
    /**
     * Rendering class. Provides all drawing functions.
     * There is no concept of depth. First drawn is furthest behind.
     *
     * @todo Change this from a static class to an instance class so that the transition to the new renderer is easier later.
     * @warning This API will be replaced once the new Ngine renderer is ready. Enable it by defining USE_EXPERIMENTAL_RENDERER.
     */
#endif
    class NEAPI Renderer {
#ifdef USE_EXPERIMENTAL_RENDERER
    public:
        /**
         * The size of the internal VBOs.
         * The maximum number of vertices each batch.
         */
        static const unsigned int VBO_SIZE = 32768;

        /**
         * The maximum number of matrices on the stack.
         */
        static const unsigned int MATRIX_STACK_SIZE = 32;
    private:
        /**
         * The graphics device.
         */
        GraphicsDevice *m_graphicsDevice = nullptr;

        /**
         * The default shader program.
         */
        ShaderProgram *m_defaultShaderProgram = nullptr;

        /**
         * 1x1 white texture.
         */
        Texture2D *m_whiteTexture = nullptr;

        /**
         * Flag to fix threading problems.
         */
        bool m_rendering = false;

        /**
         * Whether or not we are adding to the vertex array.
         */
        bool m_midBatch = false;

        /**
         * The VAO used for rendering.
         */
        unsigned int m_VAO;

        /**
         * The VBO used to store vertex data.
         */
        unsigned int m_VBO;

        /**
         * This contains a list of all the vertices to be pushed into the VBO.
         */
        VertexData m_vertices[VBO_SIZE];

        /**
         * The number of vertices to be pushed from the array
         */
        int m_vertexCount = 0;

        /**
         * The currently applied shader program.
         *
         * @todo Replace with ShaderProgramState when added.
         */
        ShaderProgram *m_currentShader;

        /**
         * The currently applied texture.
         */
        Texture2D *m_currentTexture;

        /**
         * The current primitive type being rendered.
         */
        PrimitiveType m_currentPrimitiveType = PRIMITIVE_TRIANGLES;

        Matrix m_matrixStack[MATRIX_STACK_SIZE];

        int m_matrixStackCounter = 0;

        /**
         * The temporary vertex data from immediate mode calls.
         */
        VertexData m_tempVertexData[VBO_SIZE];

        /**
         * The number of vertices from immediate mode calls.
         */
        int m_tempVertexDataCount = 0;

        /**
         * Enable OpenGL capabilities.
         */
        void _enableGLCapabilities();

        /**
         * Create the default shader
         */
        void _createDefaultShader();

        /**
         * Create the buffers needed for rendering.
         */
        void _createBuffers();

        /**
         * Delete the rendering buffers.
         */
        void _deleteBuffers();

        /**
         * Bind the rendering buffers.
         */
        void _bindBuffers();

        /**
         * Unbind the buffers
         */
        void _unbindBuffers();

        /**
         * Write the vertices to the buffer.
         */
        void _writeBuffer();

        /**
         * Add a vertex array to the buffer.
         *
         * @note This converts any primitive type to triangles.
         * @param type_ The primitive type.
         * @param vertices_ The vertex array.
         * @param count_ The size of the array.
         */
        void _addVertices(PrimitiveType type_, VertexData *vertices_, int count_);

        /**
         * Render the batch to the display.
         */
        void _renderBatch();
    public:
        /**
         * Create a renderer.
         *
         * @param graphicsDevice_ The current graphics device.
         */
        explicit Renderer(GraphicsDevice *graphicsDevice_);
        ~Renderer();

        /**
         * Add a vertex array to the renderer batch.
         *
         * @param vertices_ The vertex array to add.
         * @param primitiveType_ The primitive type.
         * @param texture_ The texture to render with.
         * @param shader_ The shader to render with.
         */
        void Add(std::vector<VertexData> vertices_, PrimitiveType primitiveType_, Texture2D *texture_ = nullptr, ShaderProgram *shader_ = nullptr);

        /**
         * Begin adding some vertices to the current framebuffer batch.
         *
         * @param primitiveType_ The primitive type.
         * @param texture_ The texture to render with.
         * @param shader_ The shader to render with.
         */
        void Begin(PrimitiveType primitiveType_, Texture2D *texture_ = nullptr, ShaderProgram *shader_ = nullptr);

        /**
         * Push some vertex data to the batch.
         *
         * @param vertexData_ The data to push.
         */
        void Vertex(VertexData vertexData_);

        /**
         * Create a new vertex and push it to the batch.
         *
         * @param pos_ Vertex position.
         * @param texCoord_ Vertex texture coord.
         * @param color_ Vertex color.
         */
        void Vertex(const Vector2 &pos_, const Vector2 &texCoord_, const Color &color_);

        /**
         * End the current batch.
         */
        void End();

        /**
         * Render the current batch.
         */
        void Render();

        /**
         * Clear the framebuffer.
         */
        void Clear();

        /**
         * Set the clear color.
         *
         * @param color_ The color to clear with.
         */
        void SetClearColor(Color color_);

        /**
         * Push a matrix onto the stack (with the current matrix's value).
         *
         * @note This is extremely handy for moving vertices around. This **only** supports the immediate mode (Begin, Vertex, End) workflow at the moment.
         */
        void PushMatrix();

        /**
         * Pop a matrix from the stack.
         *
         * @warning Will throw if called when there's nothing to pop.
         */
        void PopMatrix();

        /**
         * Set the value of the current Matrix.
         *
         * @warning Remember to push first, this could destroy critical existing data. General rule of thumb: Push if you modify something.
         * @param mat_ The new value for the matrix.
         */
        void SetMatrix(const Matrix &mat_);

        /**
         * Load the identity matrix to the current matrix.
         *
         * @see `SetMatrix`
         */
        void LoadIdentity();

        /**
         * Multiply the given matrix by the current one.
         *
         * @note Formula is as follows: Current Matrix = Matrix Provided * Current Matrix
         */
        void MultMatrix(const Matrix &mat_);

        /**
         * Multiply the current matrix with a translation matrix.
         *
         * @param translation_ The translation to apply.
         */
        void Translate(const Vector3 &translation_);

        /**
         * Multiply the current matrix with a rotation matrix.
         *
         * @param rotation_ Rotation to apply
         * @param axis_ Axis in which to apply rotation.
         */
        void Rotate(const Angle &rotation_, const Vector3 &axis_);

        /**
         * Multiply the current matrix with a scale matrix.
         *
         * @param scale_ The scale to apply.
         */
        void Scale(const Vector3 &scale_);
#else
        // Internal OpenGL Methods
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)

        void __DrawLine(Vector2 a_, Vector2 b_, Color c_, float thick_);

#endif

        GraphicsDevice *m_graphicsDevice = nullptr;
    public:
        Renderer(GraphicsDevice *graphicsDevice_) : m_graphicsDevice(graphicsDevice_) {}
        ~Renderer() = default;

        /*
         * Begin a drawing loop
         */
        void BeginDrawing();

        /*
         * Clear the screen with a color
         */
        void Clear(Color color_);

        /*
         * Draw a pixel
         */
        void DrawPixel(Vector2 position_, Color color_);

        /*
         * Draw line
         */
        void DrawLine(Vector2 startPos_, Vector2 endPos_, Color color_,
                             float thickness_ = 1);

        /*
         * Draw a polygon outline from a point array
         */
        // TODO: Thickness for this too?
        void DrawLineStrip(const std::vector<Vector2> &points_, Color color_, float lineThickness_ = 1);

        /*
         * Draw a color-filled circle
         */
        void DrawCircle(Vector2 center_, float radius_, Color color_);

        /*
         * Draw a circle outline
         */
        void DrawCircleLines(Vector2 center_, float radius_, Color color_, float lineThickness_ = 1);

        /*
         * Draw a sector of a circle
         */
        void DrawCircleSector(Vector2 center_, float radius_, float startAngle_, float endAngle_,
                                     int segments_, Color color_);

        /*
         * Draw a circle sector outline
         */
        void DrawCircleSectorLines(Vector2 center_, float radius_, float startAngle_, float endAngle_,
                                          int segments_, Color color_, float lineThickness_ = 1);

//        /*
//         * Draw a ring
//         */
//        void DrawRing(Vector2 center_, float innerRadius_, float outerRadius_, int startAngle_,
//                             int endAngle_, int segments_, Color color_);
//
//        /*
//         * Draw a ring outline
//         */
//        void DrawRingLines(Vector2 center_, float innerRadius_, float outerRadius_, int startAngle_,
//                                  int endAngle_, int segments_, Color color_);

        /*
         * Draw a rectangle
         */
        void DrawRectangle(Vector2 position_, float width_, float height_, Color color_,
                                  float rotation_ = 0, Vector2 origin_ = Vector2());

        /*
         * Draw a rectangle
         */
        void DrawRectangle(Vector2 position_, Vector2 size_, Color color_,
                                  float rotation_ = 0, Vector2 origin_ = Vector2());

        /*
         * Draw a rectangle with additional parameters
         */
        void DrawRectangle(Rectangle rectangle_, Color color_, float rotation_ = 0,
                                  Vector2 origin_ = Vector2());

        /*
         * Draw a vertical-gradient-filled rectangle
         */
        void DrawRectangleGradientV(Vector2 position_, float width_, float height_,
                                           Color color1_, Color color2_, float rotation_ = 0,
                                           Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a vertical-gradient-filled rectangle
         */
        void DrawRectangleGradientV(Vector2 position_, Vector2 size_, Color color1_,
                                           Color color2_, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a vertical-gradient-filled rectangle
         */
        void DrawRectangleGradientV(Rectangle rectangle_, Color color1_,
                                           Color color2_, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a horizonal-gradient-filled rectangle
         */
        void DrawRectangleGradientH(Vector2 position_, float width_, float height_,
                                           Color color1_, Color color2_, float rotation_ = 0,
                                           Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a horizonal-gradient-filled rectangle
         */
        void DrawRectangleGradientH(Vector2 position_, Vector2 size_, Color color1_,
                                           Color color2_, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a horizonal-gradient-filled rectangle
         */
        void DrawRectangleGradientH(Rectangle rectangle_, Color color1_,
                                           Color color2_, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a rectangle with a color per vertex
         */
        void DrawRectangleGradientEx(Vector2 position_, float width_, float height_,
                                            Color color1_, Color color2_, Color color3_,
                                            Color color4_, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a rectangle with a color per vertex
         */
        void DrawRectangleGradientEx(Vector2 position_, Vector2 size_, Color color1_,
                                            Color color2_, Color color3_, Color color4_, float rotation_ = 0,
                                            Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a rectangle with a color per vertex
         */
        void DrawRectangleGradientEx(Rectangle rectangle_, Color color1_,
                                            Color color2_, Color color3_, Color color4_, float rotation_ = 0,
                                            Vector2 origin_ = Vector2::Zero);

        /*
         * Draw rectangle outline
         */
        void DrawRectangleLines(Vector2 position_, float width_, float height_, Color color_,
                                       int lineThickness_ = 1);

        /*
         * Draw rectangle outline
         */
        void DrawRectangleLines(Vector2 position_, Vector2 size_, Color color_,
                                       int lineThickness_ = 1);

        /*
         * Draw rectangle outline
         */
        void DrawRectangleLines(Rectangle rectangle_, Color color_, int lineThickness_ = 1);

        /*
         * Draw text
         */
        void DrawText(Font *font_, const std::string &string_, Vector2 position_,
                             float fontSize_, float spacing_, Color color_);

        // TODO: Work on these next 2 methods

        /*
         * Draw text with rectangle constraint
         */
        void DrawTextRect(Font *font_, const std::string &string_,
                                 Rectangle rectangle_, float fontSize_, float spacing_,
                                 Color color_, bool wordWrap_ = true);

        /*
         * Draw text with rectangle constraint and select support
         */
        void DrawTextRectEx(Font *font_, const std::string &string_,
                                   Rectangle rectangle_, float fontSize_, float spacing_,
                                   Color color_, int selectStart_, int selectLength_,
                                   Color selectText_, Color selectBack_, bool wordWrap_ = true);

        /*
         * Draw a texture
         */
        void DrawTexture(Texture2D *texture_, Vector2 position_, Color color_,
                                float scale_ = 1, Vector2 origin_ = Vector2(), float rotation_ = 0);

        /*
         * Draw a texture with specified dimensions
         */
        void DrawTexture(Texture2D *texture_, Vector2 position_, float width_,
                                float height_, Color color_, Vector2 origin_ = Vector2(),
                                float rotation_ = 0);

        /*
         * Draw a part of a texture
         */
        void DrawTexture(Texture2D *texture_, Rectangle sourceRectangle_,
                                Vector2 position_, Color color_,
                                Vector2 origin_ = Vector2(), float rotation_ = 0);

        /*
         * Draw a part of a texture with specified dimensions
         */
        void DrawTexture(Texture2D *texture_, Rectangle sourceRectangle_,
                                Vector2 position_, float width_, float height_, Color color_,
                                Vector2 origin_ = Vector2(), float rotation_ = 0);

        /*
         * Draw a texture with pro parameters
         */
        void DrawTexture(Texture2D *texture_, Rectangle destRectangle_,
                                Rectangle sourceRectangle_, Color color_,
                                Vector2 origin_ = Vector2(), float rotation_ = 0);

        /*
         * Draw a triangle
         */
        void DrawTriangle(Vector2 v1_, Vector2 v2_, Vector2 v3_,
                                 Color color_);

        /*
         * Draw a triangle outline
         */
        void DrawTriangleLines(Vector2 v1_, Vector2 v2_, Vector2 v3_,
                                      Color color_, float lineThickness_ = 1);

        /*
         * Draw a triangle fan from a point array
         */
        void DrawTriangleFan(const std::vector<Vector2> &points_, Color color_);

        /*
         * Draw a polygon
         */
        void DrawPoly(Vector2 center_, int sides_, float radius_, float rotation_,
                             Color color_);

        /*
         * End a drawing loop.
         */
        void EndDrawing();
#endif

        /**
         * Get the graphics device used by the renderer.
         *
         * @return The graphics device.
         */
        GraphicsDevice *GetGraphicsDevice() {
            return m_graphicsDevice;
        }
    };
}

#endif //RENDERER_HPP
