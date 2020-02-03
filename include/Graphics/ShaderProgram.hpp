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

#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "../Config.hpp"

#include "GraphicsDevice.hpp"

namespace Ngine::Graphics {
    class Shader;

    namespace API {
        class PlatformGraphicsAPI;
    }

    enum class ShaderDataType {
        /**
         * A signed integer.
         */
        Int = 0,

        /**
         * An unsigned integer.
         */
        UnsignedInt,

        /**
         * A float.
         */
        Float,

        /**
         * A matrix is a grid of 16 floats.
         */
        Matrix,

        // Next two are wip.
        /**
         * A struct contains a number of different types.
         */
        Struct,

        /**
         * An array contains a list of a single type.
         */
        Array
    };

    class T {
    public:
        std::string Name;
        ShaderDataType Type;
        int Count;
        int Offset;
    };

    // WIP
    struct ShaderDataStructure {
        /**
         * The number of elements.
         *
         * @note For standard scalars, this determines float, vec2, vec3 etc.
         */
        int Count = 1;

        /**
         * The element type. This is used to calculate the memory size.
         */
        ShaderDataType Type;

        /**
         * The element name.
         */
        std::string Name;

        /**
         * The element members.
         *
         * @note For arrays this should contain a singular member to determine what type the array is holding.
         */
        std::vector<ShaderDataStructure> Members;

        /**
         * Offset. Set internally.
         */
        int Offset = 0;

        /**
         * Add a member.
         */
        void AddMember(ShaderDataStructure member_) {
            member_.Offset = GetSize();
            Members.push_back(member_);
        }

        /**
         * Get the size of the structure.
         */
        int GetSize() const {
            int s = 0;
            switch(Type) {
                case ShaderDataType::Int:
                    s = sizeof(int) * Count;
                    break;
                case ShaderDataType::UnsignedInt:
                    s = sizeof(unsigned int) * Count;
                    break;
                case ShaderDataType::Float:
                    s = sizeof(float) * Count;
                    break;
                case ShaderDataType::Matrix:
                    s = sizeof(float) * 16;
                    break;
                case ShaderDataType::Struct:
                    for (auto member : Members)
                        s += member.GetSize();
                    break;
                case ShaderDataType::Array:
                    s = Members[0].GetSize() * Count;
                    break;
            }

            return s;
        }
    };

    /**
     * Description of a shader uniform.
     */
    struct NEAPI ShaderUniformDesc {
        /**
         * Size of a single element in memory.
         */
        int Size = 0;

        /**
         * The number of elements to be submitted,
         */
        int Count = 0;

        /**
         * Unique identifier for the uniform.
         */
        std::string Name = "";

        /**
         * The memory offset within the uniform data.
         * @note This is normally determined by `ShaderUniformData` upon format construction. Only set this if using your own data structure.
         */
        int Offset = -1;

        /**
         * The target data type.
         */
        ShaderDataType Type;

        /**
         * The size of the array (1 = no array).
         */
        int ArraySize = 1;

        /**
         * Blank shader uniform description.
         */
        ShaderUniformDesc() {}

        /**
         * Define a new shader uniform.
         */
        ShaderUniformDesc(std::string name_, ShaderDataType type_, int count_, int arraySize_ = 1) : Name(std::move(name_)), Count(count_), Type(type_), ArraySize(arraySize_) {
            switch (Type) {
                case ShaderDataType::Int:
                    Size = sizeof(int);
                    break;
                case ShaderDataType::UnsignedInt:
                    Size = sizeof(unsigned int);
                    break;
                case ShaderDataType::Float:
                    Size = sizeof(float);
                    break;
                case ShaderDataType::Matrix:
                    Size = sizeof(float) * 16;
                    break;
            }
        }
    };

    /**
     * Shader program which contains a shader and manages it.
     *
     * Predefined attributes:
     *  * vec3 - NG_VertexPos
     *  * vec2 - NG_VertexTexCoord
     *  * vec4 - NG_VertexColor
     *
     * Predefined uniforms:
     *  * mat4 - NGU_MATRIX_MVP
     *  * sampler2D - NGU_TEXTURE
     */
    class NEAPI ShaderProgram final : public IResource {
    private:
        /**
         * The graphics API.
         */
        API::PlatformGraphicsAPI *m_API;

        /**
         * Attribute location cache.
         */
        std::unordered_map<std::string, unsigned int> _AttribLocationCache;

        /**
         * Uniform location cache.
         */
        std::unordered_map<std::string, unsigned int> _UniformLocationCache;

        /**
         * All of the uniforms that are part of the shader.
         */
        std::vector<ShaderDataStructure> m_uniforms;

        /**
         * Whether or not the shader is final.
         */
        bool m_final = false;
    public:
        union {
            /**
             * Shader program ID.
             */
            unsigned int ID = 0;
        };

        /**
         * The attached vertex shader.
         */
        const Shader *VertexShader;

        /**
         * The attached fragment/pixel shader.
         */
        const Shader *FragmentShader;

        /**
         * Create a shader program.
         *
         * @todo Rework this constructor if we want to support more kinds of shader.
         * @warning Uniforms must be passed in the correct order as they are defined, this is soley for Direct X.
         * @param graphicsDevice_ The graphics device.
         * @param vertexShader_ The vertex shader.
         * @param fragmentShader_ The fragment shader.
         * @param uniforms_ The uniform definitions for the program.
         */
        ShaderProgram(GraphicsDevice *graphicsDevice_, Shader *vertexShader_, Shader *fragmentShader_, std::vector<ShaderDataStructure> uniforms_ = {});
        ~ShaderProgram();

        /**
         * Add a uniform to the shader program so that it can be given a value.
         *
         * @param uniform_ The uniform to add.
         */
        void AddUniform(ShaderDataStructure uniform_);

        /**
         * Get the list of uniforms.
         */
        std::vector<ShaderDataStructure> GetUniforms() const;

        /**
         * Get the size of the uniform data.
         *
         * @return The size (in bytes) the uniform data occupies.
         */
        int GetUniformDataSize() const;

        /**
         * Mark shader program as final. This makes it ready for use.
         */
        void Finalize();

        /**
         * Whether or not the shader program is final.
         *
         * @return Whether or not the shader program is final/ready for use.
         */
        bool IsFinal() const;

        // These need a universal system.
        unsigned int GetAttribLocation(const std::string &name_);
        unsigned int GetUniformLocation(const std::string &name_);

        /**
         * Determine if the shader program is valid.
         */
        bool IsValid() const override;

        /**
         * Free the shader program.
         */
        void Free() override;
    };
}

#endif //SHADERPROGRAM_HPP
