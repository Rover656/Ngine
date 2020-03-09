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
         * The platform graphics API interface. This defines all functions
         * required of a graphics API and exposes them in a universal way.
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
             * @param layout Layout to use.
             */
            virtual void _useVertexLayout(VertexLayout *layout) = 0;

            /**
             * Actually stop using the vertex layout.
             *
             * @param layout Layout to stop.
             */
            virtual void _stopVertexLayout(VertexLayout *layout) = 0;

        public:
            /**
             * Create a platform API.
             *
             * @param graphicsDevice The graphics device to attach to.
             */
            PlatformGraphicsAPI(GraphicsDevice *graphicsDevice)
                : m_graphicsDevice(graphicsDevice) {}
            virtual ~PlatformGraphicsAPI() = default;

            /**
             * Configure the viewport.
             *
             * @param x X coordinate.
             * @param y Y coordinate.
             * @param width Width.
             * @param height Height.
             */
            virtual void configureViewport(int x, int y, int width,
                                           int height) = 0;

            /**
             * Clear the display.
             *
             * @param color The color to clear with.
             */
            virtual void clear(const Color &color) = 0;

            /**
             * Create a texture on the GPU.
             *
             * @param texture Texture object.
             * @param data Pixel data (can be null).
             */
            virtual void createTexture(Texture2D *texture,
                                       unsigned char *data) = 0;

            /**
             * Delete a texture on the GPU.
             *
             * @param texture Texture to delete.
             */
            virtual void deleteTexture(Texture2D *texture) = 0;

            /**
             * Bind a texture.
             *
             * @param texture Texture to bind, null to unbind.
             */
            virtual void bindTexture(Texture2D *texture) = 0;

            /**
             * Set the texture's filter mode.
             *
             * @param texture Texture.
             * @param mode Mode to use.
             */
            virtual void setTextureFilterMode(Texture2D *texture,
                                              TextureFilterMode mode) = 0;

            /**
             * Set the texture's wrap mode.
             *
             * @param texture Texture.
             * @param mode Mode to use.
             */
            virtual void setTextureWrapMode(Texture2D *texture,
                                            TextureWrapMode mode) = 0;

            /**
             * Determine if the texture is valid on the GPU.
             *
             * @param texture Texture to check.
             * @return Whether or not the texture is valid.
             */
            virtual bool isTextureValid(const Texture2D *texture) = 0;

            /**
             * Compare two textures.
             *
             * @param a Texture A.
             * @param b Texture B.
             * @returns Whether or not the textures are the same.
             */
            virtual bool compareTextures(const Texture2D *a,
                                         const Texture2D *b) = 0;

            /**
             * Create a shader on the GPU.
             *
             * @note This will compile the shader too.
             * @param shader Shader object.
             * @param sourceData The source data, this will be a GLSL string for
             * OpenGL, HLSL bytecode for DirectX and so on.
             */
            virtual void createShader(Shader *shader, void *sourceData) = 0;

            /**
             * Delete a shader.
             *
             * @param shader Shader to delete.
             */
            virtual void deleteShader(Shader *shader) = 0;

            /**
             * Check if a shader is valid.
             *
             * @param shader Shader to check.
             */
            virtual bool isShaderValid(const Shader *shader) = 0;

            /**
             * Create a shader program.
             *
             * @note DirectX does not use these, its just really for OGL.
             * @note This will also link the GL program.
             * @param program Program object.
             */
            virtual void createShaderProgram(ShaderProgram *program) = 0;

            /**
             * Delete a shader program.
             *
             * @param program Program to delete.
             */
            virtual void deleteShaderProgram(ShaderProgram *program) = 0;

            /**
             * Bind the provided shader program.
             *
             * @param program Program to bind.
             */
            virtual void bindShaderProgram(const ShaderProgram *program) = 0;

            /**
             * Check if a shader program is valid.
             *
             * @param program Program to check.
             */
            virtual bool isShaderProgramValid(const ShaderProgram *program) = 0;

            /**
             * Bind a shader program's state.
             *
             * @param state State to bind.
             */
            virtual void bindShaderProgramState(ShaderProgramState *state) = 0;

            /**
             * Create a render target on the GPU.
             *
             * @param renderTarget Render target object.
             */
            virtual bool createRenderTarget(RenderTarget *renderTarget) = 0;

            /**
             * Delete a render target from the GPU.
             *
             * @param renderTarget Render target object.
             */
            virtual void deleteRenderTarget(RenderTarget *renderTarget) = 0;

            /**
             * Bind a render target.
             *
             * @param renderTarget Target to bind or null.
             */
            virtual void bindRenderTarget(RenderTarget *renderTarget) = 0;

            /**
             * Test if a render target is valid.
             *
             * @param renderTarget Render target to check.
             * @return Whether or not the render target is valid.
             */
            virtual bool
            isRenderTargetValid(const RenderTarget *renderTarget) = 0;

            /**
             * Compare two render targets.
             *
             * @param a Render target A.
             * @param b Render target B.
             * @return Whether or not the targets are the same.
             */
            virtual bool compareRenderTargets(const RenderTarget *a,
                                              const RenderTarget *b) = 0;

            /**
             * Bind a buffer.
             *
             * @warning Due to the nature of buffers, you must use
             * `UnbindBuffer` instead of passing null.
             * @param buffer The buffer to bind
             */
            virtual void bindBuffer(Buffer *buffer) = 0;

            /**
             * Unbind a buffer
             *
             * @param buffer Buffer to unbind.
             */
            virtual void unbindBuffer(Buffer *buffer) = 0;

            /**
             * Using a provided buffer, create it on the GPU.
             *
             * @param buffer Buffer to initialize.
             */
            virtual void createBuffer(Buffer *buffer) = 0;

            /**
             * Cleanup buffer on GPU.
             *
             * @param buffer Buffer to clean/remove.
             */
            virtual void deleteBuffer(Buffer *buffer) = 0;

            /**
             * Write the given data to the given buffer.
             *
             * @param buffer The buffer to write to.
             * @param data The data array
             * @param count The number of array entries
             * @param size The size of each entry
             * @param update GL only, whether or not to use glBufferSubData
             */
            virtual void writeBuffer(Buffer *buffer, void *data, int count,
                                     int size, bool update) = 0;

            /**
             * Initialize a vertex layout on the GPU.
             *
             * @param layout The layout to initialize
             */
            virtual void createVertexLayout(VertexLayout *layout) = 0;

            /**
             * Cleanup a vertex layout in the GPU.
             *
             * @param layout Layout to clean.
             */
            virtual void deleteVertexLayout(VertexLayout *layout) = 0;

            /**
             * Configure/Finalize the vertex layout in the GPU.
             *
             * @param layout Layout to configure.
             */
            virtual void configureVertexLayout(VertexLayout *layout) = 0;

            /**
             * Use a vertex layout.
             *
             * @param layout Layout to use.
             */
            void useVertexLayout(VertexLayout *layout);

            /**
             * Stop using a vertex layout.
             *
             * @param layout Layout to stop.
             */
            void stopVertexLayout(VertexLayout *layout);

            /**
             * Prepare for rendering 2D
             */
            virtual void prepareFor2D() = 0;

            /**
             * Draw the currently bound buffer array.
             *
             * @param count Number of vertices (triangles * 3) to draw.
             * @param start The starting index.
             */
            virtual void draw(int count, int start) = 0;

            /**
             * Draw the currently bound vertex buffer which is indexed by a
             * bound index buffer.
             *
             * @param count Number of vertices (triangles * 3) to draw.
             * @param start The starting index.
             */
            virtual void drawIndexed(int count, int start) = 0;
        };
    } // namespace API
} // namespace ngine::graphics

#endif // PLATFORMGRAPHICSAPI_HPP
