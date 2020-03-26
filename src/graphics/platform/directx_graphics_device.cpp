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

#include "ngine/graphics/platform/directx_graphics_device.hpp"

#if defined(NGINE_ENABLE_DIRECTX)

#include "ngine/console.hpp"
#include "ngine/window.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <d3dcompiler.h>

namespace ngine::graphics::platform {
    void DirectXGraphicsDevice::clear(Color color) {
        float c[4] = {color.R, color.G, color.B, color.A};
        m_deviceContext->ClearRenderTargetView(m_backbuffer, c);
    }

    void DirectXGraphicsDevice::bindBuffer(Buffer *buffer) {
        auto buf = (ID3D11Buffer *) buffer->Handle;
        switch (buffer->Type) {
            case BufferType::Vertex: {
                unsigned int stride = buffer->Size;
                unsigned int offset = 0;
                m_deviceContext->IASetVertexBuffers(0, 1, &buf, &stride, &offset);
                break;
            }
            case BufferType::Index:
                //m_deviceContext->IASetIndexBuffer(buf, DXGI_FORMAT_, nullptr);
                break;
        }
    }

    void DirectXGraphicsDevice::unbindBuffer(Buffer *buffer) {
        // Do nothing, no need (i think)
    }

    void DirectXGraphicsDevice::unbindBuffer(BufferType type) {
        // Do nothing, no need (i think)
    }

    void DirectXGraphicsDevice::bindVertexArray(VertexArray *array) {
        // Bind vertex array and index buffer (if present)
        bindBuffer(array->getVertexBuffer());
        if (array->getIndexBuffer())
            bindBuffer(array->getIndexBuffer());
    }

    void DirectXGraphicsDevice::drawPrimitives(PrimitiveType primitiveType, int start, int count) {
        switch (primitiveType) {
            case PrimitiveType::TriangleList:
                m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                break;
            case PrimitiveType::TriangleStrip:
                m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
                break;
            case PrimitiveType::LineList:
                m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
                break;
            case PrimitiveType::LineStrip:
                m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
                break;
        }
        m_deviceContext->Draw(count, start);
    }

    void DirectXGraphicsDevice::free(GraphicsResource *resource) {
        switch (resource->getResourceType()) {
            case ResourceType::Buffer:
                break;
            case ResourceType::Shader:
                switch (((Shader *) resource)->Type) {
                    case ShaderType::Vertex:
                        ((ID3D11VertexShader *) resource->Handle)->Release();
                        ((ID3DBlob *) resource->Handle1)->Release();
                        break;
                    case ShaderType::Fragment:
                        ((ID3D11PixelShader *) resource->Handle)->Release();
                        ((ID3DBlob *) resource->Handle1)->Release();
                        break;
                }
                break;
            case ResourceType::ShaderProgram:
                ((ID3D11InputLayout *) resource->Handle)->Release();
                break;
            case ResourceType::VertexArray:
                break;
        }
    }

