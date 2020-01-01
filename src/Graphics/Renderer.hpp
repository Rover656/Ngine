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
#include "Rendering/Renderable.hpp"
#include "GraphicsDevice.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#endif

#include "Texture2D.hpp"

namespace NerdThings::Ngine::Graphics {
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
        static const unsigned int VBO_SIZE = 65536;
        static const unsigned int QUAD_IBO_SIZE = VBO_SIZE * 6 / 4;

        enum PrimitiveType {
            PRIMITIVE_TRIANGLES = 0,
            PRIMITIVE_QUADS
        };
    private:
        struct BatchItem {
            int Depth = 0;
            PrimitiveType PrimitiveType = PRIMITIVE_TRIANGLES;
            std::vector<Rendering::VertexData> Vertices;
            int VertexCount = 0;
            ShaderProgram *Shader = nullptr;
            Texture2D *Texture = nullptr;
        };

        enum BatchBucket {
            Z_POS = 0,
            Z_NEU,
            Z_NEG
        };

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
         * Whether or not we are creating a batch.
         */
        bool m_midBatch = false;

        /**
         * The current batch item.
         */
        BatchItem m_currentBatch;

        /**
         * The current batch's target for error checking.
         */
        RenderTarget *m_currentBatchTarget = nullptr;

        /**
         * Batches (one per target).
         */
        std::map<RenderTarget *, std::vector<std::vector<BatchItem>>> m_batches;

        unsigned int m_triangleVAO;

        unsigned int m_triangleVBO;

        unsigned int m_quadVAO;

        unsigned int m_quadVBO;

        unsigned int m_quadIBO;

        /**
         * The current primitive type being rendered.
         */
        PrimitiveType m_currentPrimitiveType = PRIMITIVE_TRIANGLES;

        /**
         * This contains a list of all the vertices to be pushed to the framebuffer.
         */
        Rendering::VertexData m_vertices[VBO_SIZE];

        int m_vertexCount = 0;

        /**
         * This contains a list of items that are ready to be pushed to the framebuffer.
         */
        std::vector<BatchItem> m_processedItems;

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

        void _deleteBuffers();

        void _bindTriangleBuffers();

        void _bindQuadBuffers();

        void _writeBuffer(unsigned int vbo_);

        void _unbindBuffers();

        /**
         * Sort batch items.
         *
         * @param a_ The first item.
         * @param b_ The second item.
         * @return Whether A should be before B.
         */
        static bool _sortPredicate(const BatchItem &a_, const BatchItem &b_);

        /**
         * RenderBatched the batch for the given target.
         *
         * @param target_ The target to render for.
         */
        void _render(RenderTarget *target_);

        void _renderBucket(RenderTarget *target_, BatchBucket bucket_);

        void _processBatchItem(const BatchItem &item_);

        /**
         * Flush the buffers
         */
        void _flush();
    public:
        /**
         * Create a renderer.
         *
         * @param graphicsDevice_ The current graphics device.
         */
        explicit Renderer(GraphicsDevice *graphicsDevice_);
        ~Renderer();

        /**
         * Begin adding some vertices to the current framebuffer batch.
         */
        void Begin(PrimitiveType primitiveType_, Texture2D *texture_ = nullptr, int depth_ = 0, ShaderProgram *shader_ = nullptr);

        void Vertex(Vector2 pos_, Vector2 texCoord_, Color color_);

        /**
         * End this batch.
         */
        void End();

        /**
         * RenderBatched the current batch.
         */
        void RenderBatched();

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
#else
        // Internal OpenGL Methods
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)

        static void __DrawLine(Vector2 a_, Vector2 b_, Color c_, float thick_);

#endif
    public:
        // Public Methods

        /*
         * Begin a drawing loop
         */
        static void BeginDrawing();

        /*
         * Clear the screen with a color
         */
        static void Clear(Color color_);

        /*
         * Draw a pixel
         */
        static void DrawPixel(Vector2 position_, Color color_);

        /*
         * Draw line
         */
        static void DrawLine(Vector2 startPos_, Vector2 endPos_, Color color_,
                             float thickness_ = 1);

        /*
         * Draw a polygon outline from a point array
         */
        // TODO: Thickness for this too?
        static void DrawLineStrip(const std::vector<Vector2> &points_, Color color_, float lineThickness_ = 1);

        /*
         * Draw a color-filled circle
         */
        static void DrawCircle(Vector2 center_, float radius_, Color color_);

        /*
         * Draw a circle outline
         */
        static void DrawCircleLines(Vector2 center_, float radius_, Color color_, float lineThickness_ = 1);

        /*
         * Draw a sector of a circle
         */
        static void DrawCircleSector(Vector2 center_, float radius_, float startAngle_, float endAngle_,
                                     int segments_, Color color_);

        /*
         * Draw a circle sector outline
         */
        static void DrawCircleSectorLines(Vector2 center_, float radius_, float startAngle_, float endAngle_,
                                          int segments_, Color color_, float lineThickness_ = 1);

