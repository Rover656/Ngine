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

#ifndef PLATFORMGRAPHICSAPI_HPP
#define PLATFORMGRAPHICSAPI_HPP

#include "../../Config.hpp"

#include "../ShaderProgram.hpp"
#include "../Texture2D.hpp"

namespace Ngine::Graphics::API {
    class Buffer;
    class VertexLayout;

    /**
     * The platform renderer interface. This defines what a renderer can do and how.
     */
    class NEAPI PlatformGraphicsAPI {
    protected:
        /**
         * The graphics device.
         */
         GraphicsDevice *m_graphicsDevice = nullptr;

         VertexLayout *m_currentLayout = nullptr;

         virtual void _useVertexLayout(VertexLayout *layout_) = 0;
         virtual void _stopVertexLayout(VertexLayout *layout_) = 0;
    public:
        PlatformGraphicsAPI(GraphicsDevice *graphicsDevice_) : m_graphicsDevice(graphicsDevice_) {}
        virtual ~PlatformGraphicsAPI() = default;

        /**
         * Clear the display.
         *
         * @param color_ The color to clear with.
         */
        virtual void Clear(const Color &color_) = 0;

        /**
         * Bind a texture.
         *
         * @param texture_ Texture to bind, null to unbind.
         */
        virtual void BindTexture(Texture2D *texture_) = 0;

        /**
         * Bind a shader.
         *
         * @param shader_ Shader to bind, null to unbind.
         */
        virtual void BindShader(ShaderProgram *shader_) = 0;

        /**
         * Bind a buffer.
         *
         * @warning Due to the nature of buffers, you must use `UnbindBuffer` instead of passing null.
         * @param buffer_ The buffer to bind
         */
        virtual void BindBuffer(Buffer *buffer_) = 0;

        /**
         * Unbind a buffer
         *
         * @param buffer_ Buffer to unbind.
         */
        virtual void UnbindBuffer(Buffer *buffer_) = 0;

        /**
         * Using a provided buffer, create it on the GPU.
         *
         * @param buffer_ Buffer to initialize.
         */
        virtual void InitializeBuffer(Buffer *buffer_) = 0;

        /**
         * Cleanup buffer on GPU.
         *
         * @param buffer_ Buffer to clean/remove.
         */
        virtual void CleanupBuffer(Buffer *buffer_) = 0;

        /**
         * Write the given data to the given buffer.
         *
         * @param buffer_ The buffer to write to.
         * @param data_ The data array
         * @param count_ The number of array entries
         * @param size_ The size of each entry
         * @param update_ GL only, whether or not to use glBufferSubData
         */
        virtual void WriteBuffer(Buffer *buffer_, void *data_, int count_, int size_, bool update_) = 0;

        virtual void InitializeVertexLayout(VertexLayout *layout_) = 0;

        virtual void CleanupVertexLayout(VertexLayout *layout_) = 0;

        virtual void ConfigureVertexLayout(VertexLayout *layout_) = 0;

        void UseVertexLayout(VertexLayout *layout_);

        void StopVertexLayout(VertexLayout *layout_);

        virtual void Draw(int count_, int start_) = 0;

        virtual void DrawIndexed(int count_, int start_) = 0;
    };
}

#endif //PLATFORMGRAPHICSAPI_HPP
