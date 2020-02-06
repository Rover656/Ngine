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

    Tileset::Tileset(Texture2D *texture_, float tileWidth_, float tileHeight_)
            : Texture(texture_), TileWidth(tileWidth_), TileHeight(tileHeight_) {}

    // Public Methods

    void Tileset::drawTile(graphics::Renderer *renderer_, Vector2 position_, int tile_, float scale_, float rotation_, Vector2 origin_) {
        // Tile's start from 1 to allow 0 to mean nothing
        tile_ -= 1;

        // Skip if negative
        if (tile_ < 0) return;

        // Get coords
        auto x = 0.0f;
        auto y = 0.0f;
        for (auto i = 0; i < tile_; i++) {
            x += TileWidth;
            if (x >= (float) Texture->Width) {
                x = 0;
                y += TileHeight;
            }
        }

        // Draw
        Texture->draw(
                renderer_,
                {
                        position_,
                        TileWidth * scale_,
                        TileHeight * scale_
                },
                {
                        x,
                        y,
                        TileWidth,
                        TileHeight
                },
                Color::White,
                origin_,
                rotation_);
    }
}