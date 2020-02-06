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

namespace ngine::components {
    SpriteComponent::SpriteComponent(Entity *parent_, const graphics::Sprite &sprite_, Vector2 origin_)
            : Component(parent_), m_origin(origin_), m_sprite(sprite_) {
        subscribeToUpdate();
    }

    Vector2 SpriteComponent::getOrigin() const {
        return Vector2();
    }

    void SpriteComponent::setOrigin(const Vector2 &origin_) {
        m_origin = origin_;
    }

    graphics::Sprite *SpriteComponent::getSprite() {
        return &m_sprite;
    }

    const graphics::Sprite *SpriteComponent::getSprite() const {
        return &m_sprite;
    }

    void SpriteComponent::setSprite(const graphics::Sprite &sprite_) {
        if (m_sprite != sprite_)
            m_sprite = sprite_;
    }

    void SpriteComponent::draw(graphics::Renderer *renderer_) {
        Component::draw(renderer_);
        const auto par = getEntity<Entity>();
        m_sprite.draw(renderer_, par->getPosition(), 1, par->getRotation().getDegrees(), m_origin);
    }

    void SpriteComponent::update() {
        Component::update();
        m_sprite.update();
    }
}