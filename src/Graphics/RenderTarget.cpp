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

#include "Graphics/RenderTarget.hpp"

#include "Console.hpp"

namespace Ngine::Graphics {
    RenderTarget::RenderTarget(GraphicsDevice *graphicsDevice_, unsigned int width_, unsigned int height_)
            : Width(width_), Height(height_) {
        // Get API
        m_API = graphicsDevice_->GetAPI();

        // Create color attachment
        m_texture = new Texture2D(graphicsDevice_, nullptr, width_, height_, UNCOMPRESSED_R8G8B8A8, 1);

        // Verify texture
        if (!m_texture->IsValid()) {
            Console::Error("RenderTarget", "Failed to create color attachment for render target.");

            // Delete texture
            delete m_texture;
            m_texture = nullptr;
            return;
        }

        // Create render target
        if (!m_API->CreateRenderTarget(this)) {
            delete m_texture;
            m_texture = nullptr;
        }
    }

    RenderTarget::~RenderTarget() {
        // Delete self
        Unload();
    }

    Texture2D *RenderTarget::GetTexture() {
        return m_texture;
    }

    bool RenderTarget::IsValid() const {
        return m_API->IsRenderTargetValid(this);
    }

    void RenderTarget::Unload() {
        // Delete texture
        delete m_texture;
        m_texture = nullptr;

        // Delete on GPU
        m_API->DeleteRenderTarget(this);
    }

    bool RenderTarget::operator==(const RenderTarget &b) const {
        return m_API->CompareRenderTargets(this, &b);
    }

    bool RenderTarget::operator!=(const RenderTarget &b) const {
        return !m_API->CompareRenderTargets(this, &b);
    }
}
