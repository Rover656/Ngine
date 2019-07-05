/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*   
*   File reviewed on 01/06/2019 by R.M
*
**********************************************************************************************/

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "../ngine.h"

#include "../Core/Component.h"
#include "../Core/BaseEntity.h"
#include "../Core/EventHandler.h"
#include "../Graphics/Sprite.h"

namespace NerdThings::Ngine::Components {
    /*
     * Sprite component
     */
    class SpriteComponent : public Core::Component {
        // Private Fields

        /*
         * Sprite used
         */
        Graphics::TSprite _Sprite;
    public:

        // Public Constructor(s)

        /*
         * Create a sprite component
         */
        SpriteComponent(Core::BaseEntity *parent_, const Graphics::TSprite &sprite_)
            : Component(parent_), _Sprite(sprite_) {
            SubscribeToDraw();
            SubscribeToUpdate();
        }

        // Public Methods

        void Draw(Core::EventArgs &e) override {
            const auto par = GetParent<Core::BaseEntity>();
            _Sprite.Draw(par->GetPosition(), par->GetRotation(), par->GetOrigin());
        }

        Graphics::TSprite *GetSprite() {
            return &_Sprite;
        }

        void SetSprite(const Graphics::TSprite &sprite_) {
            if (_Sprite != sprite_)
                _Sprite = sprite_;
        }

        void Update(Core::EventArgs &e) override {
            _Sprite.Update();
        }
    };
}

#endif //SPRITECOMPONENT_H
