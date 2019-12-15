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

#include "Tileset.hpp"

#include "../Rectangle.hpp"
#include "Renderer.hpp"

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    Tileset::Tileset(Texture2D *texture_, float tileWidth_, float tileHeight_)
            : Texture(texture_), TileWidth(tileWidth_), TileHeight(tileHeight_) {}

    // Public Methods

    void Tileset::DrawTile(Vector2 position_, int tile_, float scale_, float rotation_, Vector2 origin_) {
        // Tile's start from 1 to allow 0 to mean nothing
        tile_ -= 1;

        // Skip if negative
        if (tile_ < 0) return;

        // Get coords
        auto x = 0.0f;
        auto y = 0.0f;
        for (auto i = 0; i < tile_; i++) {
            x += TileWidth;
            if (x >= (float)Texture->Width) {
                x = 0;
                y += TileHeight;
            }
        }

        // Get source rectangle
        Rectangle sourceRectangle = {x, y, TileWidth, TileHeight};

        // Draw
#ifndef USE_EXPERIMENTAL_RENDERER
        Renderer::DrawTexture(Texture, sourceRectangle, position_, TileWidth * scale_, TileHeight * scale_, Color::White, origin_, rotation_);
#endif
    }
}