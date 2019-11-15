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
#include "../BaseEntity.h"
#include "../EventHandler.h"

namespace NerdThings::Ngine::Components {
    /*
     * Sprite component
     */
    class SpriteComponent : public Component {
        // Private Fields

        /*
         * Render origin
         */
        Vector2 _Origin;

        /*
         * Sprite used
         */
        Graphics::Sprite _Sprite;
    public:

        // Public Constructor(s)

        /*
         * Create a sprite component
         */
        SpriteComponent(BaseEntity *parent_, const Graphics::Sprite &sprite_, Vector2 origin_ = Vector2::Zero);

        // Public Methods

        /*
         * Draw the sprite (called by the engine)
         */
        void Draw() override;

        /*
         * Get the sprite render origin.
         */
        Vector2 GetOrigin() const;

        /*
         * Get a pointer to the sprite.
         */
        Graphics::Sprite *GetSprite();

        /*
         * Set the sprite render origin.
         */
        void SetOrigin(const Vector2 &origin_);

        /*
         * Set the sprite.
         */
        void SetSprite(const Graphics::Sprite &sprite_);

        /*
         * Update the sprite (called by the engine)
         */
        void Update() override;
    };
}

#endif //SPRITECOMPONENT_H
