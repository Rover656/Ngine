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
         * The tileset renderer
         */
        Graphics::TilesetRenderer *_Tileset;

        /*
         * Use the cull area instead of the viewport for rendering tiles.
         */
        bool _UseCullArea = false;
    public:

        // Public Constructor(s)

        /*
         * Create a tileset component with a tileset renderer
         */
        TilesetComponent(BaseEntity *parent_, Graphics::TilesetRenderer *tileset_);

        // Destructor

        virtual ~TilesetComponent();

        // Public Methods

        /*
         * Draw the tileset (called by engine).
         */
        void Draw() override;

        /*
         * Get the tileset renderer
         */
        Graphics::TilesetRenderer *GetTileset();
    };
}

#endif // TILESETCOMPONENT_H
