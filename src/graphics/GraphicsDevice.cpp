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

#include "ngine/graphics/GraphicsDevice.hpp"

#include "API/PlatformGLAPI.hpp"

#include "ngine/Console.hpp"
#include "ngine/Window.hpp"
#include "ngine/graphics/RenderTarget.hpp"
#include "ngine/graphics/Renderer.hpp"

#include <cstring>

namespace ngine::graphics {
    bool GraphicsDevice::m_targetAccessed = false;

    GraphicsDevice::GraphicsDevice(Window *window) : m_attachedWindow(window) {
        // Check window
        if (m_attachedWindow == nullptr)
            throw std::runtime_error("Window cannot be null.");

        // Get context version
        auto targetAPI = window->getContextAPI();
        auto major = window->getContextAPIMajorVersion();
        auto minor = window->getContextAPIMinorVersion();

        // Fix default versions
        if (major == -1 && minor == -1) {
            switch (targetAPI) {
            case GraphicsAPI::OpenGL:
                major = 3;
                minor = 3;
                break;
            case GraphicsAPI::OpenGLES:
                major = 2;
                minor = 0;
                break;
            case GraphicsAPI::DirectX:
                // TODO
                break;
            }
        }

        // Check API minimum versions
        switch (targetAPI) {
        case GraphicsAPI::OpenGL:
#if defined(API_OPENGL_ENABLED)
#if !defined(PLATFORM_DESKTOP)
            Console::Fail("GraphicsDevice",
                          "OpenGL is not supported on this platform.");
#endif
            if (major < 2 && (major == 3 && minor < 0))
                Console::fail(
                    "GraphicsDevice",
                    "Target OpenGL version too low, minimum version is 3.0");
            if (major == 4 && minor > 6)
                Console::fail("GraphicsDevice",
                              "OpenGL 4.6 is the latest Ngine supports.");
#else
            Console::fail(
                "GraphicsDevice",
                "OpenGL not supported as Ngine was not built with it enabled.");
#endif
            break;
        case GraphicsAPI::OpenGLES:
#if defined(API_OPENGLES_ENABLED)
            if (major < 2)
                Console::fail("GraphicsDevice", "OpenGL ES 2.0+ required.");
            if (major == 3 && minor > 1)
                Console::fail("GraphicsDevice",
                              "OpenGL ES 3.1 is the latest Ngine supports.");
#else
            Console::fail("GraphicsDevice", "OpenGL ES not supported as Ngine "
                                            "was not built with it enabled.");
#endif
            break;
        }

        // Create API (If context is created by the API, it should be made
        // current.)
        switch (targetAPI) {
        case GraphicsAPI::OpenGL:
        case GraphicsAPI::OpenGLES:
#if defined(API_OPENGL_ENABLED) || defined(API_OPENGLES_ENABLED)
            m_platformAPI = new API::PlatformGLAPI(this);
#endif
            break;
        case GraphicsAPI::DirectX:
            Console::fail("GraphicsDevice", "DirectX not implemented.");
            break;
        default:
            Console::fail("GraphicsDevice", "Cannot determine target API.");
            break;
        }
        Console::notice("GraphicsDevice", "Successfully loaded extensions.");

        // Load default matrices
        m_projectionMatrix = Matrix::Identity;
    }

    GraphicsDevice::~GraphicsDevice() { delete m_platformAPI; }

    Window *GraphicsDevice::getWindow() { return m_attachedWindow; }

    void GraphicsDevice::clear(Color color) { m_platformAPI->clear(color); }

    RenderTarget *GraphicsDevice::getCurrentTarget() {
        if (m_targetCounter > 0)
            return m_targetStack[m_targetCounter - 1];
        return nullptr;
    }

    const Viewport *GraphicsDevice::getCurrentViewport() {
        if (m_targetCounter > 0)
            return m_targetStack[m_targetCounter - 1]->getViewport();
        return m_attachedWindow->getWindowViewport();
    }

    void GraphicsDevice::pushTarget(RenderTarget *target) {
        // Check the stack has space
        if (m_targetCounter >= MAX_TARGETS)
            Console::fail("GraphicsDevice", "Render target stack overflow.");

        // Force render of any batch before swapping target
        for (auto renderer : m_attachedRenderers)
            renderer->renderBatch();

        // Add to stack
        m_targetStack[m_targetCounter] = target;
        m_targetCounter++;

        // Bind
        m_platformAPI->bindRenderTarget(target);
    }

    void GraphicsDevice::popTarget() {
        // Force render of any batch before swapping target
        for (auto renderer : m_attachedRenderers)
            renderer->renderBatch();

        // If target counter greater than 0, decrease
        if (m_targetCounter > 0)
            m_targetCounter--;

        // Start using next target, or unbind
        if (m_targetCounter > 0)
            m_platformAPI->bindRenderTarget(m_targetStack[m_targetCounter - 1]);
        else
            m_platformAPI->bindRenderTarget(nullptr);
    }

    Matrix GraphicsDevice::getProjectionMatrix() const {
        return m_projectionMatrix;
    }

    API::PlatformGraphicsAPI *GraphicsDevice::getAPI() { return m_platformAPI; }
} // namespace ngine::graphics
