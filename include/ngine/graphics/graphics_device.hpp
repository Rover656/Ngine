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
#include "shader.hpp"
#include "shader_program.hpp"
#include "vertex_array.hpp"

namespace ngine {
    class Window;

    namespace graphics {
        enum class PrimitiveType {
            TriangleList,
            TriangleStrip,
            LineList,
            LineStrip
        };

        class NEAPI GraphicsDevice {
            friend class ngine::Window;
            friend class ngine::graphics::Buffer;
            friend class ngine::graphics::Shader;
            friend class ngine::graphics::ShaderProgram;
            friend class ngine::graphics::VertexArray;
        public:
            /**
             * Clear the current framebuffer.
             *
             * @param color Color to clear with.
             */
            virtual void clear(Color color) = 0;

            /**
             * Bind a vertex array.
             * @warning Must match the shader's layout if you are going to render with it.
             */
            virtual void bindVertexArray(VertexArray *array) = 0;

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
            GraphicsDevice(Window *window);

            /**
             * Destroy the graphics device.
             */
            virtual ~GraphicsDevice();

            /**
             * The window who owns the context.
             */
            Window *m_window;
        private:
            /**
             * Create a buffer on the GPU.
             */
            virtual void _initBuffer(Buffer *buffer, int size, int count) = 0;
            virtual void _bindBuffer(Buffer *buffer) = 0;
            virtual void _writeBuffer(Buffer *buffer, void* data, int count) = 0;

            virtual void _initShader(Shader *shader, const std::string &source) = 0;

            virtual void _initShaderProgram(ShaderProgram *prog) = 0;
            virtual void _shaderProgramAttach(ShaderProgram *prog, Shader *shader) = 0;
            virtual void _linkShaderProgram(ShaderProgram *prog) = 0;
            virtual void _useShaderProgram(ShaderProgram *prog) = 0; // TODO: Do this through GraphicsDevice to make API more uniform.

            virtual void _initVertexArray(VertexArray *array) = 0;

            virtual void _present() = 0;
            virtual void _onResize() = 0;
        };
    }
}

#endif //NGINE_GRAPHICSDEVICE_HPP
