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
#include "ShaderProgramState.hpp"
#include "Texture2D.hpp"
#include "VertexLayout.hpp"
#include "Viewport.hpp"

namespace ngine::graphics {
    /**
     * Vertex data.
     * `Vector3` for Position, `Vector2` for tex coords and `Color` for color.
     */
    struct Vertex {
        Vector3 Position;
        Vector2 TexCoords;
        graphics::Color Color;
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

        // TEMP: GLSL shaders for each version. I want to have a single shader language for this somepoint (either HLSL converter or something else)
        static constexpr const char *GLES2_V_SHADER = "#version 100\n"
                                                      "attribute vec3 NG_VertexPos;\n"
                                                      "attribute vec2 NG_VertexTexCoord;\n"
                                                      "attribute vec4 NG_VertexColor;\n"
                                                      "varying vec2 fragTexCoord;\n"
                                                      "varying vec4 fragColor;\n"
                                                      "uniform mat4 NGINE_MATRIX_MVP;\n"
                                                      "void main()\n"
                                                      "{\n"
                                                      "    fragTexCoord = NG_VertexTexCoord;\n"
                                                      "    fragColor = NG_VertexColor;\n"
                                                      "    gl_Position = NGINE_MATRIX_MVP * vec4(NG_VertexPos, 1.0);\n"
                                                      "}\n";

        static constexpr const char *GLES2_F_SHADER = "#version 100\n"
                                                      "precision mediump float;\n"
                                                      "varying vec2 fragTexCoord;\n"
                                                      "varying vec4 fragColor;\n"
                                                      "uniform sampler2D NGINE_TEXTURE;\n"
                                                      "void main()\n"
                                                      "{\n"
                                                      "    vec4 texelColor = texture2D(NGINE_TEXTURE, fragTexCoord);\n"
                                                      "    gl_FragColor = texelColor*fragColor;\n"
                                                      "}\n";

        static constexpr const char *GLES3_V_SHADER = "#version 300 es\n"
                                                      "in vec3 NG_VertexPos;\n"
                                                      "in vec2 NG_VertexTexCoord;\n"
                                                      "in vec4 NG_VertexColor;\n"
                                                      "out vec2 fragTexCoord;\n"
                                                      "out vec4 fragColor;\n"
                                                      "uniform mat4 NGINE_MATRIX_MVP;\n"
                                                      "void main()\n"
                                                      "{\n"
                                                      "    fragTexCoord = NG_VertexTexCoord;\n"
                                                      "    fragColor = NG_VertexColor;\n"
                                                      "    gl_Position = NGINE_MATRIX_MVP * vec4(NG_VertexPos, 1.0);\n"
                                                      "}\n";

        static constexpr const char *GLES3_F_SHADER = "#version 300 es\n"
                                                      "precision mediump float;"
                                                      "in vec2 fragTexCoord;\n"
                                                      "in vec4 fragColor;\n"
                                                      "out vec4 finalColor;\n"
                                                      "uniform sampler2D NGINE_TEXTURE;\n"
                                                      "void main()\n"
                                                      "{\n"
                                                      "    vec4 texelColor = texture(NGINE_TEXTURE, fragTexCoord);\n"
                                                      "    finalColor = texelColor*fragColor;\n"
                                                      "}\n";

        static constexpr const char *GL3X_V_SHADER = "#version 130\n"
                                                     "in vec3 NG_VertexPos;\n"
                                                     "in vec2 NG_VertexTexCoord;\n"
                                                     "in vec4 NG_VertexColor;\n"
                                                     "out vec2 fragTexCoord;\n"
                                                     "out vec4 fragColor;\n"
                                                     "uniform mat4 NGINE_MATRIX_MVP;\n"
                                                     "void main()\n"
                                                     "{\n"
                                                     "    fragTexCoord = NG_VertexTexCoord;\n"
                                                     "    fragColor = NG_VertexColor;\n"
                                                     "    gl_Position = NGINE_MATRIX_MVP * vec4(NG_VertexPos, 1.0);\n"
                                                     "}\n";

