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
#include "../GraphicsDevice.h"
#include "../Texture2D.h"
#include "OpenGLDefines.h"
#include "QuadRenderable.h"
#include "Renderable.h"
#include "Shader.h"
#include "ShaderProgram.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
    /**
     * The rewritten Ngine renderer.
     * This properly implements all required features for much easier batching and depth sorting.
     * @note This will probably not be completed by the end of the year. This must be enabled with the USE_EXPERIMENTAL_RENDERER flag.
     */
    class NEAPI Renderer {
    public:
        static const unsigned int VBO_SIZE = 65536;
        static const unsigned int QUAD_IBO_SIZE = VBO_SIZE * 6 / 4;
    private:
        struct QuadBatchItem {
            unsigned int Count;
            QuadRenderable *Obj;
        };

        enum RenderBucket {
            BUCKET_Z_NEGATIVE = 0,
            BUCKET_Z_NEUTRAL,
            BUCKET_Z_POSITIVE
        };

        /**
         * The graphics device.
         */
        GraphicsDevice *m_graphicsDevice;

        /**
         * The quad batch vertex array.
         */
        GLuint m_quadVAO;

        /**
         * The quad batch vertex buffer.
         */
        GLuint m_quadVBO;

        /**
         * The quad batch index buffer.
         */
        GLuint m_quadIBO;

        /**
         * Quad vertex data.
         */
        VertexData m_quadVertices[VBO_SIZE];

        /**
         * The number of vertices used in the vertex data.
         */
        unsigned int m_quadVerticesCount = 0;

        /**
         * The current quad batch.
         */
        std::vector<QuadBatchItem> m_quads;

        /**
         * The default shader program.
         */
        ShaderProgram *m_defaultShaderProgram;

        /**
         * 1x1 white texture.
         */
        Texture2D *m_whiteTexture;

        /**
         * The render queues.
         */
        std::vector<Renderable *> m_renderQueue[BUCKET_Z_POSITIVE + 1];

        /**
         * Enable OpenGL capabilities.
         */
        void _enableGLCapabilities();

        /**
         * Create the default shader
         */
        void _createDefaultShader();

        /**
         * Create the quad buffers
         */
        void _createQuadBuffers();

        /**
         * Delete the quad buffers.
         */
        void _deleteQuadBuffers();

        /**
         * Push data to the quad buffers
         */
        void _updateQuadBuffers();

        /**
         * Bind the quad buffers
         */
        void _bindQuadBuffers();

        /**
         * Unbind the quad buffers
         */
        void _unbindQuadBuffers();

        /**
         * Render the quad buffers
         */
        void _renderQuadBuffers();

        /**
         * Sort a bucket.
         *
         * @param bucket_ Bucket to sort.
         */
        void _sortBucket(RenderBucket bucket_);

        /**
         * Sort predicate.
         *
         * @param a_ The first.
         * @param b_ The second.
         * @return Bleh.
         */
        static bool _sortPredicate(Renderable *a_, Renderable *b_);

        /**
         * Render the queue.
         */
        void _processQueue();

        /**
         * Render the given bucket.
         *
         * @param bucket_ The bucket to render.
         */
        void _processBucket(RenderBucket bucket_);

        /**
         * Process a renderable.
         *
         * @param renderable_ The renderable to process.
         */
        void _processRenderable(Renderable *renderable_);

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
        Renderer(GraphicsDevice *graphicsDevice_);
        ~Renderer();

        /**
         * Add a renderable to the batch.
         */
        void Add(Renderable *obj_);

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
    };
}

#endif //RENDERER_H
