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

#ifndef NGINE_GL_GRAPHICS_DEVICE_HPP
#define NGINE_GL_GRAPHICS_DEVICE_HPP

#include "ngine/config.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#include "ngine/graphics/graphics_device.hpp"

#include "gl_context.hpp"

#include <mutex>
#include <unordered_map>

namespace ngine::platform::graphics::gl {
    // Do this for the sake of cleaner code
    using namespace ngine;
    using namespace ngine::graphics;
    
    /**
     * Graphics device for OpenGL(ES) platform.
     */
    class NEAPI GLGraphicsDevice : public IGraphicsDevice {
        friend class ngine::IWindow;
    public:
        void clear(Color color) override;

        void bindUniformBuffer(unsigned int location, Buffer *buffer) override;

        void drawPrimitives(PrimitiveType primitiveType, int start, int count) override;

        void free(GraphicsResource *resource) override;

    private:
        /**
         * The OpenGL context.
         */
        GLContext *m_context = nullptr;

        /**
         * Whether or not GL_CLAMP_TO_BORDER is available.
         */
        bool m_supportClampToBorder = false;

        /**
         * The GLES clamp to border flag.
         */
        int m_glesClampToBorder = 0;

        /**
         * Whether or not we warned about the lack of support.
         */
        bool m_didWarnClampToBorder = false;

        /**
         * Whether or not anisotropic filtering is supported.
         */
        bool m_supportAnisotropicFiltering = false;

        /**
         * The maximum anisotropy level.
         */
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

        GLGraphicsDevice(IWindow *window);
        ~GLGraphicsDevice();

        void _initBuffer(Buffer *buffer, void *initialData, unsigned int size) override;
        void _writeBuffer(Buffer *buffer, void *data, unsigned int size) override;

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

        void _allocateUniformData(UniformData *uniformData, std::vector<unsigned int> *offsets,
                                  unsigned int *size) override;

        void _freeResource(GraphicsResource *resource);

        void _present() override;
        void _onResize() override;
    };
}

#endif // defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#endif //NGINE_GL_GRAPHICS_DEVICE_HPP
