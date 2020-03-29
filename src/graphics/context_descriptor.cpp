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
#if defined(_WIN32)
            {
                    ContextType::DirectX,
                    // Numbers below mean nothing for now
                    0, 0
            };
#else
    {
            ContextType::OpenGL,
            3,
            3
    };
#endif
#elif defined(PLATFORM_UWP)
    {
            ContextType::DirectX,
            // Numbers below mean nothing for now
            0, 0
    };
#endif

    ContextDescriptorStatus ContextDescriptor::verify() const {
        switch (Type) {
            case ContextType::OpenGL:
#if !defined(NGINE_ENABLE_OPENGL)
                return ContextDescriptorStatus::NotEnabledOrSupported;
#else
                // We now require 3.3 for Uniform Buffer Objects and Sampler Objects.
                if (MajorVersion < 3 || (MajorVersion == 3 && MinorVersion < 3)) return ContextDescriptorStatus::Outdated;
                if (MajorVersion == 3 && MinorVersion > 3) return ContextDescriptorStatus::InvalidVersion;
                if (MajorVersion == 4 && MinorVersion > 6) return ContextDescriptorStatus::InvalidVersion;
#endif
                break;
            case ContextType::OpenGLES:
#if !defined(NGINE_ENABLE_OPENGLES)
                return ContextDescriptorStatus::NotEnabledOrSupported;
#else
                // We now require GLES 3.0 or greater. This is to remove the masses of legacy code to support it.
                if (MajorVersion < 3) return ContextDescriptorStatus::Outdated;
                if (MajorVersion == 3 && MinorVersion > 1) return ContextDescriptorStatus::InvalidVersion; // 3.2 isn't available through ANGLE yet.
#endif
                break;
            case ContextType::DirectX:
#if !defined(NGINE_ENABLE_DIRECTX)
                return ContextDescriptorStatus::NotEnabledOrSupported;
#endif
                break;
            case ContextType::Vulkan:
                return ContextDescriptorStatus::NotImplemented;
                break;
            case ContextType::Metal:
                return ContextDescriptorStatus::NotImplemented;
                break;
        }
        return ContextDescriptorStatus::OK;
    }
}