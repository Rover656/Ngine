/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#ifndef TILESETRENDERER_HPP
#define TILESETRENDERER_HPP

#include "../Config.hpp"

#include "../Physics/Polygon.hpp"
#include "../Rectangle.hpp"
#include "Canvas.hpp"
#include "Tileset.hpp"

namespace NerdThings::Ngine::Graphics {
    /**
     * A tileset canvas.
     *
     * @todo Wait for Tileset to be finished before documenting.
     */
    class NEAPI TilesetRenderer {
        /*
         * Render area height in tile units
         */
        int m_height;

        /*
         * The tile data
         */
        std::vector<int> m_tiles;

        /*
         * The tileset
         */
        Tileset m_tileset;

        /*
         * Render area width in tile units
         */
        int m_width;

    public:
        /*
         * Create a tileset.
         * Width and height is in tile space.
         * Tiles indices start from 1, so tile one is index 1.
         */
        TilesetRenderer(const Tileset& tileset_, int width_, int height_);

        /*
         * Create a tileset with data.
         * Width and height is in tile space.
         * Tiles indices start from 1, so tile one is index 1.
         */
        TilesetRenderer(const Tileset& tileset_, int width_, int height_, std::vector<int> tiles_);

        // Public Methods

        /*
         * Draw the tileset.
         * Origin is in tile coordinates.
         */
        void Draw(Graphics::Renderer *renderer_, Vector2 pos_, float scale_ = 1.0f, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);

        /*
         * Draw the tileset. Does not currently support rotation.
         * position defines the top-left corner of the entire tileset.
         * renderFrom defines where to begin rendering. Normally the viewport position.
         * renderTo defines where to finish rendering. Normally the viewport position + dimensions.
         */
        void Draw(Graphics::Renderer *renderer_, Vector2 pos_, Vector2 renderFrom_, Vector2 renderTo_, float scale_ = 1);

        /*
         * Get collision shapes for a tile in a range.
         * All shapes must be deleted afterwards.
         */
        std::vector<Physics::Shape *> GetCollisionShapesFor(int tile_, Rectangle range_, Vector2 tilesetPosition_ = Vector2::Zero);

        /*
         * Get collision shapes for tiles in a range.
         * All shapes must be deleted afterwards.
         */
        std::vector<Physics::Shape *> GetCollisionShapesFor(std::vector<int> tiles_, Rectangle range_, Vector2 tilesetPosition_ = Vector2::Zero);

        /*
         * Get collision shapes for tiles in a range.
         * min_ <= tile <= max_.
         * All shapes must be deleted afterwards.
         */
        std::vector<Physics::Shape *> GetCollisionShapesFor(int min_, int max_, Rectangle range_, Vector2 tilesetPosition_ = Vector2::Zero);

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

#endif //TILESETRENDERER_HPP
