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

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "../Ngine.h"

#include "../Graphics/Sprite.h"
#include "../Component.h"
#include "../Entity.h"
#include "../Events.h"

namespace NerdThings::Ngine::Components {
    /**
     * Sprite component. Attaches a sprite at the position of the entity.
     */
    class SpriteComponent : public Component {
        /**
         * Render origin.
         */
        Vector2 m_origin;

        /**
         * Sprite to render.
         */
        Graphics::Sprite m_sprite;
    public:
        /**
         * Create a sprite component.
         *
         * @param parent_ The parent entity we will attach to.
         * @param sprite_ The sprite to render with.
         * @param origin_ The sprite render origin.
         */
        SpriteComponent(Entity *parent_, const Graphics::Sprite &sprite_, Vector2 origin_ = Vector2::Zero);

        /**
         * Get the sprite render origin.
         *
         * @return The rendering origin.
         */
        Vector2 GetOrigin() const;

        /**
         * Set the sprite render origin.
         *
         * @param origin_ The new rendering origin.
         */
        void SetOrigin(const Vector2 &origin_);

        /**
         * Get a pointer to the sprite.
         *
         * @return The current sprite (pointer to it).
         */
        Graphics::Sprite *GetSprite();

        /**
         * Get a pointer to the sprite (read-only).
         *
         * @return A read-only pointer to the current sprite.
         */
        const Graphics::Sprite *GetSprite() const;

        /**
         * Set the sprite.
         *
         * @param sprite_ The new sprite to use.
         */
        void SetSprite(const Graphics::Sprite &sprite_);

        /**
         * Draw the sprite (called by the engine).
         */
        void Draw() override;

        /**
         * Update the sprite (called by the engine).
         */
        void Update() override;
    };
}

#endif //SPRITECOMPONENT_H
