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

#include "graphics/API/PlatformGraphicsAPI.hpp"

#include "Console.hpp"

namespace ngine::graphics::API {
    void PlatformGraphicsAPI::useVertexLayout(VertexLayout *layout_) {
        if (m_currentLayout != nullptr)
            Console::Fail("PlatformGraphicsAPI", "Cannot start another vertex layout without stopping the last one!");
        _useVertexLayout(layout_);
        m_currentLayout = layout_;
    }

    void PlatformGraphicsAPI::stopVertexLayout(VertexLayout *layout_) {
        if (m_currentLayout == nullptr)
            Console::Fail("PlatformGraphicsAPI", "There is no vertex layout to stop.");
        _stopVertexLayout(layout_);
        m_currentLayout = nullptr;
    }
}