//        /*
//         * Draw a ring
//         */
//        static void DrawRing(Vector2 center_, float innerRadius_, float outerRadius_, int startAngle_,
//                             int endAngle_, int segments_, Color color_);
//
//        /*
//         * Draw a ring outline
//         */
//        static void DrawRingLines(Vector2 center_, float innerRadius_, float outerRadius_, int startAngle_,
//                                  int endAngle_, int segments_, Color color_);

        /*
         * Draw a rectangle
         */
        static void DrawRectangle(Vector2 position_, float width_, float height_, Color color_,
                                  float rotation_ = 0, Vector2 origin_ = Vector2());

        /*
         * Draw a rectangle
         */
        static void DrawRectangle(Vector2 position_, Vector2 size_, Color color_,
                                  float rotation_ = 0, Vector2 origin_ = Vector2());

        /*
         * Draw a rectangle with additional parameters
         */
        static void DrawRectangle(Rectangle rectangle_, Color color_, float rotation_ = 0,
                                  Vector2 origin_ = Vector2());

        /*
         * Draw a vertical-gradient-filled rectangle
         */
        static void DrawRectangleGradientV(Vector2 position_, float width_, float height_,
                                           Color color1_, Color color2_, float rotation_ = 0,
                                           Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a vertical-gradient-filled rectangle
         */
        static void DrawRectangleGradientV(Vector2 position_, Vector2 size_, Color color1_,
                                           Color color2_, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a vertical-gradient-filled rectangle
         */
        static void DrawRectangleGradientV(Rectangle rectangle_, Color color1_,
                                           Color color2_, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a horizonal-gradient-filled rectangle
         */
        static void DrawRectangleGradientH(Vector2 position_, float width_, float height_,
                                           Color color1_, Color color2_, float rotation_ = 0,
                                           Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a horizonal-gradient-filled rectangle
         */
        static void DrawRectangleGradientH(Vector2 position_, Vector2 size_, Color color1_,
                                           Color color2_, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a horizonal-gradient-filled rectangle
         */
        static void DrawRectangleGradientH(Rectangle rectangle_, Color color1_,
                                           Color color2_, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a rectangle with a color per vertex
         */
        static void DrawRectangleGradientEx(Vector2 position_, float width_, float height_,
                                            Color color1_, Color color2_, Color color3_,
                                            Color color4_, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a rectangle with a color per vertex
         */
        static void DrawRectangleGradientEx(Vector2 position_, Vector2 size_, Color color1_,
                                            Color color2_, Color color3_, Color color4_, float rotation_ = 0,
                                            Vector2 origin_ = Vector2::Zero);

        /*
         * Draw a rectangle with a color per vertex
         */
        static void DrawRectangleGradientEx(Rectangle rectangle_, Color color1_,
                                            Color color2_, Color color3_, Color color4_, float rotation_ = 0,
                                            Vector2 origin_ = Vector2::Zero);

        /*
         * Draw rectangle outline
         */
        static void DrawRectangleLines(Vector2 position_, float width_, float height_, Color color_,
                                       int lineThickness_ = 1);

        /*
         * Draw rectangle outline
         */
        static void DrawRectangleLines(Vector2 position_, Vector2 size_, Color color_,
                                       int lineThickness_ = 1);

        /*
         * Draw rectangle outline
         */
        static void DrawRectangleLines(Rectangle rectangle_, Color color_, int lineThickness_ = 1);

        /*
         * Draw text
         */
        static void DrawText(Font *font_, const std::string &string_, Vector2 position_,
                             float fontSize_, float spacing_, Color color_);

        // TODO: Work on these next 2 methods

        /*
         * Draw text with rectangle constraint
         */
        static void DrawTextRect(Font *font_, const std::string &string_,
                                 Rectangle rectangle_, float fontSize_, float spacing_,
                                 Color color_, bool wordWrap_ = true);

        /*
         * Draw text with rectangle constraint and select support
         */
        static void DrawTextRectEx(Font *font_, const std::string &string_,
                                   Rectangle rectangle_, float fontSize_, float spacing_,
                                   Color color_, int selectStart_, int selectLength_,
                                   Color selectText_, Color selectBack_, bool wordWrap_ = true);

        /*
         * Draw a texture
         */
        static void DrawTexture(Texture2D *texture_, Vector2 position_, Color color_,
                                float scale_ = 1, Vector2 origin_ = Vector2(), float rotation_ = 0);

        /*
         * Draw a texture with specified dimensions
         */
        static void DrawTexture(Texture2D *texture_, Vector2 position_, float width_,
                                float height_, Color color_, Vector2 origin_ = Vector2(),
                                float rotation_ = 0);

        /*
         * Draw a part of a texture
         */
        static void DrawTexture(Texture2D *texture_, Rectangle sourceRectangle_,
                                Vector2 position_, Color color_,
                                Vector2 origin_ = Vector2(), float rotation_ = 0);

        /*
         * Draw a part of a texture with specified dimensions
         */
        static void DrawTexture(Texture2D *texture_, Rectangle sourceRectangle_,
                                Vector2 position_, float width_, float height_, Color color_,
                                Vector2 origin_ = Vector2(), float rotation_ = 0);

        /*
         * Draw a texture with pro parameters
         */
        static void DrawTexture(Texture2D *texture_, Rectangle destRectangle_,
                                Rectangle sourceRectangle_, Color color_,
                                Vector2 origin_ = Vector2(), float rotation_ = 0);

        /*
         * Draw a triangle
         */
        static void DrawTriangle(Vector2 v1_, Vector2 v2_, Vector2 v3_,
                                 Color color_);

        /*
         * Draw a triangle outline
         */
        static void DrawTriangleLines(Vector2 v1_, Vector2 v2_, Vector2 v3_,
                                      Color color_, float lineThickness_ = 1);

        /*
         * Draw a triangle fan from a point array
         */
        static void DrawTriangleFan(const std::vector<Vector2> &points_, Color color_);

        /*
         * Draw a polygon
         */
        static void DrawPoly(Vector2 center_, int sides_, float radius_, float rotation_,
                             Color color_);

        /*
         * End a drawing loop.
         */
        static void EndDrawing();
#endif
    };
}

#endif //RENDERER_HPP
