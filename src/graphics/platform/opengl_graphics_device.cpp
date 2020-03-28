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

#include "ngine/graphics/platform/opengl_graphics_device.hpp"

#if defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)

#include "ngine/console.hpp"
#include "ngine/window.hpp"

#if !defined(PLATFORM_UWP)
#define GLAD
#include <glad/glad.h>
#endif

#if defined(PLATFORM_DESKTOP)
#define GLFW
#include <GLFW/glfw3.h>
#endif

#if defined(PLATFORM_UWP)
#define EGL
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#if defined(NGINE_ENABLE_OPENGLES) && !defined(GLAD)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1

// Include latest GLES header
#include <GLES3/gl31.h>
// Add GLES2 extensions
#include <GLES2/gl2ext.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstring>

#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY
#define GL_MAX_TEXTURE_MAX_ANISOTROPY 0x84FF
#endif

#ifndef GL_TEXTURE_MAX_ANISOTROPY
#define GL_TEXTURE_MAX_ANISOTROPY 0x84FE
#endif

// We do this here, as if we have two GLGraphicsDevices, these will be the same anyway.
#if defined(GLAD)
static PFNGLGENVERTEXARRAYSPROC glGenVertexArraysOES;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArrayOES;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArraysOES;
#endif

// TODO: Security checks and bind safety (rebinding things after modifiying another i.e. textures).

