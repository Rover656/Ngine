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

#include "d3d11_graphics_device.hpp"

#if defined(NGINE_ENABLE_DIRECTX)

#include "ngine/console.hpp"
#include "ngine/window.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#endif

namespace ngine::graphics::d3d11 {
    D3D11GraphicsDevice::D3D11GraphicsDevice(GraphicsDeviceDesc desc) : IGraphicsDevice(desc) {
        // Define feature level
        D3D_FEATURE_LEVEL FeatureLevels[] = {
                D3D_FEATURE_LEVEL_12_1,
                D3D_FEATURE_LEVEL_12_0,
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0
        };

        // Setup device creation flags
        unsigned int creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
        // Enable debug
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        // Create device
        HRESULT hr = D3D11CreateDevice(nullptr,
                                       D3D_DRIVER_TYPE_HARDWARE,
                                       nullptr,
                                       creationFlags,
                                       FeatureLevels,
                                       sizeof(FeatureLevels) / sizeof(D3D_FEATURE_LEVEL),
                                       D3D11_SDK_VERSION,
                                       &m_device,
                                       nullptr,
                                       &m_deviceContext);

        if (FAILED(hr)) {
            // Attempt to create WARP device
            hr = D3D11CreateDevice(nullptr,
                                   D3D_DRIVER_TYPE_WARP,
                                   nullptr,
                                   creationFlags,
                                   FeatureLevels,
                                   sizeof(FeatureLevels) / sizeof(D3D_FEATURE_LEVEL),
                                   D3D11_SDK_VERSION,
                                   &m_device,
                                   nullptr,
                                   &m_deviceContext);

            if (FAILED(hr))
                Console::fail("DirectX", "Failed to create device!");
        }

        // Get DXGI device
        hr = m_device->QueryInterface(__uuidof(IDXGIDevice2), (void **) &m_dxgiDevice);
        if (FAILED(hr)) {
            Console::fail("DirectX", "Failed to get DXGI device!");
        }

        // Get DXGI adapter
        hr = m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter2), (void **) &m_dxgiAdapter);
        if (FAILED(hr)) {
            Console::fail("DirectX", "Failed to get DXGI adapter!");
        }

        // Get DXGI factory
        hr = m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void **) &m_dxgiFactory);
        if (FAILED(hr)) {
            Console::fail("DirectX", "Failed to get DXGI factory!");
        }

        // Create swap chain desc
        DXGI_SWAP_CHAIN_DESC1 sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.Width = m_desc.Window->getWidth();
        sd.Height = m_desc.Window->getHeight();
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        // Create swapchain
#if defined(PLATFORM_DESKTOP)
        HWND win = glfwGetWin32Window((GLFWwindow *) m_desc.Window->getHandle());
        hr = m_dxgiFactory->CreateSwapChainForHwnd(m_device.Get(), win, &sd, nullptr, nullptr, &m_swapchain);
#elif defined(PLATFORM_UWP)
        hr = m_dxgiFactory->CreateSwapChainForCoreWindow(m_device, reinterpret_cast<IUnknown*>(CoreWindow::GetForCurrentThread()), &sd, nullptr, &m_swapchain);
