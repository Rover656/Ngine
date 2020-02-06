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

#include "Graphics/GraphicsDevice.hpp"

#include "API/PlatformGLAPI.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Console.hpp"

#include <cstring>

namespace ngine::graphics {
    // The default APIs are set here.
    GraphicsAPI GraphicsDevice::m_targetAPI
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#if defined(PLATFORM_DESKTOP) && defined(API_OPENGL_ENABLED)
            = GraphicsAPI::OpenGL
#elif defined(PLATFORM_DESKTOP) && defined(API_OPENGLES_ENABLED)
            = GraphicsAPI::OpenGLES
#elif defined(PLATFORM_UWP) && defined(API_OPENGLES_ENABLED)
            = GraphicsAPI::OpenGLES
#else
#error "Cannot determine default API."
#endif
#endif
    ;

    int GraphicsDevice::m_targetMajorVersion
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#if defined(PLATFORM_DESKTOP) && defined(API_OPENGL_ENABLED)
            = 3
#elif defined(PLATFORM_DESKTOP) && defined(API_OPENGLES_ENABLED)
            = 2
#elif defined(PLATFORM_UWP) && defined(API_OPENGLES_ENABLED)
            = 2
#else
#error "Cannot determine default API."
#endif
#endif
    ;

    int GraphicsDevice::m_targetMinorVersion
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#if defined(PLATFORM_DESKTOP) && defined(API_OPENGL_ENABLED)
            = 3
#elif defined(PLATFORM_DESKTOP) && defined(API_OPENGLES_ENABLED)
            = 0
#elif defined(PLATFORM_UWP) && defined(API_OPENGLES_ENABLED)
            = 0
#else
#error "Cannot determine default API."
#endif
#endif
    ;

    bool GraphicsDevice::m_targetAccessed = false;

    GraphicsDevice::GraphicsDevice(Window *window_) : m_attachedWindow(window_) {
        // Check window
        if (m_attachedWindow == nullptr)
            throw std::runtime_error("Window cannot be null.");

        // Check API minimum versions
        switch (m_targetAPI) {
            case GraphicsAPI::OpenGL:
#if defined(API_OPENGL_ENABLED)
#if !defined(PLATFORM_DESKTOP)
                Console::Fail("GraphicsDevice", "OpenGL is not supported on this platform.");
#endif
                if (m_targetMajorVersion < 2 && (m_targetMajorVersion == 3 && m_targetMinorVersion < 0))
                    Console::Fail("GraphicsDevice", "Target OpenGL version too low, minimum version is 3.0");
                if (m_targetMajorVersion == 4 && m_targetMinorVersion > 6)
                    Console::Fail("GraphicsDevice", "OpenGL 4.6 is the latest Ngine supports.");
#else
                Console::Fail("GraphicsDevice", "OpenGL not supported as Ngine was not built with it enabled.");
#endif
                break;
            case GraphicsAPI::OpenGLES:
#if defined(API_OPENGLES_ENABLED)
                if (m_targetMajorVersion < 2)
                    Console::Fail("GraphicsDevice", "OpenGL ES 2.0+ required.");
                if (m_targetMajorVersion == 3 && m_targetMinorVersion > 1)
                    Console::Fail("GraphicsDevice", "OpenGL ES 3.1 is the latest Ngine supports.");
#else
                Console::Fail("GraphicsDevice", "OpenGL ES not supported as Ngine was not built with it enabled.");
#endif
                break;
        }

        // Create API (If context is created by the API, it should be made current.)
        switch (m_targetAPI) {
            case GraphicsAPI::OpenGL:
            case GraphicsAPI::OpenGLES:
#if defined(API_OPENGL_ENABLED) || defined(API_OPENGLES_ENABLED)
                m_platformAPI = new API::PlatformGLAPI(this);
#endif
                break;
            case GraphicsAPI::DirectX:
                Console::Fail("GraphicsDevice", "DirectX not implemented.");
                break;
            default:
                Console::Fail("GraphicsDevice", "Cannot determine target API.");
                break;
        }
        Console::Notice("GraphicsDevice", "Successfully loaded extensions.");

        // Load default matrices
        m_projectionMatrix = Matrix::Identity;
    }

    GraphicsDevice::~GraphicsDevice() {
        delete m_platformAPI;
    }

