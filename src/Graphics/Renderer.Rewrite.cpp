/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Renderer.h"

#ifdef USE_EXPERIMENTAL_RENDERER

#include "OpenGL.h"
#include "Rendering/QuadRenderable.h"

namespace NerdThings::Ngine::Graphics {
    void Renderer::_enableGLCapabilities() {
        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::_createDefaultShader() {
        // Vertex source
        std::string vertexShaderSrc =
#if defined(GRAPHICS_OPENGL21)
                "#version 120\n"
#elif defined(GRAPHICS_OPENGLES2)
                "#version 100\n"
#endif
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21)
                "attribute vec3 NG_VertexPos;\n"
                "attribute vec2 NG_VertexTexCoord;\n"
                "attribute vec4 NG_VertexColor;\n"
                "varying vec2 fragTexCoord;\n"
                "varying vec4 fragColor;\n"
#elif defined(GRAPHICS_OPENGL33)
                "#version 330\n"
                "in vec3 NG_VertexPos;\n"
                "in vec2 NG_VertexTexCoord;\n"
                "in vec4 NG_VertexColor;\n"
                "out vec2 fragTexCoord;\n"
                "out vec4 fragColor;\n"
#endif
                "uniform mat4 NGU_MATRIX_MVP;\n"
                "void main()\n"
                "{\n"
                "    fragTexCoord = NG_VertexTexCoord;\n"
                "    fragColor = NG_VertexColor;\n"
                "    gl_Position = NGU_MATRIX_MVP*vec4(NG_VertexPos, 1.0);\n"
                "}\n";

        // Fragment source
        std::string fragmentShaderSrc =
#if defined(GRAPHICS_OPENGL21)
                "#version 120\n"
#elif defined(GRAPHICS_OPENGLES2)
                "#version 100\n"
                "precision mediump float;\n"
#endif
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21)
                "varying vec2 fragTexCoord;\n"
                "varying vec4 fragColor;\n"
#else
                "#version 330\n"
                "in vec2 fragTexCoord;\n"
                "in vec4 fragColor;\n"
                "out vec4 finalColor;\n"
#endif
                "uniform sampler2D NGU_TEXTURE;\n"
                "void main()\n"
                "{\n"
                "    vec4 texelColor = texture2D(NGU_TEXTURE, fragTexCoord);\n"
#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21)
                "    gl_FragColor = texelColor*fragColor;\n"
#elif defined(GRAPHICS_OPENGL33)
                "    finalColor = texelColor*fragColor;\n"
#endif
                "}\n";

        // Create shader
        auto shader = new Shader(vertexShaderSrc, fragmentShaderSrc);
        auto compiled = shader->Compile();

        if (!compiled) {
            delete shader;
            throw std::runtime_error("Default shader failed to compile!");
        }

        // Create shader program
        m_defaultShaderProgram = new ShaderProgram(shader);
        auto linked = m_defaultShaderProgram->Link();
        delete shader;

        if (!linked) {
            delete m_defaultShaderProgram;
            m_defaultShaderProgram = nullptr;
            throw std::runtime_error("Default shader program failed to link!");
        }

        // Create default texture (for shader)
        unsigned char pixels[4] = {255, 255, 255, 255};
        m_whiteTexture = new Texture2D(m_graphicsDevice, pixels, 1, 1);
    }

    bool Renderer::_sortPredicate(Renderer::BatchItem *a_, Renderer::BatchItem *b_) {
        // Sorting by primitive type
        if (a_->PrimitiveType < b_->PrimitiveType) return true;
        if (b_->PrimitiveType < a_->PrimitiveType) return false;

        // Sort by depth
        if (a_->Depth < b_->Depth) return true;

        // Welp, no
        return false;
    }

    void Renderer::_flush() {}

    Renderer::Renderer(GraphicsDevice *graphicsDevice_)
            : m_graphicsDevice(graphicsDevice_) {
        // Check graphics device.
        if (m_graphicsDevice == nullptr)
            throw std::runtime_error("Graphics device cannot be null!");

        // Create default shader.
        _createDefaultShader();
    }

    Renderer::~Renderer() {
        // Delete resources.
        delete m_whiteTexture;
        delete m_defaultShaderProgram;
    }

    void Renderer::Begin(PrimitiveType primitiveType_, Texture2D *texture_, int depth_, ShaderProgram *shader_) {
        // Stop rendering.
        if (m_rendering)
            throw std::runtime_error("Cannot start a new batch while rendering.");

        // Check we aren't midway
        if (m_midBatch)
            throw std::runtime_error("Cannot begin a second batch before finishing the last one.");

        // Get the current target
        auto curTarget = m_graphicsDevice->GetCurrentTarget();

        // We are now running a batch
        m_midBatch = true;
        m_currentBatchTarget = curTarget;

        // Ensure the batch exists for this target
        if (m_batches.find(curTarget) == m_batches.end()) {
            m_batches.insert({curTarget, {{}, {}, {}}});
        }

        // Create the new batch item.
        m_currentBatch = BatchItem();
        m_currentBatch.PrimitiveType = primitiveType_;
        m_currentBatch.Texture = texture_;
        m_currentBatch.Depth = depth_;
        m_currentBatch.Shader = shader_;
    }

    void Renderer::Vertex(Vector2 pos_, Vector2 texCoord_, Color color_) {
        if (!m_midBatch)
            throw std::runtime_error("Start a batch before adding a vertex.");

        // Build vertex data
        Rendering::VertexData vDat;
        vDat.Position = Vector3(pos_.X, pos_.Y, 0);
        vDat.TexCoords = texCoord_;
        vDat.Color = color_;

        m_currentBatch.Vertices.push_back(vDat);
    }

    void Renderer::End() {
        if (!m_midBatch)
            throw std::runtime_error("Start a batch before finishing it.");

        if (m_currentBatch.Depth < 0)
            m_batches[m_currentBatchTarget][Z_NEG].push_back(m_currentBatch);
        else if (m_currentBatch.Depth > 0)
            m_batches[m_currentBatchTarget][Z_POS].push_back(m_currentBatch);
        else m_batches[m_currentBatchTarget][Z_NEU].push_back(m_currentBatch);

        // No longer batching.
        m_midBatch = false;
    }

    void Renderer::Render() {
        // Check we aren't already rendering
        if (m_rendering)
            throw std::runtime_error("Another thread is already rendering. Renderer should only be used on the main thread anyway.");

        // Make sure we're not mid batch.
        if (m_midBatch)
            throw std::runtime_error("Must end the current batch before rendering.");

        // Rendering now!
        m_rendering = true;

        // Configure the current framebuffer for rendering
        m_graphicsDevice->SetupFramebuffer();

        // Enable OpenGL Capabilities
        _enableGLCapabilities();

        // Get the current framebuffer
        auto curFramebuffer = m_graphicsDevice->GetCurrentTarget();

        // TODO: Render this framebuffer's batch

        // Clear the batch
        m_batches[curFramebuffer][Z_NEG].clear();
        m_batches[curFramebuffer][Z_POS].clear();
        m_batches[curFramebuffer][Z_NEU].clear();

        // We're done
        m_rendering = false;
    }

    void Renderer::Clear() {
        // Clear color and depth buffers.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::SetClearColor(Color color_) {
        // Set the clear color.
        glClearColor(color_.R, color_.G, color_.B, color_.A);
    }
}

#endif
