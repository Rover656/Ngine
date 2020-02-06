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

#include "../GraphicsDevice.hpp"
#include "../ShaderProgram.hpp"
#include "../ShaderProgramState.hpp"
#include "../Texture2D.hpp"

namespace ngine::graphics {
    class Buffer;
    class VertexLayout;

    namespace API {
        /**
         * The platform graphics API interface. This defines all functions required of a graphics API and exposes them in a universal way.
         */
        class NEAPI PlatformGraphicsAPI {
        protected:
            /**
             * The graphics device.
             */
            GraphicsDevice *m_graphicsDevice = nullptr;

            /**
             * The current applied vertex layout.
             */
            VertexLayout *m_currentLayout = nullptr;

            /**
             * Actually use the vertex layout.
             *
             * @param layout_ Layout to use.
             */
            virtual void _useVertexLayout(VertexLayout *layout_) = 0;

            /**
             * Actually stop using the vertex layout.
             *
             * @param layout_ Layout to stop.
             */
            virtual void _stopVertexLayout(VertexLayout *layout_) = 0;
        public:
            /**
             * Create a platform API.
             *
             * @param graphicsDevice_ The graphics device to attach to.
             */
            PlatformGraphicsAPI(GraphicsDevice *graphicsDevice_) : m_graphicsDevice(graphicsDevice_) {}
            virtual ~PlatformGraphicsAPI() = default;

            /**
             * Configure the viewport.
             *
             * @param x_ X coordinate.
             * @param y_ Y coordinate.
             * @param width_ Width.
             * @param height_ Height.
             */
            virtual void configureViewport(int x_, int y_, int width_, int height_) = 0;

            /**
             * Clear the display.
             *
             * @param color_ The color to clear with.
             */
            virtual void clear(const Color &color_) = 0;

            /**
             * Create a texture on the GPU.
             *
             * @param texture_ Texture object.
             * @param data_ Pixel data (can be null).
             */
            virtual void createTexture(Texture2D *texture_, unsigned char *data_) = 0;

            /**
             * Delete a texture on the GPU.
             *
             * @param texture_ Texture to delete.
             */
            virtual void deleteTexture(Texture2D *texture_) = 0;

            /**
             * Bind a texture.
             *
             * @param texture_ Texture to bind, null to unbind.
             */
            virtual void bindTexture(Texture2D *texture_) = 0;

            /**
             * Set the texture's filter mode.
             *
             * @param texture_ Texture.
             * @param mode_ Mode to use.
             */
            virtual void setTextureFilterMode(Texture2D *texture_, TextureFilterMode mode_) = 0;

            /**
             * Set the texture's wrap mode.
             *
             * @param texture_ Texture.
             * @param mode_ Mode to use.
             */
            virtual void setTextureWrapMode(Texture2D *texture_, TextureWrapMode mode_) = 0;

            /**
             * Determine if the texture is valid on the GPU.
             *
             * @param texture_ Texture to check.
             * @return Whether or not the texture is valid.
             */
            virtual bool isTextureValid(const Texture2D *texture_) = 0;

            /**
             * Compare two textures.
             *
             * @param a_ Texture A.
             * @param b_ Texture B.
             * @returns Whether or not the textures are the same.
             */
            virtual bool compareTextures(const Texture2D *a_, const Texture2D *b_) = 0;

            /**
             * Create a shader on the GPU.
             *
             * @note This will compile the shader too.
             * @param shader_ Shader object.
             * @param sourceData_ The source data, this will be a GLSL string for OpenGL, HLSL bytecode for DirectX and so on.
             */
            virtual void createShader(Shader *shader_, void *sourceData_) = 0;

            /**
             * Delete a shader.
             *
             * @param shader_ Shader to delete.
             */
            virtual void deleteShader(Shader *shader_) = 0;

            /**
             * Check if a shader is valid.
             *
             * @param shader_ Shader to check.
             */
            virtual bool isShaderValid(const Shader *shader_) = 0;

            /**
             * Create a shader program.
             *
             * @note DirectX does not use these, its just really for OGL.
             * @note This will also link the GL program.
             * @param program_ Program object.
             */
            virtual void createShaderProgram(ShaderProgram *program_) = 0;

            /**
             * Delete a shader program.
             *
             * @param program_ Program to delete.
             */
            virtual void deleteShaderProgram(ShaderProgram *program_) = 0;

