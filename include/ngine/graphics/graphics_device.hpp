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

#ifndef NGINE_GRAPHICSDEVICE_HPP
#define NGINE_GRAPHICSDEVICE_HPP

#include "ngine/config.hpp"

#include "buffer.hpp"
#include "color.hpp"
#include "sampler_state.hpp"
#include "shader.hpp"
#include "shader_program.hpp"
#include "texture2d.hpp"
#include "uniform_data.hpp"
#include "vertex_array.hpp"

namespace ngine {
    class IWindow;

    namespace graphics {
        enum class PrimitiveType {
            TriangleList,
            TriangleStrip,
            LineList,
            LineStrip
        };

        class NEAPI IGraphicsDevice {
            friend class ngine::IWindow;
            friend class ngine::graphics::Buffer;
            friend class ngine::graphics::SamplerState;
            friend class ngine::graphics::Shader;
            friend class ngine::graphics::ShaderProgram;
            friend class ngine::graphics::Texture2D;
            friend class ngine::graphics::UniformData;
            friend class ngine::graphics::VertexArray;

        public:
            /**
             * Whether or not debug output should be displayed in the game logs.
             */
            bool DoDebugOutput = false;

            /**
             * Clear the current framebuffer.
             *
             * @param color Color to clear with.
             */
            virtual void clear(Color color) = 0;

            /**
             * Bind a uniform buffer to a specified location.
             * @warning This does not check the data format. Ensure your buffer follows the correct format. Do this by using the UniformData class.
             * @param location Buffer destination location.
             * @param buffer Buffer to bind.
             */
            virtual void bindUniformBuffer(unsigned int location, Buffer *buffer) = 0;

            /**
             * Draw primitives from the currently bound buffer.
             */
            virtual void drawPrimitives(PrimitiveType primitiveType, int start, int count) = 0;

            /**
             * Schedule a graphics resource to be freed.
             */
            virtual void free(GraphicsResource *resource) = 0;

            /**
             * Present backbuffer to screen.
             */
            void present();

        protected:
            /**
             * Create a new graphics device.
             * @param window Window whose context we will use.
             */
            IGraphicsDevice(IWindow *window);

            /**
             * Destroy the graphics device.
             */
            virtual ~IGraphicsDevice();

            /**
             * The window who owns the context.
             */
            IWindow *m_window;
        private:
            /**
             * Initialize a buffer on the GPU.
             * @param buffer The buffer to initialize.
             * @param initialData The initial data for the buffer.
             * @param size The size of the buffer (taking into account count).
             */
            virtual void _initBuffer(Buffer *buffer, void *initialData, unsigned int size) = 0;

            /**
             * Write data to a buffer.
             * @param buffer Buffer to write.
             * @param data Data to write to the buffer.
             * @param size The size of the data.
             */
            virtual void _writeBuffer(Buffer *buffer, void *data, unsigned int size) = 0;

            /**
             * Create a shader
             * @param shader Shader to create.
             * @param source The shader source
             * @todo Another one with support for bytecode.
             */
            virtual void _initShader(Shader *shader, const std::string &source) = 0;

            /**
             * Create a shader program.
             * @param prog Program to create
             */
            virtual void _initShaderProgram(ShaderProgram *prog) = 0;

            /**
             * Attach a shader to the shader program
             * @param prog The program to attach to.
             * @param shader The shader to attach.
             */
            virtual void _shaderProgramAttach(ShaderProgram *prog, Shader *shader) = 0;

            /**
             * Link a shader program
             * @param prog Program to link.
             */
            virtual void _linkShaderProgram(ShaderProgram *prog) = 0;

            /**
             * Start using a shader program.
             * @param prog Program to use.
             */
            virtual void _useShaderProgram(ShaderProgram *prog) = 0;

            virtual void _initVertexArray(VertexArray *array) = 0;
            virtual void _bindVertexArray(VertexArray *array) = 0;

            virtual void _initTexture(Texture2D *texture, const void *data) = 0;
            virtual void _bindTexture(unsigned int unit, Texture2D *texture) = 0;
            virtual int _generateTextureMipmaps(Texture2D *texture) = 0;

            virtual void _initSamplerState(SamplerState *samplerState) = 0;
            virtual void _updateSamplerState(unsigned int unit, SamplerState *samplerState) = 0;
            virtual void _bindSamplerState(unsigned int unit, SamplerState *samplerState) = 0;

            virtual void _allocateUniformData(UniformData *uniformData, std::vector<unsigned int> *offsets,
                                              unsigned int *size) = 0;

            virtual void _present() = 0;
            virtual void _onResize() = 0;
        };
    }
}

#endif //NGINE_GRAPHICSDEVICE_HPP
