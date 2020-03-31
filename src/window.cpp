/**********************************************************************************************
 *
 *   Ngine - A game framework.
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

#include "ngine/window.hpp"

#include "ngine/console.hpp"

// Include them all, they won't implement anything if they're not enabled.
#include "graphics/d3d11/d3d11_graphics_device.hpp"

// Include our sub-types so we can create them
#include "platform/glfw_window.hpp"
#include "platform/uwp_window.hpp"

namespace ngine {
    IWindow::~IWindow() {
        delete m_graphicsDevice;
        Console::notice("Window", "Successfully closed window.");
    }

    IWindow *IWindow::createWindow(WindowDesc windowDesc) {
#if defined(PLATFORM_DESKTOP)
        return new platform::GLFWWindow(windowDesc);
#elif defined(PLATFORM_UWP)
        return new platform::UWPWindow(config);
#else
        Console::fail("Game", "Could not determine which window manager to use!");
#endif
    }

    const graphics::ContextDescriptor IWindow::getContextDescriptor() const {
        return m_desc.GraphicsDeviceDesc.ContextDescriptor;
    }

    graphics::IGraphicsDevice *IWindow::getGraphicsDevice() {
        return m_graphicsDevice;
    }

    const graphics::IGraphicsDevice *IWindow::getGraphicsDevice() const {
        return m_graphicsDevice;
    }

    float IWindow::getWidth() const {
        return m_windowWidth;
    }

    float IWindow::getHeight() const {
        return m_windowHeight;
    }

    IWindow::IWindow(WindowDesc windowDesc)
            : m_desc(std::move(windowDesc)) {
        m_desc.GraphicsDeviceDesc.Window = this;
    }

    int IWindow::m_windowCount = 0;

    void IWindow::_verifyContextDescriptor() {
        switch (m_desc.GraphicsDeviceDesc.ContextDescriptor.verify()) {
            case graphics::ContextDescriptorStatus::OK:
                Console::debug("Window", "Context Descriptor status OK.");
                return;
            case graphics::ContextDescriptorStatus::Outdated:
                Console::warn("Window",
                              "The desired context version is too old for Ngine. Please raise the version number to a supported version.");
                break;
            case graphics::ContextDescriptorStatus::InvalidVersion:
                Console::warn("Window", "The desired context version is invalid.");
                break;
            case graphics::ContextDescriptorStatus::NotEnabledOrSupported:
                Console::warn("Window",
                              "The desired context type is not enabled or is not supported on this platform.");
                break;
            case graphics::ContextDescriptorStatus::NotImplemented:
                Console::warn("Window", "The desired context type has not been implemented into Ngine.");
                break;
        }

        // Use the default descriptor
        m_desc.GraphicsDeviceDesc.ContextDescriptor = graphics::ContextDescriptor::Default;
        Console::warn("Window", "Creating a default context for the current platform!");
    }

    void IWindow::_createGraphicsDevice() {
        m_graphicsDevice = graphics::IGraphicsDevice::createGraphicsDevice(m_desc.GraphicsDeviceDesc);
    }

    void IWindow::_checkResized(int oldWidth, int oldHeight) {
        // TODO, resize event handling etc. or maybe leave it up to dev
        //if (m_windowWidth != oldWidth || m_windowHeight != oldHeight)
        //    m_graphicsDevice->_onResize();
    }
}