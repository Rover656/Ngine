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

#include "graphics/Tileset.hpp"

#include "graphics/Renderer.hpp"

namespace ngine::graphics {
    // Public Constructor(s)

    Tileset::Tileset(Texture2D *texture, float tileWidth, float tileHeight)
            : Texture(texture), TileWidth(tileWidth), TileHeight(tileHeight) {}

    // Public Methods

    void Tileset::drawTile(graphics::Renderer *renderer, Vector2 position, int tile, float scale, float rotation, Vector2 origin) {
        // Tile's start from 1 to allow 0 to mean nothing
        tile -= 1;

        // Skip if negative
        if (tile < 0) return;

        // Get coords
        auto x = 0.0f;
        auto y = 0.0f;
        for (auto i = 0; i < tile; i++) {
            x += TileWidth;
            if (x >= (float) Texture->Width) {
                x = 0;
                y += TileHeight;
            }
        }

        // Draw
        Texture->draw(
                renderer,
                {
                        position,
                        TileWidth * scale,
                        TileHeight * scale
                },
                {
                        x,
                        y,
                        TileWidth,
                        TileHeight
                },
                Color::White,
                origin,
                rotation);
    }
}