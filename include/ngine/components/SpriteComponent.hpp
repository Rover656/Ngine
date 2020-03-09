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

#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "../Config.hpp"

#include "../Component.hpp"
#include "../graphics/Sprite.hpp"

namespace ngine::components {
    /**
     * The sprite component displays a single sprite on an `Entity`.
     */
    class NEAPI SpriteComponent : public Component {
        /**
         * The sprite to render with.
         */
        graphics::Sprite *m_sprite = nullptr;

        /**
         * Position offset from entity.
         */
        Vector2 m_offset = Vector2::Zero;

        /**
         * Sprite rotation.
         */
        float m_rotation = 0;

        /**
         * Sprite origin
         */
        Vector2 m_origin = {0.5f, 0.5f};

        /**
         * Color modifier.
         */
        graphics::Color m_color = graphics::Color::White;

    public:
        /**
         * Create a new sprite component.
         *
         * @warning Lifecycle of the sprite will be managed by this component.
         * @param sprite Sprite to render.
         */
        SpriteComponent(graphics::Sprite *sprite);

        /**
         * Get the sprite rendered by the component
         *
         * @return The sprite.
         */
        graphics::Sprite *getSprite() const;

        /**
         * Set the sprite to be rendered.
         *
         * @warning Lifecycle of the sprite will be managed by this component.
         * This deletes the current sprite.
         * @param sprite Sprite to be rendered.
         */
        void setSprite(graphics::Sprite *sprite);

        /**
         * Get the positional offset from the entity applied.
         *
         * @return The positional offset.
         */
        Vector2 getOffset() const;

        /**
         * Set the positional offset from the entity.
         *
         * @param offset New offset.
         */
        void setOffset(Vector2 offset);

        /**
         * Get the rotation applied to the sprite.
         *
         * @return The rotation applied.
         */
        float getRotation() const;

        /**
         * Set the rotation for the sprite.
         *
         * @param rotation New rotation.
         */
        void setRotation(float rotation);

        /**
         * Get the sprite origin.
         *
         * @return Sprite origin.
         */
        Vector2 getOrigin() const;

        /**
         * Set the sprite origin.
         *
         * @param origin New origin.
         */
        void setOrigin(Vector2 origin);

        /**
         * Get the color modifier.
         *
         * @return Color modifier.
         */
        graphics::Color getColor() const;

        /**
         * Set the color modifier
         *
         * @param color New color modifier.
         */
        void setColor(graphics::Color color);

        /**
         * Component draw event.
         *
         * @param renderer Renderer used to draw.
         */
        void draw(graphics::Renderer *renderer);
    };
} // namespace ngine::components

#endif // SPRITECOMPONENT_HPP
