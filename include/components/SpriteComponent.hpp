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

#include "../graphics/Sprite.hpp"
#include "../Component.hpp"

namespace ngine::components {
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
         * @param sprite Sprite to render.
         */
        SpriteComponent(graphics::Sprite *sprite);

        graphics::Sprite *getSprite() const;

        void setSprite(graphics::Sprite *sprite);

        Vector2 getOffset() const;

        void setOffset(Vector2 offset);

        float getRotation() const;

        void setRotation(float rotation);

        Vector2 getOrigin() const;

        void setOrigin(Vector2 origin);

        graphics::Color getColor() const;

        void setColor(graphics::Color color);

        void draw(graphics::Renderer *renderer);
    };
}

#endif //SPRITECOMPONENT_HPP
