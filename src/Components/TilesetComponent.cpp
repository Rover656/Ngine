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

#include "TilesetComponent.h"

namespace NerdThings::Ngine::Components {
    TilesetComponent::TilesetComponent(BaseEntity *parent_, Graphics::TilesetRenderer *tileset_)
            : Component(parent_), m_tileset(tileset_) {
        // Prepare for drawing the renderer.
        SubscribeToDraw();
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

    void TilesetComponent::Draw() {
        // Prevent any exceptions.
        if (m_tileset != nullptr) return;

        // Get our parent
        auto par = GetParent();

        // TODO: Scale field

        // Render the tileset
        if (m_useCullArea) {
            m_tileset->Draw(par->GetPosition(), GetScene()->GetCullAreaPosition(), GetScene()->GetCullAreaEndPosition());
        } else {
            m_tileset->Draw(par->GetPosition(), GetScene()->GetViewportPosition(), GetScene()->GetViewportEndPosition());
        }
    }
}