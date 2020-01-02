/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#include "SpriteComponent.hpp"

namespace NerdThings::Ngine::Components {
    SpriteComponent::SpriteComponent(Entity *parent_, const Graphics::Sprite &sprite_, Vector2 origin_)
            : Component(parent_), m_origin(origin_), m_sprite(sprite_) {
        SubscribeToDraw();
        SubscribeToUpdate();
    }

    Vector2 SpriteComponent::GetOrigin() const {
        return Vector2();
    }

    void SpriteComponent::SetOrigin(const Vector2 &origin_) {
        m_origin = origin_;
    }

    Graphics::Sprite *SpriteComponent::GetSprite() {
        return &m_sprite;
    }

    const Graphics::Sprite *SpriteComponent::GetSprite() const {
        return &m_sprite;
    }

    void SpriteComponent::SetSprite(const Graphics::Sprite &sprite_) {
        if (m_sprite != sprite_)
            m_sprite = sprite_;
    }

    void SpriteComponent::Draw(Graphics::Renderer *renderer_) {
        Component::Draw(renderer_);
        const auto par = GetEntity<Entity > ();
        m_sprite.Draw(renderer_, par->GetPosition(), 1, par->GetRotation().GetDegrees(), m_origin);
    }

    void SpriteComponent::Update() {
        Component::Update();
        m_sprite.Update();
    }
}