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

#include "Graphics/ShaderProgramState.hpp"

#include "Console.hpp"

#include <cstring>

namespace Ngine::Graphics {
    ShaderProgramState::ShaderProgramState(ShaderProgram *program_)
            : AttachedProgram(program_) {
        // Check program
        if (!AttachedProgram->IsFinal())
            Console::Fail("ShaderProgramState", "Shader program must be marked as final before use.");

        // Allocate
        // Get data size.
        int size = AttachedProgram->GetUniformDataSize();

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

    const void *ShaderProgramState::GetUniform(const std::string &name_) {
        // Find the uniform
        ShaderDataStructure uniformDesc;
        for (const auto& uniform : AttachedProgram->GetUniforms()) {
            if (uniform.Name == name_) {
                uniformDesc = uniform;
                break;
            }
        }

        // Skip if there is no data
        if (uniformDesc.GetSize() == 0 || uniformDesc.Count == 0)
            Console::Fail("ShaderUniformData", "Uniform could not be found or uniform has invalid parameters.");

        char *dat = (char *) m_data;
        return &dat[uniformDesc.Offset];
    }

    void ShaderProgramState::SetUniform(const std::string &name_, void *data_) {
        if (m_data == nullptr)
            Console::Fail("ShaderUniformData", "Memory not allocated. Call Allocate() first.");

        // Find the uniform
        ShaderDataStructure uniformDesc;
        for (const auto& uniform : AttachedProgram->GetUniforms()) {
            if (uniform.Name == name_) {
                uniformDesc = uniform;
                break;
            }
        }

        // Skip if there is no data
        if (uniformDesc.GetSize() == 0 || uniformDesc.Count == 0) return;

        // Copy data in
        char *dat = (char *) m_data;
        memcpy(&dat[uniformDesc.Offset], data_, uniformDesc.GetSize() * uniformDesc.Count);
    }

    void ShaderProgramState::SetUniformEx(std::vector<std::string> nameTree_, void *data_) {
        ShaderDataStructure target;

        // Find initial structure
        for (const auto& uniform : AttachedProgram->GetUniforms()) {
            if (uniform.Name == nameTree_[0]) {
                target = uniform;
                break;
            }
        }

        // Skip if there is no target
        if (target.GetSize() == 0 || target.Count == 0) return;

        // Navigate name tree
        int dataOffset = target.Offset;
        for (auto i = 1; i < nameTree_.size(); i++) {
            switch (target.Type) {
                case ShaderDataType::Int:
                case ShaderDataType::UnsignedInt:
                case ShaderDataType::Float:
                case ShaderDataType::Matrix:
                    Console::Fail("ShaderProgramState", "Cannot navigate *into* a simple data format.");
                    break;
                case ShaderDataType::Struct:
                    // Find target within this struct
                    for (const auto& member : target.Members) {
                        if (member.Name == nameTree_[i]) {
                            target = member;
                            dataOffset += target.Offset;
                            goto found;
                        }
                    }
                    break;
                case ShaderDataType::Array:
                    auto mem = target.Members[0];
                    auto index = std::stoi(nameTree_[i]);
                    if (index < target.Count) {
                        target = mem;
                        dataOffset += target.GetSize() * index;
                        goto found;
                    }
                    break;
            }
            Console::Fail("ShaderProgramState", "Could not find member.");
            found: continue;
        }

        // Copy data in
        char *dat = (char *) m_data;
        memcpy(&dat[dataOffset], data_, target.GetSize() * target.Count);
    }

    const void *ShaderProgramState::GetData() const {
        return m_data;
    }
}