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

#ifndef NGINE_SAMPLERSTATE_HPP
#define NGINE_SAMPLERSTATE_HPP

#include "ngine/config.hpp"

#include "graphics_resource.hpp"

namespace ngine::graphics {
    enum class TextureFilter {
        Anisotropic,
        Linear,
        LinearMipPoint,
        MinLinearMagPointMipLinear,
        MinLinearMagPointMipPoint,
        MinPointMagLinearMipLinear,
        MinPointMagLinearMipPoint,
        Point,
        PointMipLinear
    };

    enum class WrapMode {
        Wrap,
        Mirror,
        Clamp,
        Border,
        // TODO: MirrorOnce see if this is supported on OGL.
    };

    class NEAPI SamplerState : public GraphicsResource {
    public:
        /**
         * Texture filtering mode.
         */
        TextureFilter Filter = TextureFilter::Linear;

        // TODO: Filter modes and Comparison Functions

        /**
         * UV wrap mode in the U direction.
         */
        WrapMode WrapModeU = WrapMode::Wrap;

        /**
         * UV wrap mode in the V direction.
         */
        WrapMode WrapModeV = WrapMode::Wrap;

        /**
         * Anisotropic level for anisotropic filtering.
         * @warning Must be between 1 and 16.
         */
        unsigned int MaxAnisotropy = 1;

        /**
         * Create a new sampler state.
         */
        SamplerState(GraphicsDevice *graphicsDevice);

        /**
         * Bind this sampler state.
         * @param unit Texture/Sampler unit to bind to.
         */
        void bind(unsigned int unit);
    };
}

#endif //NGINE_SAMPLERSTATE_HPP