#endif
        if (FAILED(hr))
            Console::fail("DirectX", "Failed to create swap chain!");

        Console::notice("DirectX", "Successfully created DX11 device and swap chain.");

        // Get the address of the back buffer
        ID3D11Texture2D *backBuffer;
        hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) &backBuffer);

        if (FAILED(hr)) {
            Console::fail("DirectX", "Failed to get back buffer texture!");
        }

        // Create the render target
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
        ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D.MipSlice = 0;
        hr = m_device->CreateRenderTargetView(backBuffer, &rtvDesc, &m_backbuffer);

        if (FAILED(hr)) {
            Console::fail("DirectX", "Failed to create render target view!");
        }

        // Release back buffer texture
        backBuffer->Release();

        // Set the render target as the back buffer
        m_deviceContext->OMSetRenderTargets(1, &m_backbuffer, nullptr);

        // Set viewport
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = m_desc.Window->getWidth();
        viewport.Height = m_desc.Window->getHeight();

        m_deviceContext->RSSetViewports(1, &viewport);

        // Setup rasterizer state
        D3D11_RASTERIZER_DESC rasterizerDesc;
        ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.CullMode = D3D11_CULL_BACK;
        rasterizerDesc.FrontCounterClockwise = true;

        // Create state
        hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
        if (FAILED(hr)) {
            Console::fail("DirectX", "Failed to create rasterizer state!");
        }

        // Set state
        m_deviceContext->RSSetState(m_rasterizerState.Get());

        // Create arrays for resources
        m_buffers = new D3D11Buffer[m_desc.MaxBuffers]();
        m_bufferCounter = 0;
    }

    D3D11GraphicsDevice::~D3D11GraphicsDevice() {
        // Return to windowed mode
        m_swapchain->SetFullscreenState(false, nullptr);
    }

    Buffer D3D11GraphicsDevice::createBuffer(BufferDesc bufferDesc) {
        // Determine index
        uint16_t index;
        if (!m_freeBufferSlots.empty()) {
            index = m_freeBufferSlots.back();
            m_freeBufferSlots.pop_back();
        } else {
            index = m_bufferCounter++;
        }

        // Get our slot element
        auto bf = &m_buffers[index];
        bf->Desc = bufferDesc;

        // Increase generation
        bf->Generation++;

        // Create buffer
        D3D11_BUFFER_DESC bufDesc;
        ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

        switch (bufferDesc.Usage) {
            case BufferUsage::Static:
                bufDesc.Usage = D3D11_USAGE_IMMUTABLE;
                break;
            case BufferUsage::Dynamic:
                bufDesc.Usage = D3D11_USAGE_DYNAMIC;
                break;
        }

        switch (bufferDesc.Type) {
            case BufferType::Vertex:
                bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                break;
            case BufferType::Index:
                bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                break;
            case BufferType::Uniform:
                bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                break;
        }

        // Create
        if (bufferDesc.InitialData != nullptr) {
            D3D11_SUBRESOURCE_DATA data;
            ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
            data.pSysMem = bufferDesc.InitialData;
            m_device->CreateBuffer(&bufDesc, &data, bf->Buffer.GetAddressOf());
        } else m_device->CreateBuffer(&bufDesc, nullptr, bf->Buffer.GetAddressOf());
        return {index, bf->Generation};
    }

    void D3D11GraphicsDevice::releaseBuffer(Buffer buffer) {
        // Get the slot
        auto bf = &m_buffers[buffer.Index];

        // Ensure the generation is the same!
        if (bf->Generation != buffer.Generation)
            return;

        // Free it and increment the generation
        bf->Buffer = nullptr;
        bf->Generation++;

        // Add to free index list
        m_freeBufferSlots.push_back(buffer.Index);
    }

    SamplerState D3D11GraphicsDevice::createSamplerState(SamplerDesc samplerDesc) {
        // TODO: Cache
        if (false) {
            uint16_t idx = 0;
            return {idx, m_samplerStates[idx].Generation};
        }

        // Determine index
        uint16_t index;
        if (!m_freeBufferSlots.empty()) {
            index = m_freeSamplerStateSlots.back();
            m_freeSamplerStateSlots.pop_back();
        } else {
            index = m_samplerStateCounter++;
        }

        // Get our slot element
        auto ss = &m_samplerStates[index];
        ss->Desc = samplerDesc;

        // Increase generation
        ss->Generation++;

        // Create sampler state
        D3D11_SAMPLER_DESC sDesc;
        sDesc.MinLOD = samplerDesc.MinLOD;
        sDesc.MaxLOD = samplerDesc.MaxLOD;
        sDesc.MipLODBias = samplerDesc.LODBias;
        sDesc.MaxAnisotropy = samplerDesc.MaxAnisotropy;

        // Set border color
        auto col = samplerDesc.BorderColor;
        sDesc.BorderColor[0] = col.R;
        sDesc.BorderColor[1] = col.G;
        sDesc.BorderColor[2] = col.B;
        sDesc.BorderColor[3] = col.A;

        // Set comparison function
        switch (samplerDesc.ComparisonFunction) {
            case CompareFunction::Always:
                sDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
                break;
            case CompareFunction::Equal:
                sDesc.ComparisonFunc = D3D11_COMPARISON_EQUAL;
                break;
            case CompareFunction::Greater:
                sDesc.ComparisonFunc = D3D11_COMPARISON_GREATER;
                break;
            case CompareFunction::GreaterEqual:
                sDesc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
                break;
            case CompareFunction::Less:
                sDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
                break;
            case CompareFunction::LessEqual:
                sDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
                break;
            case CompareFunction::Never:
                sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
                break;
            case CompareFunction::NotEqual:
                sDesc.ComparisonFunc = D3D11_COMPARISON_NOT_EQUAL;
                break;
        }

        // Set filter
        bool compare = samplerDesc.FilterMode == TextureFilterMode::Comparison;
        switch (samplerDesc.Filter) {
            case TextureFilter::Anisotropic:
                sDesc.Filter = compare ? D3D11_FILTER_COMPARISON_ANISOTROPIC
                                       : D3D11_FILTER_ANISOTROPIC;
                break;
            case TextureFilter::Linear:
                sDesc.Filter = compare ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR
                                       : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                break;
            case TextureFilter::LinearMipPoint:
                sDesc.Filter = compare ? D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT
                                       : D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
                break;
            case TextureFilter::MinLinearMagPointMipLinear:
                sDesc.Filter = compare ? D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR
                                       : D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
                break;
            case TextureFilter::MinLinearMagPointMipPoint:
                sDesc.Filter = compare ? D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT
                                       : D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
                break;
            case TextureFilter::MinPointMagLinearMipLinear:
                sDesc.Filter = compare ? D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR
                                       : D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
                break;
            case TextureFilter::MinPointMagLinearMipPoint:
                sDesc.Filter = compare ? D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT
                                       : D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
                break;
            case TextureFilter::Point:
                sDesc.Filter = compare ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT
                                       : D3D11_FILTER_MIN_MAG_MIP_POINT;
                break;
            case TextureFilter::PointMipLinear:
                sDesc.Filter = compare ? D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR
                                       : D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
                break;
        }

        switch (samplerDesc.WrapModeU) {
            case WrapMode::Wrap:
                sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
                break;
            case WrapMode::Mirror:
                sDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
                break;
            case WrapMode::Clamp:
                sDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
                break;
            case WrapMode::Border:
                sDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
                break;
        }

        switch (samplerDesc.WrapModeV) {
            case WrapMode::Wrap:
                sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
                break;
            case WrapMode::Mirror:
                sDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
                break;
            case WrapMode::Clamp:
                sDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
                break;
            case WrapMode::Border:
                sDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
                break;
        }

        // We don't use this at the moment.
        sDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

        // Create new state
        HRESULT hr = m_device->CreateSamplerState(&sDesc, ss->SamplerState.GetAddressOf());
        if (FAILED(hr)) {
            Console::fail("DirectX", "Could not create sampler state!");
        }

        return {index, ss->Generation};
    }

    void D3D11GraphicsDevice::releaseSamplerState(SamplerState samplerState) {

    }

    Shader D3D11GraphicsDevice::createShader(const char *source, ShaderStage stage) {
        return Shader();
    }

    void D3D11GraphicsDevice::releaseShader(Shader shader) {

    }

    PipelineState D3D11GraphicsDevice::createPipelineState(PipelineStateDesc pipelineStateDesc) {
        return PipelineState();
    }

    void D3D11GraphicsDevice::releasePipelineState(PipelineState pipelineState) {

    }
}

#endif
