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

#include "gl_uniform_data_manager.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

namespace ngine::platform::graphics {
    GLUniformDataManager::GLUniformDataManager(IGraphicsDevice *graphicsDevice, std::vector<Uniform> layout)
            : IUniformDataManager(graphicsDevice, layout) {
        // Here, we configure the offsets to adhere to std140 layout and allocate enough memory for this.
        for (auto u : m_uniforms) {
            m_offsets.push_back(m_internalDataSize);

            // Ensure the size is a multiple of 16
            m_internalDataSize += (u.getSize()-1|15)+1;
        }

        // Allocate memory
        m_data = calloc(1, m_internalDataSize);
    }
}

#endif // defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)
