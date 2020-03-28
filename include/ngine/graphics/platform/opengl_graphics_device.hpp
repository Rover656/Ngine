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

#ifndef NGINE_OPENGL_GRAPHICS_DEVICE_HPP
#define NGINE_OPENGL_GRAPHICS_DEVICE_HPP

#include "ngine/config.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#include "../graphics_device.hpp"
#include "opengl_context.hpp"

#include <mutex>
#include <unordered_map>

namespace ngine::graphics::platform {
    /**
     * Graphics device for OpenGL(ES) platform.
     */
    class NEAPI OpenGLGraphicsDevice : public GraphicsDevice {
        friend class ngine::Window;
    public:
        void clear(Color color) override;

        void drawPrimitives(PrimitiveType primitiveType, int start, int count) override;

        void free(GraphicsResource *resource) override;

    private:
        /**
         * The OpenGL context.
         */
        OpenGLContext *m_context = nullptr;

        /**
         * Whether or not the context is GLES2.
         */
        bool m_isGLES2 = false;

        /**
         * Whether or not the context is GLES3.
         */
        bool m_isGLES3 = false;

        /**
         * Whether or not sampler objects are supported.
         */
        bool m_supportSamplerObject = false;

        bool m_supportAnisotropicFiltering = false;

        float m_maxAnisotropicLevel = 1.0f;

        /**
         * The list of currently bound textures.
         */
        Texture2D *m_textures[8];

        /**
         * The currently active sampler states (or null).
         * For adding non-samplerobject based samplers.
         * Tracks one sampler for each texture unit and applies its properties to a given texture when bound.
         */
        SamplerState *m_samplerStates[8];

        /**
         * The current VAO.
         */
        VertexArray *m_currentVAO = nullptr;

        /**
         * The last applied shader program.
         */
        ShaderProgram *m_lastShaderProgram = nullptr;

        /**
         * The current applied shader program.
         */
        ShaderProgram *m_currentShaderProgram = nullptr;

        /**
         * Resources to be freed this frame.
         */
        std::vector<GraphicsResource *> m_freeThisFrame;

        /**
         * Resources to be freed next frame.
         */
        std::vector<GraphicsResource *> m_freeNextFrame;

        /**
         * Resource free lock.
         */
        std::mutex m_freeLock;

        /**
         * VAO Shader cache.
         */
        std::unordered_map<VertexArray *, ShaderProgram *> m_VAOShaderCache;

        OpenGLGraphicsDevice(Window *window);
        ~OpenGLGraphicsDevice();

        void _initBuffer(Buffer *buffer, int size, int count) override;
        void _bindBuffer(Buffer *buffer) override;
        void _writeBuffer(Buffer *buffer, void *data, int count) override;

        void _initShader(Shader *shader, const std::string &source) override;

        void _initShaderProgram(ShaderProgram *prog) override;
        void _shaderProgramAttach(ShaderProgram *prog, Shader *shader) override;
        void _linkShaderProgram(ShaderProgram *prog) override;
        void _useShaderProgram(ShaderProgram *prog) override;

        void _initVertexArray(VertexArray *array) override;
        void _prepareVertexArray(VertexArray *array);
        void _bindVertexArray(VertexArray *array) override;

        void _initTexture(Texture2D *texture, const void *data) override;
        void _bindTexture(unsigned int unit, Texture2D *texture) override;
        int _generateTextureMipmaps(Texture2D *texture) override;

        void _initSamplerState(SamplerState *samplerState) override;
        void _updateSamplerState(unsigned int unit, SamplerState *samplerState) override;
        void _applySamplerWrap(unsigned int sampler, unsigned int field, WrapMode wrapMode);
        void _applySamplerFiltering(unsigned int unit, SamplerState *samplerState);
        void _bindSamplerState(unsigned int unit, SamplerState *samplerState) override;

        void _freeResource(GraphicsResource *resource);

        void _present() override;
        void _onResize() override;
    };
}

#endif // defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#endif //NGINE_OPENGL_GRAPHICS_DEVICE_HPP
