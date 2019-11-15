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
    // Public Constructor

    TilesetComponent::TilesetComponent(BaseEntity *parent_, Graphics::TilesetRenderer *tileset_)
            : Component(parent_), _Tileset(tileset_) {
        SubscribeToDraw();
    }

    // Destructor

    TilesetComponent::~TilesetComponent() {
        delete _Tileset;
    }

    // Public Methods

    void TilesetComponent::Draw() {
        auto par = GetParent();

        // TODO: Scale field

        if (_UseCullArea) {
            _Tileset->Draw(par->GetPosition(), GetParentScene()->GetCullAreaPosition(), GetParentScene()->GetCullAreaEndPosition());
        } else {
            _Tileset->Draw(par->GetPosition(), GetParentScene()->GetViewportPosition(), GetParentScene()->GetViewportEndPosition());
        }
    }

    Graphics::TilesetRenderer *TilesetComponent::GetTileset() {
        return _Tileset;
    }
}