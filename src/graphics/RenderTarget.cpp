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

#include "ngine/graphics/RenderTarget.hpp"

#include "ngine/Console.hpp"

namespace ngine::graphics {
    RenderTarget::RenderTarget(GraphicsDevice *graphicsDevice, unsigned int width, unsigned int height)
            : Width(width), Height(height), m_viewport(0, 0, width, height) {
        // Get API
        m_API = graphicsDevice->getAPI();

        // Create color attachment
        m_texture = new Texture2D(graphicsDevice, nullptr, width, height, UNCOMPRESSED_R8G8B8A8, 1);

        // Verify texture
        if (!m_texture->isValid()) {
            Console::error("RenderTarget", "Failed to create color attachment for render target.");

            // Delete texture
            delete m_texture;
            m_texture = nullptr;
            return;
        }

        // Create render target
        if (!m_API->createRenderTarget(this)) {
            delete m_texture;
            m_texture = nullptr;
        }
    }

    const Viewport *RenderTarget::getViewport() const {
        return &m_viewport;
    }

    RenderTarget::~RenderTarget() {
        // Delete self
        free();
    }

    Texture2D *RenderTarget::getTexture() {
        return m_texture;
    }

    bool RenderTarget::isValid() const {
        return m_API->isRenderTargetValid(this);
    }

    void RenderTarget::free() {
        // Delete texture
        delete m_texture;
        m_texture = nullptr;

        // Delete on GPU
        m_API->deleteRenderTarget(this);
    }

    bool RenderTarget::operator==(const RenderTarget &b) const {
        return m_API->compareRenderTargets(this, &b);
    }

    bool RenderTarget::operator!=(const RenderTarget &b) const {
        return !m_API->compareRenderTargets(this, &b);
    }
}
