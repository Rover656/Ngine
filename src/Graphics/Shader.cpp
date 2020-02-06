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

#include "Graphics/Shader.hpp"

#include "Graphics/API/PlatformGraphicsAPI.hpp"

namespace ngine::graphics {
    Shader::Shader(GraphicsDevice *graphicsDevice_, ShaderType type_, const char *src_)
            : Type(type_) {
        // Get API
        m_API = graphicsDevice_->getAPI();

        // Create
        m_API->createShader(this, (void *) src_);
    }

    Shader::~Shader() {
        free();
    }

    bool Shader::isValid() const {
        return m_API->isShaderValid(this);
    }

    void Shader::free() {
        m_API->deleteShader(this);
    }
}
