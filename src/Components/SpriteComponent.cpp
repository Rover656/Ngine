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

#include "Components/SpriteComponent.hpp"

namespace Ngine::Components {
    SpriteComponent::SpriteComponent(Entity *parent_, const Graphics::Sprite &sprite_, Vector2 origin_)
            : Component(parent_), m_origin(origin_), m_sprite(sprite_) {
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