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

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "Color.hpp"
#include "Font.hpp"
#include "GraphicsDevice.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"

namespace Ngine::Graphics {
    /**
     * Vertex data.
     * `Vector3` for Position, `Vector2` for tex coords and `Color` for color.
     */
    struct Vertex {
        Vector3 Position;
        Vector2 TexCoords;
        Color Color;
    };

    /**
     * Primitive type for graphics rendering.
     */
    enum class PrimitiveType {
        /**
         * Render the vertices as triangles.
         */
        Triangles = 0,

        /**
         * Draw a fan of triangles.
         * The first vertex is used as the first vertex of every triangle.
         * A triangle is formed for each *adjacent* vertex pair.
         * For example, the output of triangles would be `(0, 1, 2), (0, 2, 3), (0, 3, 4)` etc.
         */
        TriangleFan,

        /**
         * Render the vertices as quads.
         */
        Quads
    };

    /**
     * The Ngine renderer.
     *
     * @todo Make the renderer platform agnostic so that its easier to switch graphics API
     * @todo Add scissor/stencil tests
     */
    class NEAPI Renderer {
    public:
        /**
         * The maximum size of elements per buffer allowed.
         */
        static const unsigned int MAX_BUFFER_SIZE = 65535;

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
         * The IBO used to store index data.
         */
        unsigned int m_IBO;

        /**
         * This contains a list of all the vertices to be pushed into the VBO.
         */
        Vertex m_vertices[MAX_BUFFER_SIZE];

        /**
         * The number of vertices to be pushed from the array
         */
        int m_vertexCount = 0;

        /**
         * This contains all of the indices for being written to the buffer.
         */
        unsigned short m_indices[MAX_BUFFER_SIZE];

        /**
         * The number of indices to be written.
         */
        int m_indexCount = 0;

        /**
         * The matrix stack.
         */
        Matrix m_matrixStack[MATRIX_STACK_SIZE];

        /**
         * The matrix stack counter.
         */
        int m_matrixStackCounter = 0;

        /**
         * The temporary vertex data from immediate mode calls.
         */
        Vertex m_tempVertexData[MAX_BUFFER_SIZE];

        /**
         * The number of vertices from immediate mode calls.
         */
        int m_tempVertexDataCount = 0;

        // Below this, any fields here are just for immediate mode rendering

        /**
         * The default shader program.
         */
        ShaderProgram *m_defaultShaderProgram = nullptr;

        /**
         * The currently applied shader program.
         *
         * @todo Replace with ShaderProgramState when added.
         */
        ShaderProgram *m_currentShader;

        /**
         * The currently applied texture.
         */
        unsigned int m_currentTexture;

        /**
         * The current primitive type being rendered.
         */
        PrimitiveType m_currentPrimitiveType = PrimitiveType::Triangles;

        // END SEGMENTATION

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
         * Write the data to a buffer.
         * Buffer max size may be reset to `MAX_BUFFER_SIZE`.
         */
        static void _writeBuffer(unsigned int buffer_, unsigned int bufferType_, void *data_, int size_, int count_);

        /**
         * Add a vertex array to the buffer.
         *
         * @note This converts any primitive type to triangles.
         * @param type_ The primitive type.
         * @param vertices_ The vertex array.
         * @param count_ The size of the array.
         */
        void _addVertices(PrimitiveType type_, Vertex *vertices_, int count_);

        /**
         * Add a set of indexed vertices.
         *
         * @warning Only supports Triangles at the moment.
         * @todo Implement index conversion.
         */
        void _addIndexedVertices(PrimitiveType type_, Vertex *vertices_, int vCount_, unsigned short *indices_,
                                 int iCount_);

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
        void Add(std::vector<Vertex> vertices_, PrimitiveType primitiveType_, Texture2D *texture_ = nullptr,
                 ShaderProgram *shader_ = nullptr);

        /**
         * Add a vertex array to the renderer batch.
         *
         * @param vertices_ The vertex array to add.
         * @param primitiveType_ The primitive type.
         * @param texture_ The texture to render with.
         * @param shader_ The shader to render with.
         */
        void Add(std::vector<Vertex> vertices_, PrimitiveType primitiveType_, unsigned int texture_ = 0,
                 ShaderProgram *shader_ = nullptr);

        /**
         * Add indexed vertices to the renderer batch.
         *
         * @param vertices_ The vertex array.
         * @param indices_ The index array.
         * @param primitiveType_ The primitive type.
         * @param texture_ The texture to render with.
         * @param shader_ The shader to render with.
         */
        void AddIndexed(std::vector<Vertex> vertices_, std::vector<unsigned short> indices_,
                        PrimitiveType primitiveType_, Texture2D *texture_ = nullptr, ShaderProgram *shader_ = nullptr);

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
        void Vertex(Vertex vertexData_);

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

        /**
         * Check if count_ of type_ will fit inside of a buffer.
         *
         * @param type_ The primitive type.
         * @param count_ The number of primitives.
         * @return Whether or not a buffer can hold this primitive.
         */
        bool CheckSize(PrimitiveType type_, int count_);

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
