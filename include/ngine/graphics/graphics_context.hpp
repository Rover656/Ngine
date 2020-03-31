/**********************************************************************************************
 *
 *   Ngine - A game framework.
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

#ifndef NGINE_GRAPHICS_CONTEXT_HPP
#define NGINE_GRAPHICS_CONTEXT_HPP

#include "ngine/config.hpp"

#include "types.hpp"

namespace ngine::graphics {
    /**
     * Context type.
     */
    enum class ContextType {
        /**
         * OpenGL context.
         */
        OpenGL,

        /**
         * OpenGL ES context.
         */
        OpenGLES,

        /**
         * DirectX context.
         */
        DirectX,

        /**
         * Vulkan context.
         * @warning Not implemented, this is reserved for future support.
         */
        Vulkan,

        /**
         * Metal context.
         * @warning Not implemented, this is reserved for future support.
         */
        Metal
    };

    /**
     * Context descriptor validation status.
     * Allows easier debugging
     */
    enum class ContextDescriptorStatus {
        /**
         * The Context Descriptor is valid.
         */
        OK = 0,

        /**
         * The version described is too old to run in Ngine.
         */
        Outdated = 1,

        /**
         * The target version does not exist.
         */
        InvalidVersion = 2,

        /**
         * The target context type is not enabled or is not supported on the current platform.
         */
        NotEnabledOrSupported = 3,

        /**
         * The target context type has not been implemented.
         */
        NotImplemented = 4
    };

    /**
     * Context version information.
     */
    struct NEAPI ContextDescriptor {
        /**
         * Default context version for the current platform.
         */
        static ContextDescriptor Default;

        /**
         * Context type.
         */
        ContextType Type;

        /**
         * Major version (only for OGL at the minute).
         */
        unsigned int MajorVersion;

        /**
         * Minor version (only for OGL at the minute).
         */
        unsigned int MinorVersion;

        /**
         * Verify context descriptor options.
         */
        ContextDescriptorStatus verify() const;
    };
    
    class NEAPI IGraphicsContext {
    public:
        virtual void Clear(Color color) = 0;
    };
}

#endif //NGINE_GRAPHICS_CONTEXT_HPP
