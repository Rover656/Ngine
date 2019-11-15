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

#include "SpriteComponent.h"

namespace NerdThings::Ngine::Components {
    // Public Constructor

    SpriteComponent::SpriteComponent(BaseEntity *parent_, const Graphics::Sprite &sprite_, Vector2 origin_)
            : Component(parent_), _Origin(origin_), _Sprite(sprite_) {
        SubscribeToDraw();
        SubscribeToUpdate();
    }

    // Public Methods

    void SpriteComponent::Draw() {
        Component::Draw();
        const auto par = GetParent<BaseEntity>();
        _Sprite.Draw(par->GetPosition(), 1, par->GetRotation(), _Origin);
    }

    Vector2 SpriteComponent::GetOrigin() const {
        return Vector2();
    }

    Graphics::Sprite *SpriteComponent::GetSprite() {
        return &_Sprite;
    }

    void SpriteComponent::SetOrigin(const Vector2 &origin_) {
        _Origin = origin_;
    }

    void SpriteComponent::SetSprite(const Graphics::Sprite &sprite_) {
        if (_Sprite != sprite_)
            _Sprite = sprite_;
    }

    void SpriteComponent::Update() {
        Component::Update();
        _Sprite.Update();
    }
}