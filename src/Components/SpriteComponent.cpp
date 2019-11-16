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
    SpriteComponent::SpriteComponent(BaseEntity *parent_, const Graphics::Sprite &sprite_, Vector2 origin_)
            : Component(parent_), _Origin(origin_), _Sprite(sprite_) {
        SubscribeToDraw();
        SubscribeToUpdate();
    }

    Vector2 SpriteComponent::GetOrigin() const {
        return Vector2();
    }

    void SpriteComponent::SetOrigin(const Vector2 &origin_) {
        _Origin = origin_;
    }

    Graphics::Sprite *SpriteComponent::GetSprite() {
        return &_Sprite;
    }

    const Graphics::Sprite *SpriteComponent::GetSprite() const {
        return &_Sprite;
    }

    void SpriteComponent::SetSprite(const Graphics::Sprite &sprite_) {
        if (_Sprite != sprite_)
            _Sprite = sprite_;
    }

    void SpriteComponent::Draw() {
        Component::Draw();
        const auto par = GetParent<BaseEntity>();
        _Sprite.Draw(par->GetPosition(), 1, par->GetRotation(), _Origin);
    }

    void SpriteComponent::Update() {
        Component::Update();
        _Sprite.Update();
    }
}