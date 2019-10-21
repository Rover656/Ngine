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
         * Sprite used
         */
        Graphics::Sprite _Sprite;
    public:

        // Public Constructor(s)

        /*
         * Create a sprite component
         */
        SpriteComponent(BaseEntity *parent_, const Graphics::Sprite &sprite_)
            : Component(parent_), _Sprite(sprite_) {
            SubscribeToDraw();
            SubscribeToUpdate();
        }

        // Public Methods

        void Draw() override {
            const auto par = GetParent<BaseEntity>();
            _Sprite.Draw(par->GetPosition(), par->GetScale(), par->GetRotation(), par->GetOrigin());
        }

        Graphics::Sprite *GetSprite() {
            return &_Sprite;
        }

        void SetSprite(const Graphics::Sprite &sprite_) {
            if (_Sprite != sprite_)
                _Sprite = sprite_;
        }

        void Update() override {
            _Sprite.Update();
        }
    };
}

#endif //SPRITECOMPONENT_H
