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

#include "graphics/ShaderProgramState.hpp"

#include "Console.hpp"

#include <cstring>

namespace ngine::graphics {
    ShaderProgramState::ShaderProgramState(ShaderProgram *program)
            : AttachedProgram(program) {
        // Check program
        if (!AttachedProgram->isFinal())
            Console::fail("ShaderProgramState", "Shader program must be marked as final before use.");

        // Allocate
        // Get data size.
        int size = AttachedProgram->getUniformDataSize();

        // Allocate
        m_data = malloc(size);

        // Zero memory
        memset(m_data, 0, size);
    }

    ShaderProgramState::~ShaderProgramState() {
        // Free memory
        free(m_data);
        m_data = nullptr;
    }

    const void *ShaderProgramState::getUniform(const std::string &name) {
        // Find the uniform
        ShaderDataStructure uniformDesc;
        for (const auto& uniform : AttachedProgram->getUniforms()) {
            if (uniform.Name == name) {
                uniformDesc = uniform;
                break;
            }
        }

        // Skip if there is no data
        if (uniformDesc.getSize() == 0 || uniformDesc.Count == 0)
            Console::fail("ShaderUniformData", "Uniform could not be found or uniform has invalid parameters.");

        char *dat = (char *) m_data;
        return &dat[uniformDesc.Offset];
    }

    void ShaderProgramState::setUniform(const std::string &name, void *data) {
        if (m_data == nullptr)
            Console::fail("ShaderUniformData", "Memory not allocated. Call Allocate() first.");

        // Find the uniform
        ShaderDataStructure uniformDesc;
        for (const auto& uniform : AttachedProgram->getUniforms()) {
            if (uniform.Name == name) {
                uniformDesc = uniform;
                break;
            }
        }

        // Skip if there is no data
        if (uniformDesc.getSize() == 0 || uniformDesc.Count == 0) return;

        // Copy data in
        char *dat = (char *) m_data;
        memcpy(&dat[uniformDesc.Offset], data, uniformDesc.getSize() * uniformDesc.Count);
    }

    void ShaderProgramState::setUniformEx(std::vector<std::string> nameTree, void *data) {
        ShaderDataStructure target;

        // Find initial structure
        for (const auto& uniform : AttachedProgram->getUniforms()) {
            if (uniform.Name == nameTree[0]) {
                target = uniform;
                break;
            }
        }

        // Skip if there is no target
        if (target.getSize() == 0 || target.Count == 0) return;

        // Navigate name tree
        int dataOffset = target.Offset;
        for (auto i = 1; i < nameTree.size(); i++) {
            switch (target.Type) {
                case ShaderDataType::Int:
                case ShaderDataType::UnsignedInt:
                case ShaderDataType::Float:
                case ShaderDataType::Matrix:
                    Console::fail("ShaderProgramState", "Cannot navigate *into* a simple data format.");
                    break;
                case ShaderDataType::Struct:
                    // Find target within this struct
                    for (const auto& member : target.Members) {
                        if (member.Name == nameTree[i]) {
                            target = member;
                            dataOffset += target.Offset;
                            goto found;
                        }
                    }
                    break;
                case ShaderDataType::Array:
                    auto mem = target.Members[0];
                    auto index = std::stoi(nameTree[i]);
                    if (index < target.Count) {
                        target = mem;
                        dataOffset += target.getSize() * index;
                        goto found;
                    }
                    break;
            }
            Console::fail("ShaderProgramState", "Could not find member.");
            found: continue;
        }

        // Copy data in
        char *dat = (char *) m_data;
        memcpy(&dat[dataOffset], data, target.getSize() * target.Count);
    }

    const void *ShaderProgramState::getData() const {
        return m_data;
    }
}