        static constexpr const char *GL3X_F_SHADER = "#version 130\n"
                                                     "in vec2 fragTexCoord;\n"
                                                     "in vec4 fragColor;\n"
                                                     "out vec4 finalColor;\n"
                                                     "uniform sampler2D NGINE_TEXTURE;\n"
                                                     "void main()\n"
                                                     "{\n"
                                                     "    vec4 texelColor = texture(NGINE_TEXTURE, fragTexCoord);\n"
                                                     "    finalColor = texelColor*fragColor;\n"
                                                     "}\n";

        static constexpr const char *GL33_V_SHADER = "#version 330\n"
                                                     "in vec3 NG_VertexPos;\n"
                                                     "in vec2 NG_VertexTexCoord;\n"
                                                     "in vec4 NG_VertexColor;\n"
                                                     "out vec2 fragTexCoord;\n"
                                                     "out vec4 fragColor;\n"
                                                     "uniform mat4 NGINE_MATRIX_MVP;\n"
                                                     "void main()\n"
                                                     "{\n"
                                                     "    fragTexCoord = NG_VertexTexCoord;\n"
                                                     "    fragColor = NG_VertexColor;\n"
                                                     "    gl_Position = NGINE_MATRIX_MVP * vec4(NG_VertexPos, 1.0);\n"
                                                     "}\n";

        static constexpr const char *GL33_F_SHADER = "#version 330\n"
                                                     "in vec2 fragTexCoord;\n"
                                                     "in vec4 fragColor;\n"
                                                     "out vec4 finalColor;\n"
                                                     "uniform sampler2D NGINE_TEXTURE;\n"
                                                     "void main()\n"
                                                     "{\n"
                                                     "    vec4 texelColor = texture(NGINE_TEXTURE, fragTexCoord);\n"
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
         * The shader state being used for the current batch.
         */
        ShaderProgramState *m_currentShaderState = nullptr;

        /**
         * The current model view matrix.
         */
        Matrix m_currentModelView = Matrix::Identity;

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
         * The default shaders state.
         */
        ShaderProgramState *m_defaultShaderState = nullptr;

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
         * The current coordinate system we are using.
         */
        CoordinateSystem m_currentCoordSystem = CoordinateSystem::Screen;

        /**
         * Add triangles to the buffer.
         */
        void _addTriangles(Vertex *vertices, int count, bool translate = false);

        /**
         * Add indexed triangles to the buffer (no matrix translation).
         */
        void
        _addIndexedTriangles(Vertex *vertices, int vCount, unsigned short *indices, int iCount, bool translate = false);

    public:
        /**
         * Create a new renderer.
         *
         * @param graphicsDevice The graphics device to use.
         */
        Renderer(GraphicsDevice *graphicsDevice);

        ~Renderer();

        /**
         * Add triangle vertices to the current batch.
         *
         * @param vertices Triangle vertices to add.
         * @param count The number of vertices in the array.
         */
        void addTriangles(Vertex *vertices, int count);

        /**
         * Add indexed triangle vertices and indices.
         *
         * @param vertices Triangle vertices that are indexed by `indices_`
         * @param vCount The number of vertices in the array.
         * @param indices The indices for the vertex array.
         * @param iCount The number of indices in the array.
         */
        void addIndexedTriangles(Vertex *vertices, int vCount, unsigned short *indices, int iCount);

        /**
         * Begin pushing vertices.
         *
         * @param type The vertex type. Will be converted by the `VertexDataTool`.
         * @note For large numbers of vertices, push use `addTriangles`, `addIndexedTriangles` or even use a custom vertex buffer. These methods are more efficient.
         */
        void beginVertices(PrimitiveType type);

        /**
         * Finish pushing vertices and add to the render queue.
         */
        void endVertices();

        /**
         * Add a vertex to the current vertex build.
         *
         * @param vertex Vertex to add.
         */
        void pushVertex(Vertex vertex);

