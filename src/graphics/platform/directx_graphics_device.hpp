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

#ifndef NGINE_DIRECTX_GRAPHICS_DEVICE_HPP
#define NGINE_DIRECTX_GRAPHICS_DEVICE_HPP

#include "ngine/config.hpp"

#if defined(NGINE_ENABLE_DIRECTX)

#include "ngine/graphics/graphics_device.hpp"

#include <d3d11.h>
#include <directxmath.h>

#include <dxgi1_2.h>

namespace ngine::graphics::platform {
    /**
     * Graphics device for DirectX platform.
     */
    class NEAPI DirectXGraphicsDevice : public GraphicsDevice {
        friend class ngine::Window;
    public:
        void clear(Color color) override;

        void drawPrimitives(PrimitiveType primitiveType, int start, int count) override;

        void free(GraphicsResource *resource) override;

    private:
        /**
         * Swapchain
         */
        IDXGISwapChain1* m_swapchain;

        /**
         * Device.
         */
        ID3D11Device* m_device;

        /**
         * Device context.
         */
        ID3D11DeviceContext* m_deviceContext;

        /**
         * Back buffer render target.
         */
        ID3D11RenderTargetView* m_backbuffer;

        /**
         * Rasterizer state.
         */
        ID3D11RasterizerState* m_rasterizerState;

        DirectXGraphicsDevice(Window *window);
        ~DirectXGraphicsDevice();

        void _initBuffer(Buffer *buffer, int size, int count) override;
        void _bindBuffer(Buffer *buffer) override;
        void _writeBuffer(Buffer *buffer, void *data, int count) override;

        void _initShader(Shader *shader, const std::string &source) override;

        void _initShaderProgram(ShaderProgram *prog) override;
        void _shaderProgramAttach(ShaderProgram *prog, Shader *shader) override;
        void _linkShaderProgram(ShaderProgram *prog) override;
        void _useShaderProgram(ShaderProgram *prog) override;

        void _initVertexArray(VertexArray *array) override;
        void _bindVertexArray(VertexArray *array) override;

        void _initTexture(Texture2D *texture, const void *data) override;
        void _bindTexture(unsigned int unit, Texture2D *texture) override;
        int _generateTextureMipmaps(Texture2D *texture) override;

        void _initSamplerState(SamplerState *samplerState) override;
        void _updateSamplerState(unsigned int unit, SamplerState *samplerState) override;
        void _bindSamplerState(unsigned int unit, SamplerState *samplerState) override;

        void _initUniformBuffer(UniformBuffer *uniformBuffer) override;
        void _bindUniformBuffer(UniformBuffer *uniformBuffer) override;

        void _present() override;
        void _onResize() override;
    };
}

#endif // defined(NGINE_ENABLE_DIRECTX)

#endif //NGINE_DIRECTX_GRAPHICS_DEVICE_HPP
