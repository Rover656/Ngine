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

#ifndef TILESETRENDERER_H
#define TILESETRENDERER_H

#include "../Ngine.h"

#include "../Physics/Polygon.h"
#include "../Rectangle.h"
#include "Canvas.h"
#include "Tileset.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * A tileset canvas
     */
    class NEAPI TilesetRenderer {
        // Private Fields

        /*
         * Render area height in tile units
         */
        int _Height;

        /*
         * The tile data
         */
        std::vector<int> _Tiles;

        /*
         * The tileset
         */
        Tileset _Tileset;

        /*
         * Render area width in tile units
         */
        int _Width;

    public:
        // Public Constructor(s)

        /*
         * Create a tileset.
         * Width and height is in tile space.
         * Tiles indices start from 1, so tile one is index 1.
         */
        TilesetRenderer(const Tileset& tileset_, float width_, float height_);

        /*
         * Create a tileset with data.
         * Width and height is in tile space.
         * Tiles indices start from 1, so tile one is index 1.
         */
        TilesetRenderer(const Tileset& tileset_, float width_, float height_, std::vector<int> tiles_);

        // Public Methods

        /*
         * Draw the tileset.
         * Origin is in tile coordinates.
         */
        void Draw(Vector2 pos_, float scale_ = 1.0f, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw the tileset. Does not currently support rotation.
         * position defines the top-left corner of the entire tileset.
         * renderFrom defines where to begin rendering. Normally the viewport position.
         * renderTo defines where to finish rendering. Normally the viewport position + dimensions.
         */
        void Draw(Vector2 pos_, Vector2 renderFrom_, Vector2 renderTo_, float scale_ = 1);

        /*
         * Get collision shapes for a tile in a range.
         * All shapes must be deleted afterwards.
         */
        std::vector<Physics::ICollisionShape *> GetCollisionShapesFor(int tile_, Rectangle range_, Vector2 tilesetPosition_ = Vector2::Zero);

        /*
         * Get collision shapes for tiles in a range.
         * All shapes must be deleted afterwards.
         */
        std::vector<Physics::ICollisionShape *> GetCollisionShapesFor(std::vector<int> tiles_, Rectangle range_, Vector2 tilesetPosition_ = Vector2::Zero);

        /*
         * Get collision shapes for tiles in a range.
         * min_ <= tile <= max_.
         * All shapes must be deleted afterwards.
         */
        std::vector<Physics::ICollisionShape *> GetCollisionShapesFor(int min_, int max_, Rectangle range_, Vector2 tilesetPosition_ = Vector2::Zero);

        /*
         * Get render area height in tile units
         */
        int GetHeight();

        /*
         * Get the tile value at the position (0,0 is first tile, 1,0 is second tile etc.).
         */
        int GetTileAt(Vector2 pos_);

        /*
         * Get the tileset being used
         */
        Tileset *GetTileset();

        /*
         * Get render area width in tile units
         */
        int GetWidth();

        /*
         * Set the tile value at a position.
         */
        void SetTileAt(Vector2 pos_, int tile_);

        /*
         * Set all tile data
         */
        void SetTileData(std::vector<int> data_);
    };
}

#endif //TILESETRENDERER_H
