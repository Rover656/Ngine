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

#ifndef RENDERER_H
#define RENDERER_H

#include "../../Ngine.h"

#include "../Color.h"
#include "../Texture2D.h"
#include "GraphicsDevice.h"
#include "OpenGLDefines.h"
#include "QuadsObject.h"
#include "RenderObject.h"
#include "Shader.h"
#include "ShaderProgram.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
    /**
     * The rewritten Ngine renderer.
     * This properly implements all required features for much easier batching and depth sorting.
     * Inspired by the architecture of the cocos2d renderer.
     * @note This will probably not be completed by the end of the year. This must be enabled with the USE_EXPERIMENTAL_RENDERER flag.
     */
    class NEAPI Renderer {
    public:
        /**
         * Maximum size of the vertex buffer.
         */
        static const int MAX_VBO_SIZE = 65536;

        /**
         * Maximum size of the index buffer.
         */
        static const int MAX_INDEX_VBO_SIZE = MAX_VBO_SIZE * 6 / 4;
    private:
        enum RenderMode {
            MODE_QUADS,
            MODE_TRIANGLES
        };

        /**
         * A renderable batch item.
         * This is produced by the data from a `RenderObject`
         */
        struct RenderBatchItem {
            /**
             * The number of vertices for this item
             */
            unsigned int Size = 0;

            /**
             * The model matrix for translating it's vertices.
             */
            Matrix ModMat;
            const VertexData *VDat = nullptr;
            Texture2D *Texture = nullptr;
            ShaderProgram *Shader = nullptr;
            float Z = 0;
            RenderMode Mode;
        };

        /**
         * The graphics device.
         */
        GraphicsDevice *_GraphicsDevice;

        /**
         * The index to the buckets vector which contains sortable objects.
         * This details how the buckets should be sorted, and doesn't sort Z neutral objects.
         */
        enum RenderBucket {
            BUCKET_Z_NEGATIVE = 0,
            BUCKET_Z_NEUTRAL,
            BUCKET_Z_POSITIVE
        };

        /**
         * Render bucket queue.
         */
        std::vector<std::vector<RenderBatchItem>> _Queue;

        /**
         * Whether or not the renderer is rendering.
         * This is for thread safety.
         */
        bool _Rendering = false;

        /**
         * Whether or not 2D depth testing should be enabled.
         */
        bool _Enable2DDepthTest = false;

        /**
         * The current render mode, used for identifying what the batch consists of.
         */
        RenderMode _CurrentMode = MODE_TRIANGLES;

        /**
         * The current vertices for rendering.
         */
        VertexData _Vertices[MAX_VBO_SIZE];

        /**
         * The number of vertices ready to be rendered.
         */
        unsigned int _VertexCount = 0;

        /**
         * The currently batched items, next to be rendered.
         */
        std::vector<RenderBatchItem> _CurrentBatchItems;

        /**
         * The quad index batch.
         */
        GLushort _QuadIndices[MAX_INDEX_VBO_SIZE];

        /**
         * The quad rendering VAO.
         */
        GLuint _QuadVAO = 0;

        /**
         * The quad batch VBO's.
         */
        GLuint _QuadBuffersVBO[2];

        /**
         * The default shader.
         */
        Shader *_DefaultShader = nullptr;

        /**
         * The default shader program.
         */
        ShaderProgram *_DefaultShaderProgram = nullptr;

        /**
         * The default texture for use with the default shader.
         */
        Texture2D *_DefaultTexture = nullptr;

        /**
         * Create the OpenGL buffers
         */
        void __CreateBuffers();

        /**
         * Create the default shaders.
         */
        void __CreateDefaultShader();

        void __AddToQueue(RenderBatchItem i_);

        /**
         * Sort the object buckets.
         */
        void __SortQueue();

        /**
         * Depth sort predicate.
         *
         * @return Whether a's z-index is less than b's.
         */
        static bool __SortPredicate(Renderer::RenderBatchItem a_, Renderer::RenderBatchItem b_);

        /**
         * Process the queue of render buckets.
         */
        void __ProcessQueue();

        /**
         * Process one of the render buckets.
         *
         * @param bucket_ The bucket to render.
         */
        void __ProcessBucket(RenderBucket bucket_);

        void __ProcessBatch(RenderBatchItem i_);

        /**
         * Draw all batched quads.
         */
        void __DrawBatchedQuads();






    public:
        /**
         * Create a renderer.
         */
        Renderer(GraphicsDevice *graphicsDevice_);
        ~Renderer();

        /**
         * Clear the screen.
         */
        void Clear();

        /**
         * Set the clear color.
         *
         * @param clearColor_ The color to clear the screen with.
         */
        void SetClearColor(const Color &clearColor_);

        /**
         * Render every queued `RenderObject`.
         */
        void Render();

        /**
         * Clean all batches and queued buckets.
         */
        void Clean();

        /**
         * Flush quads if any have been batched.
         */
        void Flush();

        /**
         * Add a render object to the queue.
         */
        void AddRenderObject(RenderObject *obj_);

        /**
         * Should we depth test 2D geometry on the GPU?
         *
         * @return Whether or not 2D geometry is depth tested on the GPU.
         */
        bool GetDepthTest2D();

        /**
         * Set whether or not we should depth test 2D geometry.
         *
         * @param depthTest_ Whether or not 2D geometry is depth tested.
         */
        void SetDepthTest2D(bool depthTest_);
    };
}

#endif //RENDERER_H
