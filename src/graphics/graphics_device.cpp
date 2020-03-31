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

#include "ngine/graphics/graphics_device.hpp"

#include "ngine/console.hpp"

#include "d3d11/d3d11_graphics_device.hpp"

namespace ngine::graphics {
    IGraphicsDevice *IGraphicsDevice::createGraphicsDevice(GraphicsDeviceDesc desc) {
        switch (desc.ContextDescriptor.Type) {
            case graphics::ContextType::OpenGL:
#if defined(NGINE_ENABLE_OPENGL)
                //m_graphicsDevice = new platform::graphics::GLGraphicsDevice(this);
#else
                Console::fail("Window", "Cannot create OpenGL graphics device, OpenGL is not enabled or compatible.");
#endif
                break;
            case graphics::ContextType::OpenGLES:
#if defined(NGINE_ENABLE_OPENGLES)
                //m_graphicsDevice = new platform::graphics::GLGraphicsDevice(this);
#else
                Console::fail("Window", "Cannot create OpenGL ES graphics device, OpenGL ES is not enabled or compatible.");
#endif
                break;
            case graphics::ContextType::DirectX:
#if defined(NGINE_ENABLE_DIRECTX)
                return new d3d11::D3D11GraphicsDevice(desc);
#else
                Console::fail("Window", "Cannot create DirectX graphics device, DirectX is not enabled or compatible.");
#endif
                break;
            case graphics::ContextType::Vulkan:
                Console::fail("Window", "Vulkan is not implemented.");
                break;
            case graphics::ContextType::Metal:
                Console::fail("Window", "Metal is not implemented.");
                break;
        }
        Console::fail("IGraphicsDevice", "Failed to create a graphics device!");
    }
}