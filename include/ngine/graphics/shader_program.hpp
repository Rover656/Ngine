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

#ifndef NGINE_SHADER_PROGRAM_HPP
#define NGINE_SHADER_PROGRAM_HPP

#include "ngine/config.hpp"

#include "buffer.hpp"
#include "graphics_resource.hpp"
#include "shader.hpp"

#include <vector>

namespace ngine::graphics {
    /**
     * List of data types that can be passed to shaders.
     */
    enum class ElementType {
        // TODO: Implement these later, I'm lazy
        //Byte,
        //UnsignedByte,
        //Short,
        //UnsignedShort,
        Int,
        UnsignedInt,
        Float,
        Matrix
    };

    /**
     * Element use.
     * Helps systems to build vertex data for many layouts.
     */
    enum class ElementUse {
        /**
         * Vertex element represents the position.
         */
        Position,

        /**
         * Vertex element represents the texture coordinates.
         */
        Texcoords,

        /**
         * Vertex element represents the color.
         */
        Color,

        /**
         * Uniform buffer element represents the model matrix.
         */
        ModelMatrix,

        /**
         * Uniform buffer element represents the view matrix.
         */
        ViewMatrix,

        /**
         * Uniform buffer element represents the projection matrix.
         */
        ProjectionMatrix,

        /**
         * Vertex element represents a custom field and will be set to 0 on automated uses.
         */
        Custom
    };

    /**
     * A buffer element is an entry in a buffer layout.
     * It details an element in the buffer.
     */
    struct BufferElement {
        /**
         * Vertex element name.
         */
        const char *Name;

        /**
         * Data type.
         */
        ElementType Type;

        /**
         * Element use.
         */
        ElementUse Use;

        /**
         * Data count.
         */
        int Count;

        /**
         * Is the data normalized?
         */
        bool Normalized;

        /**
         * The size of the array.
         * @warning Cannot be used for vertex buffers.
         */
        int ArraySize = 1;

        int getSize() const;

        bool operator==(const BufferElement &b) const {
            return Name == b.Name && Type == b.Type && Count == b.Count && Normalized == b.Normalized;
        }

        bool operator!=(const BufferElement &b) const {
            return Name != b.Name || Type != b.Type || Count != b.Count || Normalized != b.Normalized;
        }
    };

    /**
     * Buffer layout.
     * Used to define both layouts for vertices and uniform buffers
     * @note Should match layout in the shader.
     */
    struct BufferLayout {
        /**
         * The vertex layouts elements.
         */
        std::vector<BufferElement> Elements;

        /**
         * Get the layout size.
         */
        int getSize() const;

        bool operator==(const BufferLayout &b) const {
            return Elements == b.Elements;
        }

        bool operator!=(const BufferLayout &b) const {
            return Elements != b.Elements;
        }
    };

    // TODO: Accept GLSL names for uniform block buffers (for binding because GLES v2 doesn't allow binding in a shader)
    class NEAPI ShaderProgram : public GraphicsResource {
    public:
        /**
         * The name of the uniform which holds an array of samplers.
         */
        std::string GLSLSamplerUniform = "Textures";

        /**
         * The number of texture samplers expected.
         * @warning This value is critical on OpenGL as it tells the engine how big the uniform array is.
         */
        unsigned int ExpectedSamplerCount = 1;

        /**
         * Create a shader program.
         */
        ShaderProgram(GraphicsDevice *graphicsDevice, BufferLayout vertexBufferLayout);

        /**
         * Get this shaders vertex buffer layout.
         */
        BufferLayout getVertexBufferLayout() const;

        /**
         * Attach a shader to the program.
         */
        void attachShader(Shader *shader);

        /**
         * Get a shader by its type.
         * @return The shader or null.
         */
        Shader *getShaderByType(ShaderStage type);

        /**
         * Link the shader for use.
         */
        void link();

        /**
         * Use the shader in rendering.
         */
        void use();

    private:
        /**
         * The shaders attached.
         */
        std::vector<Shader *> m_shaders;

        /**
         * The vertex buffer layout for this shader.
         */
        BufferLayout m_vertexBufferLayout;
    };
}

#endif //NGINE_SHADER_PROGRAM_HPP
