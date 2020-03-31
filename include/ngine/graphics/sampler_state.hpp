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

#include "color.hpp"
#include "graphics_resource.hpp"

namespace ngine::graphics {
    /**
     * Texture filter types.
     */
    enum class TextureFilter {
        /**
         * Use anisotropic filtering.
         */
        Anisotropic,

        /**
         * Use linear filtering.
         */
        Linear,

        /**
         * Use linear filtering to shrink or expand and point filtering between mipmap levels.
         */
        LinearMipPoint,

        /**
         * Use linear filtering to shrink, point filtering to expand and linear filtering between mipmap levels.
         */
        MinLinearMagPointMipLinear,

        /**
         * Use linear filtering to shrink, point filtering to expand and point filtering between mipmap levels.
         */
        MinLinearMagPointMipPoint,

        /**
         * Use point filtering to shrink, linear filtering to expand and linear filtering between mipmap levels.
         */
        MinPointMagLinearMipLinear,

        /**
         * Use point filtering to shrink, linear filtering to expand and point filtering between mipmap levels.
         */
        MinPointMagLinearMipPoint,

        /**
         * Use point filtering.
         */
        Point,

        /**
         * Use point filtering to shrink or expand and linear filtering between mipmap levels.
         */
        PointMipLinear
    };

    /**
     * Filtering mode for texture sampling.
     */
    enum class TextureFilterMode {
        Default,
        Comparison
    };

    /**
     * Comparison function used during depth, stencil and alpha tests.
     */
    enum class CompareFunction {
        /**
         * Test always passes
         */
        Always,

        /**
         * Passes when the new pixel is equal to the current.
         */
        Equal,

        /**
         * Passes when the new pixel is greater than the current.
         */
        Greater,

        /**
         * Passes when the new pixel is greater than or equal to the current.
         */
        GreaterEqual,

        /**
         * Passes when the new pixel is less than the current.
         */
        Less,

        /**
         * Passes when the new pixel is less than or equal to the current.
         */
        LessEqual,

        /**
         * Test never passes.
         */
        Never,

        /**
         * Passes when the new pixel is not equal to the current.
         */
        NotEqual
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

        /**
         * Texture filtering mode.
         */
        TextureFilterMode FilterMode = TextureFilterMode::Default;

        /**
         * Comparison function for TextureFilterMode::Comparison.
         */
        CompareFunction ComparisonFunction = CompareFunction::Never;

        /**
         * The color used for border texels.
         */
        Color BorderColor = Color(0, 0, 0, 0);

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
         * The minimum level of detail.
         */
        float MinLOD = -1000.0f;

        /**
         * The maximum level of detail.
         */
        float MaxLOD = 1000.0f;

        /**
         * The level of detail bias.
         */
        float LODBias = 0.0f;

        /**
         * Create a new sampler state.
         */
        SamplerState(IGraphicsDevice *graphicsDevice);

        /**
         * Bind this sampler state.
         * @param unit Texture/Sampler unit to bind to.
         */
        void bind(unsigned int unit);
    };
}

#endif //NGINE_SAMPLERSTATE_HPP
