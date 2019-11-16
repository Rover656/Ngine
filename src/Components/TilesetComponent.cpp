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
            : Component(parent_), _Tileset(tileset_) {
        // Prepare for drawing the renderer.
        SubscribeToDraw();
    }

    TilesetComponent::~TilesetComponent() {
        // Delete the attached tileset
        delete _Tileset;
        _Tileset = nullptr;
    }

    Graphics::TilesetRenderer *TilesetComponent::GetTileset() {
        return _Tileset;
    }

    const Graphics::TilesetRenderer *TilesetComponent::GetTileset() const {
        return _Tileset;
    }

    bool TilesetComponent::GetCullAreaDraw() const {
        return _UseCullArea;
    }

    void TilesetComponent::SetCullAreaDraw(bool flag_) {
        _UseCullArea = flag_;
    }

    void TilesetComponent::Draw() {
        // Prevent any exceptions.
        if (_Tileset != nullptr) return;

        // Get our parent
        auto par = GetParent();

        // TODO: Scale field

        // Render the tileset
        if (_UseCullArea) {
            _Tileset->Draw(par->GetPosition(), GetParentScene()->GetCullAreaPosition(), GetParentScene()->GetCullAreaEndPosition());
        } else {
            _Tileset->Draw(par->GetPosition(), GetParentScene()->GetViewportPosition(), GetParentScene()->GetViewportEndPosition());
        }
    }
}