        /**
         * Set the coordinate system.
         *
         * @warning Don't touch this unless you are going to reset the value and not call anything that depends on a different system. It can seriously break stuff.
         * @param system The new system to use.
         */
        void setCoordinateSystem(CoordinateSystem system);

        /**
         * Get the current coordinate system.
         *
         * @return The current coordinate system.
         */
        CoordinateSystem getCoordinateSystem();

        /**
         * Set the current texture for rendering.
         *
         * @note Will force a draw of anything before this call if the texture is different.
         * @param texture The texture to render with.
         */
        void setTexture(Texture2D *texture);

        /**
         * Set the current shader for rendering.
         *
         * @note Will force a draw of anything before this call if the texture is different.
         * @param state The shader program state to use.
         */
        void setShader(ShaderProgramState *state);

        /**
         * Set the current mode view matrix.
         *
         * @note Will force a draw, use this sparingly.
         * @param modelView The new modelview matrix.
         */
        void setModelViewMatrix(Matrix modelView);

        /**
         * Push a matrix onto the stack (with the current matrix's value).
         *
         * @note This is extremely handy for moving vertices around. This **only** supports the immediate mode (Begin, Vertex, End) workflow at the moment.
         */
        void pushMatrix();

        /**
         * Pop a matrix from the stack.
         *
         * @warning Will throw if called when there's nothing to pop.
         */
        void popMatrix();

        /**
         * Set the value of the current Matrix.
         *
         * @warning Remember to push first, this could destroy critical existing data. General rule of thumb: Push if you modify something.
         * @param mat The new value for the matrix.
         */
        void setMatrix(const Matrix &mat);

        /**
         * Load the identity matrix to the current matrix.
         *
         * @see `SetMatrix`
         */
        void loadIdentity();

        /**
         * Multiply the given matrix by the current one.
         *
         * @note Formula is as follows: Current Matrix = Matrix Provided * Current Matrix
         */
        void multiplyMatrix(const Matrix &mat);

        /**
         * Multiply the current matrix with a translation matrix.
         *
         * @param translation The translation to apply.
         */
        void translate(const Vector3 &translation);

        /**
         * Multiply the current matrix with a rotation matrix.
         *
         * @param rotation Rotation to apply
         * @param axis Axis in which to apply rotation.
         */
        void rotate(const Angle &rotation, const Vector3 &axis);

        /**
         * Multiply the current matrix with a scale matrix.
         *
         * @param scale The scale to apply.
         */
        void scale(const Vector3 &scale);

        /**
         * Trigger a render of the internal buffers with the graphics device's coordinate system.
         */
        void renderBatch();

        /**
         * Render a vertex buffer.
         *
         * @param layout The vertex layout.
         * @param VBO_ The vertex buffer.
         * @param count Number of vertices to render.
         * @param texture The texture to render with.
         * @param shader_ The shader program state to render with.
         */
        void
        renderBuffer(const Viewport *viewport, CoordinateSystem coordSys, VertexLayout *layout, Buffer *VBO, int count,
                     Texture2D *texture, ShaderProgramState *state);

        /**
         * Render an indexed vertex buffer.
         *
         * @param layout The vertex layout.
         * @param VBO The vertex buffer.
         * @param IBO The index buffer.
         * @param count The number of elements. I.e. triangle count * 3
         * @param texture The texture to render with.
         * @param state_ The shader program state to render with.
         */
        void renderBufferIndexed(const Viewport *viewport, CoordinateSystem coordSys, VertexLayout *layout, Buffer *VBO,
                                 Buffer *IBO, int count, Texture2D *texture, ShaderProgramState *state);

        /**
         * Check if n elements of type will fit in a buffer.
         *
         * @param type The primitive type.
         * @param elements The number of elements.
         */
        bool willFit(PrimitiveType type, int elements);

        /**
         * Get the graphics device attached to the renderer.
         *
         * @return The attached graphics device.
         */
        GraphicsDevice *getGraphicsDevice();
    };
}

#endif //RENDERER_HPP