namespace ngine::graphics::platform {
    void OpenGLGraphicsDevice::clear(Color color) {
        glClearColor(color.R, color.G, color.B, color.A);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLGraphicsDevice::drawPrimitives(PrimitiveType primitiveType, int start, int count) {
        GLenum prim;
        switch (primitiveType) {
            case PrimitiveType::TriangleList:
                prim = GL_TRIANGLES;
                break;
            case PrimitiveType::TriangleStrip:
                prim = GL_TRIANGLE_STRIP;
                break;
            case PrimitiveType::LineList:
                prim = GL_LINES;
                break;
            case PrimitiveType::LineStrip:
                prim = GL_LINE_STRIP;
                break;
        }
        glDrawArrays(prim, start, count);
    }

    void OpenGLGraphicsDevice::free(GraphicsResource *resource) {
        m_freeLock.lock();
        m_freeNextFrame.push_back(resource);
        m_freeLock.unlock();
    }

    OpenGLGraphicsDevice::OpenGLGraphicsDevice(Window *window) : GraphicsDevice(window) {
        // Create context
        m_context = new OpenGLContext(m_window);

        // Make context current on this thread (if it changes then developer isn't using one thread per context)
        //  A thread should have only **one** context current, ever.
        m_context->makeCurrent();

        // Determine if we're running GLES
#if !defined(GLAD)
        if (window->getContextDescriptor().Type == ContextType::OpenGLES) {
            auto major = window->getContextDescriptor().MajorVersion;
            m_isGLES2 = major == 2;
            m_isGLES3 = major == 3;
        }
#else
        m_isGLES2 = GLAD_GL_ES_VERSION_2_0 && !GLAD_GL_ES_VERSION_3_0 &&
                    !GLAD_GL_ES_VERSION_3_1 && !GLAD_GL_ES_VERSION_3_2;
        m_isGLES3 = GLAD_GL_ES_VERSION_3_0;
#endif

        // Determine if we can use sampler objects
        if (!m_isGLES2) { // Only platform that can't is GLES2, we will have a workaround once I get to adding GLES2
            m_supportSamplerObject = true;
            m_supportVAOs = true;
        }

        // Setup viewport
        glViewport(0, 0, m_window->getWidth(), m_window->getHeight());

        // Process extensions
        int numExt = 0;
        const char **extList = nullptr;
        char *toDelete = nullptr;
        if (m_isGLES2) {
            // Create array
            extList = new const char *[512];

            const char *extensions = (const char *)glGetString(GL_EXTENSIONS);

            int len = strlen(extensions) + 1;
            toDelete = (char *)new char[len];
            strcpy(toDelete, extensions);

            extList[numExt] = toDelete;

            for (int i = 0; i < len; i++) {
                if (toDelete[i] == ' ') {
                    toDelete[i] = '\0';
                    numExt++;
                    extList[numExt] = &toDelete[i + 1];
                }
            }
        } else {
#if defined(GLAD)
            // Get extension count
            glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

            // Get extensions
            extList = new const char *[numExt];
            for (auto i = 0; i < numExt; i++)
                extList[i] = (char *)glGetStringi(GL_EXTENSIONS, i);
#endif
        }

        for (auto i = 0; i < numExt; i++) {
            // Check for VAO support
            if (m_isGLES2) {
#if defined(GLAD)
#if defined(GLFW)
                // GLFW does not provide the OES methods, try to find them.
                glGenVertexArraysOES =
                        (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress(
                                "glGenVertexArraysOES");
                glBindVertexArrayOES =
                        (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress(
                                "glBindVertexArrayOES");
                glDeleteVertexArraysOES =
                        (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress(
                                "glDeleteVertexArraysOES");
#endif
                if ((glGenVertexArraysOES != nullptr) &&
                    (glBindVertexArrayOES != nullptr) &&
                    (glDeleteVertexArraysOES != nullptr))
                    m_supportVAOs = true;
#else
                // gl2ext.h provides the functions already.
                m_supportVAOs = true;
#endif
            }

            // Anisotropic texture filter
            if (strcmp(extList[i],
                       (const char *)"GL_EXT_texture_filter_anisotropic") ==
                0) {
                m_supportAnisotropicFiltering = true;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &m_maxAnisotropicLevel);
            }
        }

        // Delete if needed
        delete[] toDelete;
        Console::notice("OpenGL", "OpenGL context created and extensions processed.");

        // Set default nulls
        for (auto i = 0; i < 8; i++) {
            m_textures[i] = nullptr;
            m_samplerStates[i] = nullptr;
        }
    }

    OpenGLGraphicsDevice::~OpenGLGraphicsDevice() {}

    void OpenGLGraphicsDevice::_initBuffer(Buffer *buffer, int size, int count) {
        // Create
        buffer->GLID = 0;
        glGenBuffers(1, &buffer->GLID);

        // Write default size.
        _bindBuffer(buffer);

        GLenum bufType, bufUsage;
        switch (buffer->Type) {
            case BufferType::Vertex:
                bufType = GL_ARRAY_BUFFER;
                break;
            case BufferType::Index:
                bufType = GL_ELEMENT_ARRAY_BUFFER;
                break;
        }

        switch (buffer->Usage) {
            case BufferUsage::Static:
                bufUsage = GL_STATIC_DRAW;
                break;
            case BufferUsage::Dynamic:
                bufUsage = GL_DYNAMIC_DRAW;
                break;
            case BufferUsage::Stream:
                bufUsage = GL_STREAM_DRAW;
                break;
        }
        glBufferData(bufType, size * count, nullptr, bufUsage);
    }

    void OpenGLGraphicsDevice::_bindBuffer(Buffer *buffer) {
        switch (buffer->Type) {
            case BufferType::Vertex:
                glBindBuffer(GL_ARRAY_BUFFER, buffer->GLID);
                break;
            case BufferType::Index:
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->GLID);
                break;
        }
    }

    void OpenGLGraphicsDevice::_writeBuffer(Buffer *buffer, void *data, int count) {
        // Bind
        _bindBuffer(buffer);

        // Get type and usage
        GLenum bufType, bufUsage;
        switch (buffer->Type) {
            case BufferType::Vertex:
                bufType = GL_ARRAY_BUFFER;
                break;
            case BufferType::Index:
                bufType = GL_ELEMENT_ARRAY_BUFFER;
                break;
        }

        // Write (we don't use glBufferData as we want to restrict buffer size for parity with DX11).
        glBufferSubData(bufType, 0, buffer->Size * count, data);
    }

    void OpenGLGraphicsDevice::_initShader(Shader *shader, const std::string &source) {
        // Create shader
        GLenum type;
        switch (shader->Type) {
            case ShaderType::Vertex:
                type = GL_VERTEX_SHADER;
                break;
            case ShaderType::Fragment:
                type = GL_FRAGMENT_SHADER;
                break;
        }
        shader->GLID = glCreateShader(type);

        // Set source
        auto src = source.c_str();
        glShaderSource(shader->GLID, 1, &src, nullptr);

        // Compile
        glCompileShader(shader->GLID);

        // Check for compile errors
        int params = -1;
        glGetShaderiv(shader->GLID, GL_COMPILE_STATUS, &params);
        if (GL_TRUE != params) {
            int max_length = 2048;
            int actual_length = 0;
            char shader_log[2048];
            glGetShaderInfoLog(shader->GLID, max_length, &actual_length, shader_log);
            Console::warn("OpenGL", "Shader info log for shader %u:\n%s", shader->GLID, shader_log);
            Console::fail("OpenGL", "Failed to compile shader %u.", shader->GLID);
        }
        Console::debug("OpenGL", "Successfully created and compiled shader %u.", shader->GLID);
    }

    void OpenGLGraphicsDevice::_initShaderProgram(ShaderProgram *prog) {
        prog->GLID = glCreateProgram();
        Console::debug("OpenGL", "Successfully created shader program %u.", prog->GLID);
    }

    void OpenGLGraphicsDevice::_shaderProgramAttach(ShaderProgram *prog, Shader *shader) {
        glAttachShader(prog->GLID, shader->GLID);
    }

    void OpenGLGraphicsDevice::_linkShaderProgram(ShaderProgram *prog) {
        // Link
        glLinkProgram(prog->GLID);

        // Check for errors
        int params = -1;
        glGetProgramiv(prog->GLID, GL_LINK_STATUS, &params);
        if (GL_TRUE != params) {
            int max_length = 2048;
            int actual_length = 0;
            char program_log[2048];
            glGetProgramInfoLog(prog->GLID, max_length, &actual_length, program_log);
            Console::warn("OpenGL", "Program info log for %u:\n%s", prog->GLID, program_log);
            Console::fail("OpenGL", "Failed to link shader program %u.", prog->GLID);
        }
        Console::debug("OpenGL", "Successfully linked shader program %u.", prog->GLID);
    }

    void OpenGLGraphicsDevice::_useShaderProgram(ShaderProgram *prog) {
        // Use the program
        glUseProgram(prog->GLID);

        // Save current and last programs
        m_lastShaderProgram = m_currentShaderProgram;
        m_currentShaderProgram = prog;

        // Setup VAO if available.
        if (m_currentVAO)
            _prepareVertexArray(m_currentVAO);

        // Setup texture uniforms
        int *samplers = new int[prog->ExpectedSamplerCount];
        for (auto i = 0; i < prog->ExpectedSamplerCount; i++)
            samplers[i] = i;
        int loc = glGetUniformLocation(prog->GLID, prog->GLSLSamplerUniform.c_str());
        glUniform1iv(loc, prog->ExpectedSamplerCount, samplers);
        delete[] samplers;
    }

    void OpenGLGraphicsDevice::_initVertexArray(VertexArray *array) {
        if (m_supportVAOs) {
            // Create VAO
            array->GLID = 0;
            if (m_isGLES2) glGenVertexArraysOES(1, &array->GLID);
            else glGenVertexArrays(1, &array->GLID);
            _prepareVertexArray(array);
            Console::debug("OpenGL", "Created and prepared vertex array %u.", array->GLID);
        }
    }

    void OpenGLGraphicsDevice::_prepareVertexArray(VertexArray *array) {
        // Bind
        if (m_supportVAOs) {
            if (m_isGLES2) glBindVertexArrayOES(array->GLID);
            else glBindVertexArray(array->GLID);
        }

        // Bind buffers
        _bindBuffer(array->getVertexBuffer());
        if (array->getIndexBuffer())
            _bindBuffer(array->getIndexBuffer());

        // Add to cache if missing
        if (m_VAOShaderCache.find(array) == m_VAOShaderCache.end() && m_supportVAOs) {
            m_VAOShaderCache.insert({array, nullptr});
        }

        // If there is no shader, we cannot perform element layouts yet.
        // If we're using the same shader as the last setup, skip
        if (!m_currentShaderProgram || m_VAOShaderCache[array] == m_currentShaderProgram)
            return;

        // Get elements
        auto elements = array->getLayout().Elements;
        auto size = array->getLayout().getSize();

        if (m_lastShaderProgram) {
            for (const auto &e : elements) {
                int loc = glGetAttribLocation(m_lastShaderProgram->GLID, e.Name);
                glDisableVertexAttribArray(loc);
            }
        }

        // Don't configure if mismatched
        if (array->getLayout() != m_currentShaderProgram->getLayout()) {
            // Only warn, as it may be an accident/they set the shader then the array.
            Console::warn("OpenGL", "Cannot configure a vertex array to use a shader with a different layout. Not configuring.");
            return;
        }

        // Prepare layout
        int offset = 0;
        for (const auto &e : elements) {
            GLenum type;
            switch (e.Type) {
//                case ElementType::Byte:
//                    type = GL_BYTE;
//                    break;
//                case ElementType::UnsignedByte:
//                    type = GL_UNSIGNED_BYTE;
//                    break;
//                case ElementType::Short:
//                    type = GL_SHORT;
//                    break;
//                case ElementType::UnsignedShort:
//                    type = GL_UNSIGNED_SHORT;
//                    break;
                case ElementType::Int:
                    type = GL_INT;
                    break;
                case ElementType::UnsignedInt:
                    type = GL_UNSIGNED_INT;
                    break;
                case ElementType::Float:
                    type = GL_FLOAT;
                    break;
            }

            GLint count = e.Count;
            if (count > 4)
                Console::fail("OpenGL", "VertexBufferElement count cannot be greater than 4.");

            // Get location in shader
            int loc = glGetAttribLocation(m_currentShaderProgram->GLID, e.Name);

            // Enable attrib
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, count, type, e.Normalized ? GL_TRUE : GL_FALSE, size,
                                  (void *) offset);
            offset += e.getSize();
        }

        // Register in cache
        if (m_supportVAOs)
            m_VAOShaderCache[array] = m_currentShaderProgram;
    }

    void OpenGLGraphicsDevice::_bindVertexArray(VertexArray *array) {
        if (m_supportVAOs) {
            if (m_isGLES2) glBindVertexArrayOES(array->GLID);
            else glBindVertexArray(array->GLID);
        }

        // Check for changes (with shader attributes)
        _prepareVertexArray(array);
        m_currentVAO = array;
    }

    void OpenGLGraphicsDevice::_initTexture(Texture2D *texture, const void *data) {
        // Create texture
        texture->GLID = 0;
        glGenTextures(1, &texture->GLID);

        // Bind texture and set it up (always use unit 0 as it should be replaced when drawing commences).
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GLID);

        // Load mipmaps
        int mipWidth = texture->getWidth();
        int mipHeight = texture->getHeight();
        int mipOffset = 0;
        auto format = texture->getPixelFormat();

        // Determine image formats
        GLenum glInternalFormat = 0, glFormat = 0, glType = 0;
        switch (texture->getPixelFormat()) {
            case PixelFormat::R8G8B8A8:
                glInternalFormat = GL_RGBA;
                glFormat = GL_RGBA;
                glType = GL_UNSIGNED_BYTE;
                break;
        }

        // Write mip levels
        for (auto i = 0; i < texture->getMipmapCount(); i++) {
            // Get size of mip data
            unsigned int mipSize = Image::getDataSize(mipWidth, mipHeight, format);

            // Write TODO: Compressed formats
            glTexImage2D(GL_TEXTURE_2D, i, glInternalFormat, mipWidth, mipHeight, 0, glFormat, glType, (unsigned char *) data + mipOffset);

            // TODO: Greyscale swizzle

            // Prepare for next mipmap
            mipWidth /= 2;
            mipHeight /= 2;
            mipOffset += mipSize;

            // NPOT textures
            if (mipWidth < 1) mipWidth = 1;
            if (mipHeight < 1) mipHeight = 1;
        }

        Console::debug("OpenGL", "Successfully created texture %u.", texture->GLID);
    }

