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

#ifndef NGINE_DIRECTX_BUFFER_HPP
#define NGINE_DIRECTX_BUFFER_HPP

#include "ngine/config.hpp"

#if defined(NGINE_ENABLE_DIRECTX)

#include "ngine/graphics/buffer.hpp"

#include "directx_graphics_device.hpp"

namespace ngine::platform::graphics {
    using namespace ngine;
    using namespace ngine::graphics;

    class DirectXBuffer : public IBuffer {
    public:
        DirectXBuffer(IGraphicsDevice *graphicsDevice, BufferType type, BufferUsage usage, void *data,
                      unsigned int size, unsigned int count);

        ~DirectXBuffer();

        void bind(int slot = 0) override;

        void free() override;

    protected:
        /**
         * The DirectX buffer.
         */
        ID3D11Buffer *m_buffer;

        void _write(void *data, unsigned int count) override;
    };
}

#endif // defined(NGINE_ENABLE_DIRECTX)

#endif //NGINE_DIRECTX_BUFFER_HPP
