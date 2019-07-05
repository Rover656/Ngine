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

#ifndef TILESET_H
#define TILESET_H

#include "../ngine.h"

#include "Canvas.h"
#include "Sprite.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * A tileset canvas
     */
    class NEAPI Tileset : public Canvas {
        // Private Fields

        /*
         * The tile data
         */
        std::vector<int> _Tiles;

        /*
         * The tileset sprite and data
         */
        TSprite _TilesetSprite;

    public:
        // Public Constructor(s)

        /*
         * Create a tileset.
         * Width and height is in tile space multiplied by tilesetSprite frame width and height.
         * Tiles indices start from 1, so tile one is index 1.
         */
        Tileset(const TSprite& tilesetSprite_, float width_, float height_);

        /*
         * Create a tileset with data.
         * Width and height is in tile space multiplied by tilesetSprite frame width and height
         * Tiles indices start from 1, so tile one is index 1.
         */
        Tileset(const TSprite& tilesetSprite_, float width_, float height_, std::vector<int> tiles_);

        // Public Methods

        /*
         * Get the tile value at the position (0,0 is first tile, 1,0 is second tile etc.).
         */
        int GetTileAt(Math::TVector2 pos_);

        /*
         * Set the tile value at a position.
         */
        void SetTileAt(Math::TVector2 pos_, int tile_);

        /*
         * Set all tile data
         */
        void SetTileData(std::vector<int> data_);
    protected:

        // Protected Methods

        void RenderTargetRedraw() override;
    };
}

#endif // TILESET_H