    DirectXGraphicsDevice::DirectXGraphicsDevice(Window *window) : GraphicsDevice(window) {
        // Get window handle
        HWND win = glfwGetWin32Window((GLFWwindow *) m_window->getHandle());

        // Create swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 1;
        sd.BufferDesc.Width = m_window->getWidth();
        sd.BufferDesc.Height = m_window->getHeight();
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = win;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE; // TODO: Window fullscreening
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        // Define feature level
        D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;

        HRESULT hr = S_OK;
        D3D_FEATURE_LEVEL FeatureLevel;

        unsigned int deviceFlags = 0;

#if defined(_DEBUG)
        deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        hr = D3D11CreateDeviceAndSwapChain(NULL,
                                           D3D_DRIVER_TYPE_HARDWARE,
                                           NULL,
                                           deviceFlags,
                                           &FeatureLevels,
                                           1,
                                           D3D11_SDK_VERSION,
                                           &sd,
                                           &m_swapchain,
                                           &m_device,
                                           &FeatureLevel,
                                           &m_deviceContext);

        if (FAILED (hr)) {
            // Attempt to create a warp device
            hr = D3D11CreateDeviceAndSwapChain(NULL,
                                               D3D_DRIVER_TYPE_WARP,
                                               NULL,
                                               deviceFlags,
                                               &FeatureLevels,
                                               1,
                                               D3D11_SDK_VERSION,
                                               &sd,
                                               &m_swapchain,
                                               &m_device,
                                               &FeatureLevel,
                                               &m_deviceContext);

            if (FAILED(hr))
                Console::fail("DirectX", "Failed to create device and swap chain!");
        }
        Console::notice("DirectX", "Successfully created DX11 device and swap chain.");

        // Get the address of the back buffer
        ID3D11Texture2D *backBuffer;
        hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) &backBuffer);

        if (FAILED(hr)) {
            Console::fail("DirectX", "Failed to get back buffer texture!");
        }

        // Create the render target
        hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_backbuffer);

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
        viewport.Width = m_window->getWidth();
        viewport.Height = m_window->getHeight();

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
        m_deviceContext->RSSetState(m_rasterizerState);
    }

    DirectXGraphicsDevice::~DirectXGraphicsDevice() {
        // Return to windowed mode
        m_swapchain->SetFullscreenState(false, nullptr);

        // Release swapchain, device and device context
        m_swapchain->Release();
        m_backbuffer->Release();
        m_device->Release();
        m_deviceContext->Release();
    }

    void DirectXGraphicsDevice::_initBuffer(Buffer *buffer, int size, int count) {
        // Setup buffer
        ID3D11Buffer *handle;
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC; // We ignore usage for now, I will learn immutable sometime
        bd.ByteWidth = size * count;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        // Set buffer type.
        switch (buffer->Type) {
            case BufferType::Vertex:
                bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                break;
            case BufferType::Index:
                bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
                break;
        }

        // Create buffer
        m_device->CreateBuffer(&bd, NULL, &handle);
        buffer->Handle = (void *) handle;
    }

    void
    DirectXGraphicsDevice::_writeBuffer(Buffer *buffer, void *data, int count) {
        auto handle = (ID3D11Buffer *) buffer->Handle;

        // Map and write.
        D3D11_MAPPED_SUBRESOURCE ms;
        m_deviceContext->Map(handle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, data, buffer->Size * count);
        m_deviceContext->Unmap(handle, NULL);
    }

    void DirectXGraphicsDevice::_initShader(Shader *shader, const std::string &source) {
        // Load and compile shader
        ID3DBlob *blob;
        HRESULT hr;
        ID3D11VertexShader *vshdr;
        ID3D11PixelShader *pshdr;

        ID3DBlob *error;

        switch (shader->Type) {
            case ShaderType::Vertex:
                hr = D3DCompile(source.c_str(), source.length(), nullptr, nullptr, nullptr, "VSMain", "vs_4_0",
                                0, 0, &blob, &error);

                if (FAILED(hr)) {
                    if (error) {
                        Console::error("DirectX", "Error: %s", (char *) error->GetBufferPointer());
                    }
                    Console::fail("DirectX", "Failed to compile vertex shader.");
                }
                Console::notice("DirectX", "Successfully compiled vertex shader blob.");

                m_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vshdr);
                shader->Handle = vshdr;
                shader->Handle1 = blob;
                break;
            case ShaderType::Fragment:
                hr = D3DCompile(source.c_str(), source.length(), nullptr, nullptr, nullptr, "PSMain", "ps_4_0",
                                0, 0, &blob, &error);

                if (FAILED(hr)) {
                    if (error) {
                        Console::error("DirectX", "Error: %s", (char *) error->GetBufferPointer());
                    }
                    Console::fail("DirectX", "Failed to compile fragment/pixel shader.");
                }
                Console::notice("DirectX", "Successfully compiled fragment/pixel shader blob.");

                m_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pshdr);
                shader->Handle = pshdr;
                shader->Handle1 = blob;
                break;
        }
    }

    void DirectXGraphicsDevice::_initShaderProgram(ShaderProgram *prog) {}

    void DirectXGraphicsDevice::_shaderProgramAttach(ShaderProgram *prog, Shader *shader) {}

    void DirectXGraphicsDevice::_linkShaderProgram(ShaderProgram *prog) {
        // Here we setup the input layout as we will have all the shaders attached by now.
        std::vector<D3D11_INPUT_ELEMENT_DESC> layoutTemplate;

        int offset = 0;
        for (const auto &e : prog->getLayout().Elements) {
            D3D11_INPUT_ELEMENT_DESC desc;
            ZeroMemory(&desc, sizeof(D3D11_INPUT_ELEMENT_DESC));
            desc.SemanticName = e.Name;
            desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            desc.AlignedByteOffset = offset;

            // Determine format
            switch (e.Type) {
                case ElementType::Int:
                    switch (e.Count) {
                        case 1:
                            desc.Format = DXGI_FORMAT_R32_SINT;
                            break;
                        case 2:
                            desc.Format = DXGI_FORMAT_R32G32_SINT;
                            break;
                        case 3:
                            desc.Format = DXGI_FORMAT_R32G32B32_SINT;
                            break;
                        case 4:
                            desc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
                            break;
                    }
                    break;
                case ElementType::UnsignedInt:
                    switch (e.Count) {
                        case 1:
                            desc.Format = DXGI_FORMAT_R32_UINT;
                            break;
                        case 2:
                            desc.Format = DXGI_FORMAT_R32G32_UINT;
                            break;
                        case 3:
                            desc.Format = DXGI_FORMAT_R32G32B32_UINT;
                            break;
                        case 4:
                            desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
                            break;
                    }
                    break;
                case ElementType::Float:
                    switch (e.Count) {
                        case 1:
                            desc.Format = DXGI_FORMAT_R32_FLOAT;
                            break;
                        case 2:
                            desc.Format = DXGI_FORMAT_R32G32_FLOAT;
                            break;
                        case 3:
                            desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                            break;
                        case 4:
                            desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                            break;
                    }
                    break;
            }

            // Add to layout
            layoutTemplate.push_back(desc);
            offset += e.getSize();
        }

        // Get vertex shader
        auto vs = (ID3DBlob *) prog->getShaderByType(ShaderType::Vertex)->Handle1;

        // Create layout
        ID3D11InputLayout *layout;
        HRESULT hr = m_device->CreateInputLayout(layoutTemplate.data(), layoutTemplate.size(), vs->GetBufferPointer(), vs->GetBufferSize(), &layout);

        if (FAILED(hr)) {
            Console::fail("DirectX", "Failed to create input layout!");
        }

        prog->Handle = layout;
    }

    void DirectXGraphicsDevice::_useShaderProgram(ShaderProgram *prog) {
        // Enable the shaders
        m_deviceContext->VSSetShader((ID3D11VertexShader *) prog->getShaderByType(ShaderType::Vertex)->Handle, nullptr,
                                     0);
        m_deviceContext->PSSetShader((ID3D11PixelShader *) prog->getShaderByType(ShaderType::Fragment)->Handle, nullptr,
                                     0);
        m_deviceContext->IASetInputLayout((ID3D11InputLayout *) prog->Handle);
    }

    void DirectXGraphicsDevice::_initVertexArray(VertexArray *array) {}

    void DirectXGraphicsDevice::_present() {
        m_swapchain->Present(0, 0);
    }

    void DirectXGraphicsDevice::_onResize() {
        if (m_swapchain) {
            m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);

            // Release references to backbuffer
            m_backbuffer->Release();

            // Resize buffers
            HRESULT hr = m_swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

            if (FAILED(hr)) {
                Console::fail("DirectX", "Failed to resize buffers!");
            }

            // Get the address of the back buffer
            ID3D11Texture2D *backBuffer;
            hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) &backBuffer);

            if (FAILED(hr)) {
                Console::fail("DirectX", "Failed to get back buffer texture!");
            }

            // Create the render target
            hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_backbuffer);

            if (FAILED(hr)) {
                Console::fail("DirectX", "Failed to create render target view!");
            }

            backBuffer->Release();

            // Set the render target as the back buffer
            m_deviceContext->OMSetRenderTargets(1, &m_backbuffer, nullptr);

            // Set viewport
            D3D11_VIEWPORT viewport;
            ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
            viewport.TopLeftX = 0;
            viewport.TopLeftY = 0;
            viewport.Width = m_window->getWidth();
            viewport.Height = m_window->getHeight();

            m_deviceContext->RSSetViewports(1, &viewport);
        }
    }
}

#endif // defined(NGINE_ENABLE_DIRECTX)
