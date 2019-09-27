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

#include "Tileset.h"

#include "../Rectangle.h"
#include "Renderer.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    Tileset::Tileset(Texture2D *texture_, float tileWidth_, float tileHeight_)
            : _Texture(std::shared_ptr<Texture2D>(texture_)), _TileWidth(tileWidth_), _TileHeight(tileHeight_) {}

    // Public Methods

    void Tileset::DrawTile(Vector2 position_, int tile_) {
        // Tile's start from 1 to allow 0 to mean nothing
        tile_ -= 1;

        // Skip if negative
        if (tile_ < 0) return;

        // Get coords
        auto x = 0.0f;
        auto y = 0.0f;
        for (auto i = 0; i < tile_; i++) {
            x += _TileWidth;
            if (x >= (float)_Texture->Width) {
                x = 0;
                y += _TileHeight;
            }
        }

        // Get source rectangle
        Rectangle sourceRectangle = {x, y, _TileWidth, _TileHeight};

        // Draw
        Renderer::DrawTexture(_Texture.get(), sourceRectangle, position_, Color::White);
    }

    Texture2D *Tileset::GetTexture() {
        return _Texture.get();
    }

    float Tileset::GetTileHeight() const {
        return _TileHeight;
    }

    float Tileset::GetTileWidth() const {
        return _TileWidth;
    }
}