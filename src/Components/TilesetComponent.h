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

#ifndef TILESETCOMPONENT_H
#define TILESETCOMPONENT_H

#include "../Ngine.h"

#include "../Graphics/TilesetRenderer.h"
#include "../BaseEntity.h"
#include "../Component.h"

namespace NerdThings::Ngine::Components {
    class TilesetComponent : public Component {
        // Private Fields

        /*
         * The tileset
         */
        Graphics::TilesetRenderer *_Tileset;

        /*
         * Use the cull area instead of the viewport for rendering tiles.
         */
        bool _UseCullArea = false;
    public:

        // Public Constructor(s)

        TilesetComponent(BaseEntity *parent_, Graphics::TilesetRenderer *tileset_)
         : Component(parent_), _Tileset(tileset_) {
            SubscribeToDraw();
        }

        // Destructor

        virtual ~TilesetComponent() {
            delete _Tileset;
        }

        // Public Methods

        void Draw(EventArgs &e) override {
            auto par = GetParent();

            if (_UseCullArea) {
                _Tileset->Draw(par->GetPosition(), GetParentScene()->GetCullAreaPosition(), GetParentScene()->GetCullAreaEndPosition(), par->GetScale());
            } else {
                _Tileset->Draw(par->GetPosition(), GetParentScene()->GetViewportPosition(), GetParentScene()->GetViewportEndPosition(), par->GetScale());
            }
        }

        Graphics::TilesetRenderer *GetTileset() {
            return _Tileset;
        }
    };
}

#endif // TILESETCOMPONENT_H
