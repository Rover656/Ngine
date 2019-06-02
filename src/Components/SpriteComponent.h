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

        /*
         * Whether or not to draw using the camera
         */
        bool _UsingCamera = true;
    public:

        // Public Constructor(s)

        /*
         * Create a sprite component
         */
        SpriteComponent(Core::BaseEntity *parent_, const Graphics::TSprite &sprite_, const bool useCamera_ = true)
            : Component(parent_), _Sprite(sprite_), _UsingCamera(useCamera_) {
            if (_UsingCamera)
                SubscribeToCameraDraw();
            else
                SubscribeToDraw();

            SubscribeToUpdate();
        }

        // Public Methods

        void Draw(Core::EventArgs &e) override {
            if (!_UsingCamera) {
                const auto par = GetParent<Core::BaseEntity>();
                _Sprite.Draw(par->GetPosition(), par->GetRotation(), par->GetOrigin());
            }
        }

        void DrawCamera(Core::EventArgs &e) override {
            if (_UsingCamera) {
                const auto par = GetParent<Core::BaseEntity>();
                _Sprite.Draw(par->GetPosition(), par->GetRotation(), par->GetOrigin());
            }
        }

        void Update(Core::EventArgs &e) override {
            _Sprite.Update();
        }
    };
}

#endif //SPRITECOMPONENT_H
