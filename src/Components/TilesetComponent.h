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

#include "../Graphics/TilesetCanvas.h"
#include "../BaseEntity.h"
#include "../Component.h"

namespace NerdThings::Ngine::Components {
    class TilesetComponent : public Component {
        // Private Fields

        /*
         * The tileset
         */
        Graphics::TilesetCanvas *_Tileset;
    public:

        // Public Constructor(s)

        TilesetComponent(BaseEntity *parent_, Graphics::TilesetCanvas *tileset_)
         : Component(parent_), _Tileset(tileset_) {
            SubscribeToDraw();
        }

        // Destructor

        virtual ~TilesetComponent() {
            delete _Tileset;
        }

        // Public Methods

        void Draw(EventArgs &e) override {
            auto par = GetParent<BaseEntity>();
            _Tileset->Draw(par->GetPosition());
        }

        Graphics::TilesetCanvas *GetTileset() {
            return _Tileset;
        }
    };
}

#endif // TILESETCOMPONENT_H