    void OpenGLGraphicsDevice::_bindTexture(unsigned int unit, Texture2D *texture) {
        // Check unit limit
        if (unit >= 8)
            Console::fail("OpenGL", "Ngine limits the number of texture units to 8.");

        // Set the unit we are using
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture->GLID);
        m_textures[unit] = texture;

        // Update filter
        if (m_samplerStates[unit]) {
            if (m_supportSamplerObject) {
                _applySamplerFiltering(unit, m_samplerStates[unit]);
            } else {
                // We've got to do a full update because we're using fake samplers
                _updateSamplerState(unit, m_samplerStates[unit]);
            }
        }
    }

    int OpenGLGraphicsDevice::_generateTextureMipmaps(Texture2D *texture) {
        // TODO: Deal with lack of NPOT support (GL_OES_texture_npot)

        // Bind and generate
        _bindTexture(0, texture);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Remove "max" macro
#undef max
        return 1 + (int)floor(log(std::max(texture->getWidth(), texture->getHeight()))/log(2));
    }

    void OpenGLGraphicsDevice::_initSamplerState(SamplerState *samplerState) {
        if (m_supportSamplerObject) {
            // Create sampler
            samplerState->GLID = 0;
            glGenSamplers(1, &samplerState->GLID);
            Console::debug("OpenGL", "Created sampler %u.", samplerState->GLID);
        } // We don't do anything if we're using fake samplers.
    }

    void OpenGLGraphicsDevice::_updateSamplerState(unsigned int unit, SamplerState *samplerState) {
        // Apply wrap modes
        _applySamplerWrap(samplerState->GLID, GL_TEXTURE_WRAP_R, samplerState->WrapModeU);
        _applySamplerWrap(samplerState->GLID, GL_TEXTURE_WRAP_S, samplerState->WrapModeV);

        // Apply filter
        _applySamplerFiltering(unit, samplerState);

        // Apply LOD options
        if (m_supportSamplerObject) {
            glSamplerParameterf(samplerState->GLID, GL_TEXTURE_MAX_LOD, samplerState->MaxLOD);
            glSamplerParameterf(samplerState->GLID, GL_TEXTURE_MIN_LOD, samplerState->MinLOD);
#if defined(GLAD)
            // GLES V2 does not support these features.
            if (!m_isGLES2) {
                // Apply border color
                auto col = samplerState->BorderColor;
                float c[4] = { col.R, col.G, col.B, col.A };
                glSamplerParameterfv(samplerState->GLID, GL_TEXTURE_BORDER_COLOR, c);

                glSamplerParameterf(samplerState->GLID, GL_TEXTURE_LOD_BIAS, samplerState->LODBias);
            }
#endif
        } else {
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, samplerState->MaxLOD);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, samplerState->MinLOD);
#if defined(GLAD)
            // GLES V2 does not support these features.
            if (!m_isGLES2) {
                // Apply border color
                auto col = samplerState->BorderColor;
                float c[4] = { col.R, col.G, col.B, col.A };
                glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, c);

                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, samplerState->LODBias);
            }
