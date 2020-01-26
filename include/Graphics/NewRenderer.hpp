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

#ifndef NEWRENDERER_HPP
#define NEWRENDERER_HPP

#include "../Config.hpp"

#include "Buffer.hpp"
#include "Renderer.hpp"
#include "Texture2D.hpp"
#include "VertexLayout.hpp"

namespace Ngine::Graphics {
    /**
     * The vertex data tool can do many useful things.
     * The tool can convert between primitive types (useful as the renderer only uses Triangles).
     * The tool can also generate vertices for basic things (such as a box) and give you them in Triangle form.
     *
     * @todo Add a tool that can split vertex arrays that are too large?
     */
    class NEAPI VertexDataTool {
    public:
        /**
         * Generate a rectangle using indexed triangles.
         *
         * @param width_ Rectangle width.
         * @param height_ Rectangle height.
         * @param vertexArray_ The vertex array to write to.
         * @param indexArray_ The index array to write to.
         */
        static void GenerateRect(float width_, float height_, Color color_, std::vector<Vertex> &vertexArray_, std::vector<unsigned short> &indexArray_);
    };

    /**
     * This is another new renderer, provides more basic features as we now render without immediate mode features.
     */
    class NEAPI NewRenderer {
        /**
         * The maximum number of triangle vertices per batch.
         *
         * @note 65535 because that is the maximum indexable by unsigned short.
         */
        static const int MAX_TRIANGLE_VERTICES = 65535;

        /**
         * The size of the matrix stack size.
         */
        static const int MATRIX_STACK_SIZE = 32;

        /**
         * The graphics device attached to the renderer.
         */
        GraphicsDevice *m_graphicsDevice = nullptr;

        /**
         * The batch vertex layout.
         */
        VertexLayout *m_layout;

        /**
         * The vertex buffer for the batch.
         */
        Buffer *m_VBO;

        /**
         * The index buffer for the batch.
         */
        Buffer *m_IBO;

        /**
         * The texture being used for the current batch
         */
        Texture2D *m_currentTexture = nullptr;

        /**
         * The shader being used for the current batch.
         */
        ShaderProgram *m_currentShader = nullptr;

        /**
         * The vertex array for the current batch.
         */
        Vertex m_vertexArray[MAX_TRIANGLE_VERTICES];

        /**
         * The vertex array counter.
         */
        int m_vertexCount = 0;

        /**
         * The index array for the current batch.
         */
        unsigned short m_indexArray[MAX_TRIANGLE_VERTICES];

        /**
         * The index array counter.
         */
        int m_indexCount = 0;

        /**
         * The default shader program for rendering.
         */
        ShaderProgram *m_defaultShader = nullptr;

        /**
         * White texture used for rendering things without texture.
         */
        Texture2D *m_whiteTexture = nullptr;

        /**
         * The matrix stack.
         */
        Matrix m_matrixStack[MATRIX_STACK_SIZE];

        /**
         * The matrix stack counter.
         */
        int m_matrixStackCounter = 0;
    public:
        NewRenderer(GraphicsDevice *graphicsDevice_);
        ~NewRenderer();

        /**
         * Add triangle vertices to the current batch.
         *
         * @param vertices_ Triangle vertices to add.
         * @param count_ The number of vertices in the array.
         */
        void AddTriangles(Vertex *vertices_, int count_);

        /**
         * Add indexed triangle vertices and indices.
         *
         * @param vertices_ Triangle vertices that are indexed by `indices_`
         * @param vCount_ The number of vertices in the array.
         * @param indices_ The indices for the vertex array.
         * @param iCount_ The number of indices in the array.
         */
        void AddIndexedTriangles(Vertex *vertices_, int vCount_, unsigned short *indices_, int iCount_);

        /**
         * Set the current texture for rendering.
         *
         * @note Will force a draw of anything before this call if the texture is different.
         * @param texture_ The texture to render with.
         */
        void SetTexture(Texture2D *texture_);

        /**
         * Set the current shader for rendering.
         *
         * @note Will force a draw of anything before this call if the texture is different.
         * @todo ShaderProgramState/Material.
         * @param shader_ The shader to use.
         */
        void SetShader(ShaderProgram *shader_);

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
        void MultiplyMatrix(const Matrix &mat_);

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

        /**
         * Trigger a render of the internal buffers.
         */
        void RenderBatch();

        /**
         * Render a vertex buffer.
         *
         * @param layout_ The vertex layout.
         * @param VBO_ The vertex buffer.
         * @param count_ Number of vertices to render.
         * @param texture_ The texture to render with.
         * @param shader_ The shader to render with.
         */
        void RenderBuffer(VertexLayout *layout_, Buffer *VBO_, int count_, Texture2D *texture_, ShaderProgram *shader_);

        /**
         * Render an indexed vertex buffer.
         *
         * @param layout_ The vertex layout.
         * @param VBO_ The vertex buffer.
         * @param IBO_ The index buffer.
         * @param count_ The number of elements. I.e. triangle count * 3
         * @param texture_ The texture to render with.
         * @param shader_ The shader to render with.
         */
        void RenderBufferIndexed(VertexLayout *layout_, Buffer *VBO_, Buffer *IBO_, int count_, Texture2D *texture_, ShaderProgram *shader_);

        /**
         * Get the graphics device attached to the renderer.
         *
         * @return The attached graphics device.
         */
        GraphicsDevice *GetGraphicsDevice();
    };
}

#endif //NEWRENDERER_HPP
