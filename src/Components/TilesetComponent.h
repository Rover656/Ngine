/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef TILESETCOMPONENT_H
#define TILESETCOMPONENT_H

#include "../ngine.h"

#include "../Core/BaseEntity.h"
#include "../Core/Component.h"
#include "../Graphics/Tileset.h"

namespace NerdThings::Ngine::Components {
    class TilesetComponent : public Core::Component {
        // Private Fields

        /*
         * The tileset
         */
        Graphics::Tileset *_Tileset;
    public:

        // Public Constructor(s)

        TilesetComponent(Core::BaseEntity *parent_, Graphics::Tileset *tileset_)
         : Component(parent_), _Tileset(tileset_) {
            SubscribeToDraw();
        }

        // Destructor

        ~TilesetComponent() {
            delete _Tileset;
        }

        // Public Methods

        void Draw(Core::EventArgs &e) override {
            auto par = GetParent<Core::BaseEntity>();
            _Tileset->Draw(par->GetPosition());
        }

        Graphics::Tileset *GetTileset() {
            return _Tileset;
        }
    };
}

#endif // TILESETCOMPONENT_H