#endif
        }
    }

    void OpenGLGraphicsDevice::_applySamplerWrap(unsigned int sampler, unsigned int field, WrapMode wrapMode) {
        // TODO: Deal with lack of NPOT support (GL_OES_texture_npot)

        GLint param;
        switch (wrapMode) {
            case WrapMode::Wrap:
                param = GL_REPEAT;
                break;
            case WrapMode::Mirror:
                param = GL_MIRRORED_REPEAT;
                break;
            case WrapMode::Clamp:
                param = GL_CLAMP_TO_EDGE;
                break;
            case WrapMode::Border:
#if defined(GLAD)
                // GLES V2 does not support these features.
                if (!m_isGLES2) {
                    param = GL_CLAMP_TO_BORDER;
                } else {
                    if (!m_haveWarnedWrapBorder) {
                        Console::warn("OpenGL", "OpenGL ES 2.0 does not support WrapMode::Border!");
                        m_haveWarnedWrapBorder = true;
                    }
                    param = GL_CLAMP_TO_EDGE;
                }
#else
                if (!m_haveWarnedWrapBorder) {
                        Console::warn("OpenGL", "OpenGL ES 2.0 does not support WrapMode::Border!");
                        m_haveWarnedWrapBorder = true;
                    }
                    param = GL_CLAMP_TO_EDGE;
#endif
                break;
        }

        if (m_supportSamplerObject)
            glSamplerParameteri(sampler, field, param);
        else glTexParameteri(GL_TEXTURE_2D, field, param);
    }

    void OpenGLGraphicsDevice::_applySamplerFiltering(unsigned int unit, SamplerState *samplerState) {
        // No point if we have no texture yet
        if (!m_textures[unit])
            return;

        GLint paramMin;
        GLint paramMag;
        float anisotropy = 1.0f;
        bool useMipmaps = m_textures[unit]->getMipmapCount() > 1;

        switch (samplerState->Filter) {
            case TextureFilter::Anisotropic:
                anisotropy = (float) samplerState->MaxAnisotropy;
            case TextureFilter::Linear:
                paramMin = useMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR;
                paramMag = GL_LINEAR;
                break;
            case TextureFilter::LinearMipPoint:
                paramMin = useMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
                paramMag = GL_LINEAR;
                break;
            case TextureFilter::MinLinearMagPointMipLinear:
                paramMin = useMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
                paramMag = GL_NEAREST;
                break;
            case TextureFilter::MinLinearMagPointMipPoint:
                paramMin = useMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
                paramMag = GL_NEAREST;
                break;
            case TextureFilter::MinPointMagLinearMipLinear:
                paramMin = useMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
                paramMag = GL_LINEAR;
                break;
            case TextureFilter::MinPointMagLinearMipPoint:
                paramMin = useMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
                paramMag = GL_LINEAR;
                break;
            case TextureFilter::Point:
                paramMin = useMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
                paramMag = GL_NEAREST;
                break;
            case TextureFilter::PointMipLinear:
                paramMin = useMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
                paramMin = GL_NEAREST;
                break;
        }

        // Apply filtering
        if (m_supportSamplerObject) {
            glSamplerParameteri(samplerState->GLID, GL_TEXTURE_MAG_FILTER, paramMag);
            glSamplerParameteri(samplerState->GLID, GL_TEXTURE_MIN_FILTER, paramMin);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, paramMag);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, paramMin);
        }

        // Check anisotropic filtering
        if (!m_supportAnisotropicFiltering)
            anisotropy = 1.0f;
        if (anisotropy > m_maxAnisotropicLevel)
            anisotropy = m_maxAnisotropicLevel;
        if (anisotropy < 1.0f)
            anisotropy = 1.0f;

        // Apply anisotropic filtering
        if (m_supportSamplerObject)
            glSamplerParameterf(samplerState->GLID, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);
        else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);

        // Apply filter mode
        if (samplerState->FilterMode == TextureFilterMode::Comparison) {
            if (m_supportSamplerObject)
                glSamplerParameteri(samplerState->GLID, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

            // Set comparison function
            GLint param;
            switch (samplerState->ComparisonFunction) {
                case CompareFunction::Always:
                    param = GL_ALWAYS;
                    break;
                case CompareFunction::Equal:
                    param = GL_EQUAL;
                    break;
                case CompareFunction::Greater:
                    param = GL_GREATER;
                    break;
                case CompareFunction::GreaterEqual:
                    param = GL_GEQUAL;
                    break;
                case CompareFunction::Less:
                    param = GL_LESS;
                    break;
                case CompareFunction::LessEqual:
                    param = GL_LEQUAL;
                    break;
                case CompareFunction::Never:
                    param = GL_NEVER;
                    break;
                case CompareFunction::NotEqual:
                    param = GL_NOTEQUAL;
                    break;
            }

            glSamplerParameteri(samplerState->GLID, GL_TEXTURE_COMPARE_FUNC, param);
        } else {
            if (m_supportSamplerObject)
                glSamplerParameteri(samplerState->GLID, GL_TEXTURE_COMPARE_MODE, GL_NONE);
            else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        }

        // TODO: Set GL_TEXTURE_MAX_LEVEL
    }

    void OpenGLGraphicsDevice::_bindSamplerState(unsigned int unit, SamplerState *samplerState) {
        // Check unit limit
        if (unit >= 8)
            Console::fail("OpenGL", "Ngine limits the number of texture units to 8.");

        if (m_supportSamplerObject) {
            glBindSampler(unit, samplerState->GLID);
        }
        m_samplerStates[unit] = samplerState;

        // Update (in case of changes)
        _updateSamplerState(unit, samplerState);
    }

    void OpenGLGraphicsDevice::_freeResource(GraphicsResource *resource) {
        // Free resource.
        switch (resource->getResourceType()) {
            case ResourceType::Buffer:
                glDeleteBuffers(1, &resource->GLID);
                break;
            case ResourceType::SamplerState:
                if (m_supportSamplerObject) {
                    glDeleteSamplers(1, &resource->GLID);
                }
                break;
            case ResourceType::Shader:
                glDeleteShader(resource->GLID);
                break;
            case ResourceType::ShaderProgram:
                // TODO: Handle the VAO system if the current shader is removed.
                glDeleteProgram(resource->GLID);
                break;
            case ResourceType::Texture2D:
                // Check this isn't active
                for (auto i = 0; i < 8; i++) {
                    if (m_textures[i] == resource) {
                        glActiveTexture(GL_TEXTURE0 + i);
                        glBindTexture(GL_TEXTURE_2D, 0);
                        m_textures[i] = nullptr;
                    }
                }
                glDeleteTextures(1, &resource->GLID);
                break;
            case ResourceType::VertexArray:
                glDeleteVertexArrays(1, &resource->GLID);
                // Remove from VAO cache
                m_VAOShaderCache.erase((VertexArray *) resource);
                break;
        }
        resource->GLID = 0;
    }

    void OpenGLGraphicsDevice::_present() {
        // SwapBuffers
        m_context->swapBuffers();

        // Dispose resources
        for (auto res : m_freeThisFrame)
            _freeResource(res);
        m_freeThisFrame.clear();

        m_freeLock.lock();
        {
            auto tmp = m_freeThisFrame;
            m_freeThisFrame = m_freeNextFrame;
            m_freeNextFrame = m_freeThisFrame;
        }
        m_freeLock.unlock();
    }

    void OpenGLGraphicsDevice::_onResize() {
        glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
    }
}

#endif // defined(NGINE_ENABLE_OPENGL) || defined(NGINE_ENABLE_OPENGLES)
