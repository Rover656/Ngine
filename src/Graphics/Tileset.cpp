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

#include "Rectangle.h"
#include "Renderer.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    TTileset::TTileset(std::shared_ptr<TTexture2D> texture_, float tileWidth_, float tileHeight_)
            : _Texture(texture_), _TileWidth(tileWidth_), _TileHeight(tileHeight_) {}

    // Public Methods

    void TTileset::DrawTile(TVector2 position_, int tile_) {
        // Tile's start from 1 to allow 0 to mean nothing
        tile_ -= 1;

        // Skip if negative
        if (tile_ < 0) return;

        // Get coords
        auto x = 0.0f;
        auto y = 0.0f;
        for (auto i = 0; i < tile_; i++) {
            x += _TileWidth;
            if (x >= _Texture->Width) {
                x = 0;
                y += _TileHeight;
            }
        }

        // Get source rectangle
        TRectangle sourceRectangle = {x, y, _TileWidth, _TileHeight};

        // Draw
        Drawing::DrawTexture(_Texture, sourceRectangle, position_, TColor::White);
    }

    float TTileset::GetTileHeight() const {
        return _TileHeight;
    }

    float TTileset::GetTileWidth() const {
        return _TileWidth;
    }
}