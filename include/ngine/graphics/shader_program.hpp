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

#include "graphics_resource.hpp"
#include "shader.hpp"

#include <vector>

namespace ngine::graphics {
    class GraphicsDevice;

    // TODO: More permanent location
    enum class ElementType {
        //Byte,
        //UnsignedByte,
        //Short,
        //UnsignedShort,
        Int,
        UnsignedInt,
        Float
    };

    /**
     * A vertex buffer element is an entry in a vertex layout.
     * It details an element in the vertex data.
     */
    struct VertexBufferElement {
        /**
         * Vertex element name.
         */
        const char *Name;

        /**
         * Data type.
         */
        ElementType Type;

        /**
         * Data count.
         */
        int Count;

        /**
         * Is the data normalized?
         */
        bool Normalized;

        int getSize() const;

        bool operator==(const VertexBufferElement &b) const {
            return Name == b.Name && Type == b.Type && Count == b.Count && Normalized == b.Normalized;
        }

        bool operator!=(const VertexBufferElement &b) const {
            return Name != b.Name || Type != b.Type || Count != b.Count || Normalized != b.Normalized;
        }
    };

    /**
     * Vertex buffer layout.
     * @note Should match layout in the shader.
     */
    struct VertexBufferLayout {
        /**
         * The vertex layouts elements.
         */
        std::vector<VertexBufferElement> Elements;

        /**
         * Get the layout size.
         */
        int getSize() const;

        bool operator==(const VertexBufferLayout &b) const {
            return Elements == b.Elements;
        }

        bool operator!=(const VertexBufferLayout &b) const {
            return Elements != b.Elements;
        }
    };

    class NEAPI ShaderProgram : public GraphicsResource {
    public:
        /**
         * Create a shader program.
         */
        ShaderProgram(GraphicsDevice *graphicsDevice, VertexBufferLayout layout);

        /**
         * Destroy a shader program.
         */
        ~ShaderProgram();

        /**
         * Get this shaders vertex buffer layout.
         */
        const VertexBufferLayout getLayout() const;

        /**
         * Attach a shader to the program.
         */
        void attachShader(Shader *shader);

        /**
         * Get a shader by its type.
         * @return The shader or null.
         */
        Shader *getShaderByType(ShaderType type);

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
        VertexBufferLayout m_layout;
    };
}

#endif //NGINE_SHADER_PROGRAM_HPP
