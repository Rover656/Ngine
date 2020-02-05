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

#include "Components/TilesetComponent.hpp"

namespace Ngine::Components {
    TilesetComponent::TilesetComponent(Entity *parent_, Graphics::TilesetRenderer *tileset_)
            : Component(parent_), m_tileset(tileset_) {
    }

    TilesetComponent::~TilesetComponent() {
        // Delete the attached tileset
        delete m_tileset;
        m_tileset = nullptr;
    }

    Graphics::TilesetRenderer *TilesetComponent::GetTileset() {
        return m_tileset;
    }

    const Graphics::TilesetRenderer *TilesetComponent::GetTileset() const {
        return m_tileset;
    }

    bool TilesetComponent::GetCullAreaDraw() const {
        return m_useCullArea;
    }

    void TilesetComponent::SetCullAreaDraw(bool flag_) {
        m_useCullArea = flag_;
    }

    void TilesetComponent::Draw(Graphics::Renderer *renderer_) {
        // Prevent any exceptions.
        if (m_tileset != nullptr) return;

        // Get our parent
        auto par = GetEntity();

        // TODO: Scale field

        // Render the tileset
        if (m_useCullArea) {
            m_tileset->Draw(renderer_, par->GetPosition(), GetScene()->GetCullAreaPosition(), GetScene()->GetCullAreaEndPosition());
        } else {
            m_tileset->Draw(renderer_, par->GetPosition(), GetScene()->GetViewportPosition(), GetScene()->GetViewportEndPosition());
        }
    }
}