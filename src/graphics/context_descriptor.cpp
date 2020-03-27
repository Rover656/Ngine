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

#include "ngine/graphics/context_descriptor.hpp"

namespace ngine::graphics {
    ContextDescriptor ContextDescriptor::Default =
#if defined(PLATFORM_DESKTOP)
            {
                    ContextType::OpenGL,
                    3,
                    3
            };
#elif defined(PLATFORM_UWP)
            {
                    CContextType::OpenGLES,
                    2,
                    1
            };
#endif

    bool ContextDescriptor::verify() const {
        switch (Type) {
            case ContextType::OpenGL:
#if !defined(NGINE_ENABLE_OPENGL)
                return false;
#else
                if (MajorVersion == 3 && MinorVersion != 3) return false;
                if (MajorVersion == 4 && MinorVersion > 6) return false;
#endif
                break;
            case ContextType::OpenGLES:
                // TODO: Finish this
                break;
            case ContextType::DirectX: break; // Doesnt use this parameter.
            case ContextType::Vulkan: break; // Will add once we support it.
        }
        return true;
    }
}