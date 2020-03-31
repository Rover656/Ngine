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

#ifndef NGINE_D3D11_GRAPHICS_DEVICE_HPP
#define NGINE_D3D11_GRAPHICS_DEVICE_HPP

#include "ngine/config.hpp"

#if defined(NGINE_ENABLE_DIRECTX)

#include "ngine/graphics/graphics_device.hpp"

#include "d3d11_types.hpp"

#include <d3d11.h>
#include <dxgi1_4.h>
#include <vector>

namespace ngine::graphics::d3d11 {
    class D3D11GraphicsDevice : public IGraphicsDevice {
    public:
        D3D11GraphicsDevice(GraphicsDeviceDesc desc);
        ~D3D11GraphicsDevice();

        Buffer createBuffer(BufferDesc bufferDesc) override;

        void releaseBuffer(Buffer buffer) override;

        SamplerState createSamplerState(SamplerDesc samplerDesc) override;

        void releaseSamplerState(SamplerState samplerState) override;

        Shader createShader(const char *source, ShaderStage stage) override;

        void releaseShader(Shader shader) override;

        PipelineState createPipelineState(PipelineStateDesc pipelineStateDesc) override;

        void releasePipelineState(PipelineState pipelineState) override;

    private:
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_deviceContext;
        ComPtr<IDXGIDevice2> m_dxgiDevice;
        ComPtr<IDXGIAdapter2> m_dxgiAdapter;
        ComPtr<IDXGIFactory2> m_dxgiFactory;
        ComPtr<IDXGISwapChain1> m_swapchain;
        ComPtr<ID3D11RenderTargetView> m_backbuffer;

        //TEMP
        ComPtr<ID3D11RasterizerState> m_rasterizerState;

        D3D11Buffer *m_buffers;
        uint16_t m_bufferCounter;
        std::vector<uint16_t> m_freeBufferSlots;

        D3D11SamplerState *m_samplerStates;
        uint16_t m_samplerStateCounter;
        std::vector<uint16_t> m_freeSamplerStateSlots;
    };
}

#endif

#endif //NGINE_D3D11_GRAPHICS_DEVICE_HPP
