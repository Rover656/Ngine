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

#include "ngine/components/SpriteComponent.hpp"

namespace ngine::components {
    SpriteComponent::SpriteComponent(graphics::Sprite *sprite)
        : m_sprite(sprite) {}

    graphics::Sprite *SpriteComponent::getSprite() const { return m_sprite; }

    void SpriteComponent::setSprite(graphics::Sprite *sprite) {
        m_sprite = sprite;
    }

    Vector2 SpriteComponent::getOffset() const { return m_offset; }

    void SpriteComponent::setOffset(Vector2 offset) { m_offset = offset; }

    float SpriteComponent::getRotation() const { return m_rotation; }

    void SpriteComponent::setRotation(float rotation) { m_rotation = rotation; }

    Vector2 SpriteComponent::getOrigin() const { return m_origin; }

    void SpriteComponent::setOrigin(Vector2 origin) { m_origin = origin; }

    graphics::Color SpriteComponent::getColor() const { return m_color; }

    void SpriteComponent::setColor(graphics::Color color) { m_color = color; }

    void SpriteComponent::draw(graphics::Renderer *renderer) {
        m_sprite->draw(renderer, m_offset, m_rotation, m_origin);
        Component::draw(renderer);
    }
} // namespace ngine::components