            /**
             * Bind the provided shader program.
             *
             * @param program_ Program to bind.
             */
            virtual void bindShaderProgram(const ShaderProgram *program_) = 0;

            /**
             * Check if a shader program is valid.
             *
             * @param program_ Program to check.
             */
            virtual bool isShaderProgramValid(const ShaderProgram *program_) = 0;

            /**
             * Bind a shader program's state.
             *
             * @param state_ State to bind.
             */
            virtual void bindShaderProgramState(ShaderProgramState *state_) = 0;

            /**
             * Create a render target on the GPU.
             *
             * @param renderTarget_ Render target object.
             */
            virtual bool createRenderTarget(RenderTarget *renderTarget_) = 0;

            /**
             * Delete a render target from the GPU.
             *
             * @param renderTarget_ Render target object.
             */
            virtual void deleteRenderTarget(RenderTarget *renderTarget_) = 0;

            /**
             * Bind a render target.
             *
             * @param renderTarget_ Target to bind or null.
             */
            virtual void bindRenderTarget(RenderTarget *renderTarget_) = 0;

            /**
             * Test if a render target is valid.
             *
             * @param renderTarget_ Render target to check.
             * @return Whether or not the render target is valid.
             */
            virtual bool isRenderTargetValid(const RenderTarget *renderTarget_) = 0;

            /**
             * Compare two render targets.
             *
             * @param a_ Render target A.
             * @param b_ Render target B.
             * @return Whether or not the targets are the same.
             */
            virtual bool compareRenderTargets(const RenderTarget *a_, const RenderTarget *b_) = 0;

            /**
             * Bind a buffer.
             *
             * @warning Due to the nature of buffers, you must use `UnbindBuffer` instead of passing null.
             * @param buffer_ The buffer to bind
             */
            virtual void bindBuffer(Buffer *buffer_) = 0;

            /**
             * Unbind a buffer
             *
             * @param buffer_ Buffer to unbind.
             */
            virtual void unbindBuffer(Buffer *buffer_) = 0;

            /**
             * Using a provided buffer, create it on the GPU.
             *
             * @param buffer_ Buffer to initialize.
             */
            virtual void createBuffer(Buffer *buffer_) = 0;

            /**
             * Cleanup buffer on GPU.
             *
             * @param buffer_ Buffer to clean/remove.
             */
            virtual void deleteBuffer(Buffer *buffer_) = 0;

            /**
             * Write the given data to the given buffer.
             *
             * @param buffer_ The buffer to write to.
             * @param data_ The data array
             * @param count_ The number of array entries
             * @param size_ The size of each entry
             * @param update_ GL only, whether or not to use glBufferSubData
             */
            virtual void writeBuffer(Buffer *buffer_, void *data_, int count_, int size_, bool update_) = 0;

            /**
             * Initialize a vertex layout on the GPU.
             *
             * @param layout_ The layout to initialize
             */
            virtual void createVertexLayout(VertexLayout *layout_) = 0;

            /**
             * Cleanup a vertex layout in the GPU.
             *
             * @param layout_ Layout to clean.
             */
            virtual void deleteVertexLayout(VertexLayout *layout_) = 0;

            /**
             * Configure/Finalize the vertex layout in the GPU.
             *
             * @param layout_ Layout to configure.
             */
            virtual void configureVertexLayout(VertexLayout *layout_) = 0;

            /**
             * Use a vertex layout.
             *
             * @param layout_ Layout to use.
             */
            void useVertexLayout(VertexLayout *layout_);

            /**
             * Stop using a vertex layout.
             *
             * @param layout_ Layout to stop.
             */
            void stopVertexLayout(VertexLayout *layout_);

            /**
             * Prepare for rendering 2D
             */
            virtual void prepareFor2D() = 0;

            /**
             * Draw the currently bound buffer array.
             *
             * @param count_ Number of vertices (triangles * 3) to draw.
             * @param start_ The starting index.
             */
            virtual void draw(int count_, int start_) = 0;

            /**
             * Draw the currently bound vertex buffer which is indexed by a bound index buffer.
             *
             * @param count_ Number of vertices (triangles * 3) to draw.
             * @param start_ The starting index.
             */
            virtual void drawIndexed(int count_, int start_) = 0;
        };
    }
}

#endif //PLATFORMGRAPHICSAPI_HPP
