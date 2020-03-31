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

#ifndef NGINE_DIRECTX_UNIFORM_DATA_MANAGER_HPP
#define NGINE_DIRECTX_UNIFORM_DATA_MANAGER_HPP

#include "ngine/config.hpp"

#if defined(NGINE_ENABLE_DIRECTX)

#include "ngine/graphics/uniform_data_manager.hpp"

namespace ngine::platform::graphics {
    using namespace ngine;
    using namespace ngine::graphics;

    class DirectXUniformDataManager : public IUniformDataManager {
    public:
        /**
         * Create a uniform data manager.
         */
        DirectXUniformDataManager(IGraphicsDevice *graphicsDevice, std::vector<Uniform> layout);
    };
}

#endif // defined(NGINE_ENABLE_DIRECTX)

#endif //NGINE_DIRECTX_UNIFORM_DATA_MANAGER_HPP