    void GraphicsDevice::SetTargetAPI(GraphicsAPI api_, int majorVersion_, int minorVersion_) {
        if (m_targetAccessed)
            Console::Fail("GraphicsDevice", "Cannot change target API once the target has been used.");
        m_targetAPI = api_;
        m_targetMajorVersion = majorVersion_;
        m_targetMinorVersion = minorVersion_;
    }

    GraphicsAPI GraphicsDevice::GetTargetAPI() {
        if (!m_targetAccessed) m_targetAccessed = true;
        return m_targetAPI;
    }

    int GraphicsDevice::GetTargetAPIMajorVersion() {
        if (!m_targetAccessed) m_targetAccessed = true;
        return m_targetMajorVersion;
    }

    int GraphicsDevice::GetTargetAPIMinorVersion() {
        if (!m_targetAccessed) m_targetAccessed = true;
        return m_targetMinorVersion;
    }

    Window *GraphicsDevice::getWindow() {
        return m_attachedWindow;
    }

    void GraphicsDevice::clear(Color color_) {
        m_platformAPI->clear(color_);
    }

    RenderTarget *GraphicsDevice::getCurrentTarget() {
        if (m_targetCounter > 0)
            return m_targetStack[m_targetCounter - 1];
        return nullptr;
    }

    void GraphicsDevice::pushTarget(RenderTarget *target_) {
        // Check the stack has space
        if (m_targetCounter >= MAX_TARGETS)
            Console::Fail("GraphicsDevice", "Render target stack overflow.");

        // Force render of any batch before swapping target
        for (auto renderer : m_attachedRenderers)
            renderer->renderBatch(); // TODO: Review this stuff now.

        // Add to stack
        m_targetStack[m_targetCounter] = target_;
        m_targetCounter++;

        // Bind
        m_platformAPI->bindRenderTarget(target_);
    }

    void GraphicsDevice::popTarget() {
        // Force render of any batch before swapping target
        for (auto renderer : m_attachedRenderers)
            renderer->renderBatch();

        // If target counter greater than 0, decrease
        if (m_targetCounter > 0) m_targetCounter--;
    }

    Matrix GraphicsDevice::getProjectionMatrix() const {
        return m_projectionMatrix;
    }

    Matrix GraphicsDevice::getViewMatrix() const {
        if (m_viewCounter > 0)
            return m_viewMatrixStack[m_viewCounter - 1];
        return Matrix::Identity;
    }

    void GraphicsDevice::pushViewMatrix() {
        if (m_viewCounter >= MAX_MATRICES)
            Console::Fail("GraphicsDevice", "ModelView Matrix stack overflow.");

        // Add to stack
        m_viewMatrixStack[m_viewCounter] = getViewMatrix();
        m_viewCounter++;
    }

    void GraphicsDevice::popViewMatrix() {
        // If target counter greater than 0, decrease
        if (m_viewCounter > 0) m_viewCounter--;
    }

    void GraphicsDevice::loadViewIdentity() {
        if (m_viewCounter == 0) pushViewMatrix();
        m_viewMatrixStack[m_viewCounter - 1] = Matrix::Identity;
    }

    void GraphicsDevice::multView(const Matrix &matrix_) {
        if (m_viewCounter == 0) pushViewMatrix();
        m_viewMatrixStack[m_viewCounter - 1] = m_viewMatrixStack[m_viewCounter - 1] * matrix_;
    }

    void GraphicsDevice::setupFramebuffer() {
        // Bind render target, just to be sure it is done.
        m_platformAPI->bindRenderTarget(m_targetCounter > 0 ? m_targetStack[m_targetCounter - 1] : nullptr);

        // Get viewport width and height.
        int w, h;
        if (m_targetCounter > 0 && m_targetStack[m_targetCounter - 1] != nullptr) {
            w = m_targetStack[m_targetCounter - 1]->Width;
            h = m_targetStack[m_targetCounter - 1]->Height;
        } else {
            w = m_attachedWindow->getWidth();
            h = m_attachedWindow->getHeight();
        }

        // Configure viewport
        m_platformAPI->configureViewport(0, 0, w, h);

        // Create new matrix
        m_projectionMatrix = Matrix::Orthographic(0, (float) w, (float) h, 0, -1, 1);
    }

    API::PlatformGraphicsAPI *GraphicsDevice::getAPI() {
        return m_platformAPI;
    }
}
