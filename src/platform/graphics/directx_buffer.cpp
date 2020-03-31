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

#include "directx_buffer.hpp"

#if defined(NGINE_ENABLE_DIRECTX)

namespace ngine::platform::graphics {
    DirectXBuffer::DirectXBuffer(IGraphicsDevice *graphicsDevice, BufferType type, BufferUsage usage, void *data,
                                 unsigned int size, unsigned int count) : IBuffer(graphicsDevice, type, usage, data,
                                                                                  size, count) {
        // Setup buffer
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = m_usage == BufferUsage::Static ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = m_size * m_count;
        bd.CPUAccessFlags = m_usage == BufferUsage::Static ? 0 : D3D11_CPU_ACCESS_WRITE;

        // Set buffer usage
        switch (m_usage) {
            case BufferUsage::Static:
                bd.Usage = D3D11_USAGE_IMMUTABLE;
                break;
            case BufferUsage::Dynamic:
                bd.Usage = D3D11_USAGE_DYNAMIC;
                bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                break;
        }

        // Set buffer type.
        switch (m_type) {
            case BufferType::Vertex:
                bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                break;
            case BufferType::Index:
                bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
                break;
            case BufferType::Uniform:
                bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                break;
        }

        // Get DX graphics device
        auto gd = (DirectXGraphicsDevice *) m_graphicsDevice;

        // Create buffer
        if (data != nullptr) {
            // Pass initial data
            D3D11_SUBRESOURCE_DATA iData;
            ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
            iData.pSysMem = data;
            gd->Device->CreateBuffer(&bd, &iData, &m_buffer);
        } else gd->Device->CreateBuffer(&bd, nullptr, &m_buffer);
    }

    DirectXBuffer::~DirectXBuffer() {
        m_buffer->Release();
    }

    void DirectXBuffer::bind(int slot) {
        // Get DX graphics device
        auto gd = (DirectXGraphicsDevice *) m_graphicsDevice;
        unsigned int z = 0;

        switch (m_type) {
            case BufferType::Vertex:
                gd->DeviceContext->IASetVertexBuffers(0, 1, &m_buffer, &m_size, &z);
                break;
            case BufferType::Index:
                // TODO
                break;
            case BufferType::Uniform:
                gd->DeviceContext->VSSetConstantBuffers(slot, 1, &m_buffer);
                gd->DeviceContext->PSSetConstantBuffers(slot, 1, &m_buffer);
                break;
        }
    }

    void DirectXBuffer::free() {
        IGraphicsResource::free();
    }

    void DirectXBuffer::_write(void *data, unsigned int count) {
        // Get DX graphics device
        auto gd = (DirectXGraphicsDevice *) m_graphicsDevice;

        // We write dynamically as only dynamic buffers can be written to
        D3D11_MAPPED_SUBRESOURCE ms;
        gd->DeviceContext->Map(m_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, data, m_size * count);
        gd->DeviceContext->Unmap(m_buffer, NULL);
    }
}

#endif // defined(NGINE_ENABLE_DIRECTX)