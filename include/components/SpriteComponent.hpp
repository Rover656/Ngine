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
#include "../Entity.hpp"
#include "../Events.hpp"

namespace ngine::components {
    /**
     * Sprite component. Attaches a sprite at the position of the entity.
     */
    class NEAPI SpriteComponent : public Component {
        /**
         * Render origin.
         */
        Vector2 m_origin;

        /**
         * Sprite to render.
         */
        graphics::Sprite m_sprite;
    public:
        /**
         * Create a sprite component.
         *
         * @param parent_ The parent entity we will attach to.
         * @param sprite_ The sprite to render with.
         * @param origin_ The sprite render origin.
         */
        SpriteComponent(Entity *parent_, const graphics::Sprite &sprite_, Vector2 origin_ = Vector2::Zero);

        /**
         * Get the sprite render origin.
         *
         * @return The rendering origin.
         */
        Vector2 getOrigin() const;

        /**
         * Set the sprite render origin.
         *
         * @param origin_ The new rendering origin.
         */
        void setOrigin(const Vector2 &origin_);

        /**
         * Get a pointer to the sprite.
         *
         * @return The current sprite (pointer to it).
         */
        graphics::Sprite *getSprite();

        /**
         * Get a pointer to the sprite (read-only).
         *
         * @return A read-only pointer to the current sprite.
         */
        const graphics::Sprite *getSprite() const;

        /**
         * Set the sprite.
         *
         * @param sprite_ The new sprite to use.
         */
        void setSprite(const graphics::Sprite &sprite_);

        /**
         * Draw the sprite (called by the engine).
         */
        void draw(graphics::Renderer *renderer_) override;

        /**
         * Update the sprite (called by the engine).
         */
        void update() override;
    };
}

#endif //SPRITECOMPONENT_HPP
