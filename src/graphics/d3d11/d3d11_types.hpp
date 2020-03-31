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

#ifndef NGINE_D3D11_TYPES_HPP
#define NGINE_D3D11_TYPES_HPP

#include "ngine/config.hpp"

#if defined(NGINE_ENABLE_DIRECTX)

#include "ngine/graphics/types.hpp"

#include <cstdint>
#include <d3d11.h>
#include <wrl.h>

namespace ngine::graphics::d3d11 {
    using namespace Microsoft::WRL;

    struct D3D11Buffer {
        /**
         * The buffer
         */
        ComPtr<ID3D11Buffer> Buffer;

        /**
         * The buffer description
         */
        BufferDesc Desc;

        /**
         * This slot's generation
         */
        uint16_t Generation = 0;
    };

    struct D3D11SamplerState {
        /**
         * The sampler state.
         */
        ComPtr<ID3D11SamplerState> SamplerState;

        /**
         * The sampler description.
         */
        SamplerDesc Desc;

        /**
         * This slot's generation
         */
        uint16_t Generation = 0;
    };
}

#endif

#endif //NGINE_D3D11_TYPES_HPP
