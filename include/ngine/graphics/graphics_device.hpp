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

#ifndef NGINE_GRAPHICSDEVICE_HPP
#define NGINE_GRAPHICSDEVICE_HPP

#include "ngine/config.hpp"

#include "graphics_context.hpp"
#include "types.hpp"

namespace ngine {
    class IWindow;

    namespace graphics {
        struct GraphicsDeviceDesc {
            IWindow *Window;
            ContextDescriptor ContextDescriptor;
            uint16_t MaxBuffers = 1024;
            uint16_t MaxSamplerStates = 64;
            uint16_t MaxShaders = 128;
            uint16_t MaxPipelineStates = 64;
        };

        enum class PrimitiveType {
            TriangleList,
            TriangleStrip,
            LineList,
            LineStrip
        };

        class NEAPI IGraphicsDevice {
        public:
            virtual ~IGraphicsDevice() = default;

            static IGraphicsDevice *createGraphicsDevice(GraphicsDeviceDesc desc);

            IGraphicsContext *getContext() const { return m_context; }

            virtual Buffer createBuffer(BufferDesc bufferDesc) = 0;

            virtual void releaseBuffer(Buffer buffer) = 0;

            virtual SamplerState createSamplerState(SamplerDesc samplerDesc) = 0;

            virtual void releaseSamplerState(SamplerState samplerState) = 0;

            virtual Shader createShader(const char *source, ShaderStage stage) = 0;

            virtual void releaseShader(Shader shader) = 0;

            virtual PipelineState createPipelineState(PipelineStateDesc pipelineStateDesc) = 0;

            virtual void releasePipelineState(PipelineState pipelineState) = 0;
        protected:
            IGraphicsDevice(GraphicsDeviceDesc desc) : m_desc(desc) {}

            /**
             * The graphics device configuration.
             */
            GraphicsDeviceDesc m_desc;

            /**
             * The graphics context.
             * @warning Must be created by the implementation of IGraphicsDevice.
             */
            IGraphicsContext *m_context;
        };
    }
}

#endif //NGINE_GRAPHICSDEVICE_HPP
