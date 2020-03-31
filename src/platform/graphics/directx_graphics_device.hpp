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

namespace ngine::platform::graphics {
    // Do this for the sake of cleaner code
    using namespace ngine;
    using namespace ngine::graphics;

    /**
     * Graphics device for DirectX platform.
     */
    class NEAPI DirectXGraphicsDevice : public IGraphicsDevice {
        friend class ngine::IWindow;

    public:

        /**
         * Swapchain
         */
        IDXGISwapChain1* Swapchain;

        /**
         * Device.
         */
        ID3D11Device* Device;

        /**
         * Device context.
         */
        ID3D11DeviceContext* DeviceContext;

        /**
         * Back buffer render target.
         */
        ID3D11RenderTargetView* Backbuffer;

        /**
         * Rasterizer state.
         * @todo Abstract
         */
        ID3D11RasterizerState* RasterizerState;

        /**
         * The DXGI device.
         */
        IDXGIDevice2 *DxgiDevice;

        /**
         * The DXGI adapter.
         */
        IDXGIAdapter2 *DxgiAdapter;

        /**
         * The DXGI factory.
         */
        IDXGIFactory2* DxgiFactory;

        void clear(Color color) override;

        IBuffer *
        createBuffer(BufferType type, BufferUsage usage, void *data, unsigned int size, unsigned int count) override;

        IUniformDataManager *createUniformDataManager(std::vector<Uniform> layout) override;

        void drawPrimitives(PrimitiveType primitiveType, int start, int count) override;

        void free(IGraphicsResource *resource) override;

    private:
        DirectXGraphicsDevice(IWindow *window);

        ~DirectXGraphicsDevice();
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

        void _present() override;

        void _onResize() override;
    };
}

#endif // defined(NGINE_ENABLE_DIRECTX)

#endif //NGINE_DIRECTX_GRAPHICS_DEVICE_HPP
