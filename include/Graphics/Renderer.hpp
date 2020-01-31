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
#include "Buffer.hpp"
#include "Color.hpp"
#include "Font.hpp"
#include "GraphicsDevice.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include "VertexLayout.hpp"

namespace Ngine::Graphics {
    /**
     * Vertex data.
     * `Vector3` for Position, `Vector2` for tex coords and `Color` for color.
     */
    struct Vertex {
        Vector3 Position;
        Vector2 TexCoords;
        Graphics::Color Color;
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
         * The maximum number of triangle vertices per batch.
         *
         * @note 65535 because that is the maximum indexable by unsigned short.
         */
        static const int MAX_TRIANGLE_VERTICES = 65535;

        /**
         * The size of the matrix stack size.
         */
        static const int MATRIX_STACK_SIZE = 32;

        // TEMP: GLES shaders for each version. I want to have a single shader language for this somepoint (either HLSL converter or something else)
        static constexpr const char *GL21_V_SHADER = "#version 120\n"
                                                     "attribute vec3 NG_VertexPos;\n"
                                                     "attribute vec2 NG_VertexTexCoord;\n"
                                                     "attribute vec4 NG_VertexColor;\n"
                                                     "varying vec2 fragTexCoord;\n"
                                                     "varying vec4 fragColor;\n"
                                                     "uniform mat4 NGU_MATRIX_MVP;\n"
                                                     "void main()\n"
                                                     "{\n"
                                                     "    fragTexCoord = NG_VertexTexCoord;\n"
                                                     "    fragColor = NG_VertexColor;\n"
                                                     "    gl_Position = NGU_MATRIX_MVP * vec4(NG_VertexPos, 1.0);\n"
                                                     "}\n";

        static constexpr const char *GL21_F_SHADER = "#version 120\n"
                                                     "varying vec2 fragTexCoord;\n"
                                                     "varying vec4 fragColor;\n"
                                                     "uniform sampler2D NGU_TEXTURE;\n"
                                                     "void main()\n"
                                                     "{\n"
                                                     "    vec4 texelColor = texture2D(NGU_TEXTURE, fragTexCoord);\n"
                                                     "    gl_FragColor = texelColor*fragColor;\n"
                                                     "}\n";

        static constexpr const char *GLES2_V_SHADER = "#version 100\n"
                                                      "attribute vec3 NG_VertexPos;\n"
                                                      "attribute vec2 NG_VertexTexCoord;\n"
                                                      "attribute vec4 NG_VertexColor;\n"
                                                      "varying vec2 fragTexCoord;\n"
                                                      "varying vec4 fragColor;\n"
                                                      "uniform mat4 NGU_MATRIX_MVP;\n"
                                                      "void main()\n"
                                                      "{\n"
                                                      "    fragTexCoord = NG_VertexTexCoord;\n"
                                                      "    fragColor = NG_VertexColor;\n"
                                                      "    gl_Position = NGU_MATRIX_MVP * vec4(NG_VertexPos, 1.0);\n"
                                                      "}\n";

        static constexpr const char *GLES2_F_SHADER = "#version 100\n"
                                                      "precision mediump float;\n"
                                                      "varying vec2 fragTexCoord;\n"
                                                      "varying vec4 fragColor;\n"
                                                      "uniform sampler2D NGU_TEXTURE;\n"
                                                      "void main()\n"
                                                      "{\n"
                                                      "    vec4 texelColor = texture2D(NGU_TEXTURE, fragTexCoord);\n"
                                                      "    gl_FragColor = texelColor*fragColor;\n"
                                                      "}\n";

        static constexpr const char *GL33_V_SHADER = "#version 330\n"
                                                     "in vec3 NG_VertexPos;\n"
                                                     "in vec2 NG_VertexTexCoord;\n"
                                                     "in vec4 NG_VertexColor;\n"
                                                     "out vec2 fragTexCoord;\n"
                                                     "out vec4 fragColor;\n"
                                                     "uniform mat4 NGU_MATRIX_MVP;\n"
                                                     "void main()\n"
                                                     "{\n"
                                                     "    fragTexCoord = NG_VertexTexCoord;\n"
                                                     "    fragColor = NG_VertexColor;\n"
                                                     "    gl_Position = NGU_MATRIX_MVP * vec4(NG_VertexPos, 1.0);\n"
                                                     "}\n";

        static constexpr const char *GL33_F_SHADER = "#version 330\n"
                                                     "in vec2 fragTexCoord;\n"
                                                     "in vec4 fragColor;\n"
                                                     "out vec4 finalColor;\n"
                                                     "uniform sampler2D NGU_TEXTURE;\n"
                                                     "void main()\n"
                                                     "{\n"
                                                     "    vec4 texelColor = texture2D(NGU_TEXTURE, fragTexCoord);\n"
                                                     "    finalColor = texelColor*fragColor;\n"
                                                     "}\n";
    private:
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

        /**
         * Whether or not we are building vertices.
         */
        bool m_buildingVertices = false;

        /**
         * The vertex data type used to build.
         * Will be converted if not Triangles.
         */
        PrimitiveType m_builtType;

        /**
         * The built vertex array.
         */
        Vertex m_builtVertices[MAX_TRIANGLE_VERTICES];

        /**
         * The number of vertices built.
         */
        int m_builtVertexCount = 0;

        /**
         * Add triangles to the buffer.
         */
        void _addTriangles(Vertex *vertices_, int count_, bool translate_ = false);

        /**
         * Add indexed triangles to the buffer (no matrix translation).
         */
        void _addIndexedTriangles(Vertex *vertices_, int vCount_, unsigned short *indices_, int iCount_,
                                  bool translate_ = false);

    public:
        Renderer(GraphicsDevice *graphicsDevice_);

        ~Renderer();

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
         * Begin pushing vertices.
         *
         * @param type_ The vertex type. Will be converted by the `VertexDataTool`.
         * @deprecated Use `VertexDataTool` to build and then store data instead, it is far more efficient due to far less function calls. Even use a buffer for large data for even quicker rendering!
         */
        void BeginVertices(PrimitiveType type_);

        /**
         * Finish pushing vertices and add to the render queue.
         * @deprecated Use `VertexDataTool` to build and then store data instead, it is far more efficient due to far less function calls. Even use a buffer for large data for even quicker rendering!
         */
        void EndVertices();

        /**
         * Add a vertex to the current vertex build.
         *
         * @deprecated Use `VertexDataTool` to build and then store data instead, it is far more efficient due to far less function calls. Even use a buffer for large data for even quicker rendering!
         * @param vertex_ Vertex to add.
         */
        void PushVertex(Vertex vertex_);

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
        void RenderBufferIndexed(VertexLayout *layout_, Buffer *VBO_, Buffer *IBO_, int count_, Texture2D *texture_,
                                 ShaderProgram *shader_);

        /**
         * Check if n elements of type will fit in a buffer.
         *
         * @param type_ The primitive type.
         * @param elements_ The number of elements.
         */
        bool WillFit(PrimitiveType type_, int elements_);

        /**
         * Get the graphics device attached to the renderer.
         *
         * @return The attached graphics device.
         */
        GraphicsDevice *GetGraphicsDevice();
    };
}

#endif //RENDERER_HPP
