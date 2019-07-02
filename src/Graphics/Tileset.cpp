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

#include <cmath>

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    Tileset::Tileset(const TSprite& tilesetSprite_, float width_, float height_)
            : _TilesetSprite(tilesetSprite_), Canvas(width_ * tilesetSprite_.FrameWidth, height_ * tilesetSprite_.FrameHeight), _Tiles(width_ * height_) {
        _TilesetSprite.ImageSpeed = 0;

        ReDraw();
    }

    Tileset::Tileset(const TSprite& tilesetSprite_, float width_, float height_, std::vector<int> tiles_)
            : _TilesetSprite(tilesetSprite_), Canvas(width_ * tilesetSprite_.FrameWidth, height_ * tilesetSprite_.FrameHeight) {
        if (tiles_.size() != width_ * height_) {
            throw std::runtime_error("Tile data does not match dimensions.");
        }
        _Tiles = tiles_;

        _TilesetSprite.ImageSpeed = 0;

        ReDraw();
    }

    // Public Methods

    int Tileset::GetTileAt(Math::TVector2 pos_) {
        auto w = GetWidth() / _TilesetSprite.FrameWidth;
        auto h = GetHeight() / _TilesetSprite.FrameHeight;

        auto i = static_cast<int>(pos_.X) + w * static_cast<int>(pos_.Y);

        return _Tiles[i];
    }

    void Tileset::SetTileAt(Math::TVector2 pos_, int tile_) {
        auto w = GetWidth() / _TilesetSprite.FrameWidth;
        auto h = GetHeight() / _TilesetSprite.FrameHeight;

        auto i = static_cast<int>(pos_.X) + w * static_cast<int>(pos_.Y);

        _Tiles[i] = tile_;

        ReDraw(); // TODO: Batch set tiles to avoid this messy call
    }

    void Tileset::SetTileData(std::vector<int> data_) {
        if (data_.size() != GetWidth() * GetHeight()) {
            throw std::runtime_error("Tile data does not match dimensions.");
        }
        _Tiles = data_;
        ReDraw();
    }

    // Protected Method(s)

    void Tileset::RenderTargetRedraw() {
        auto w = GetWidth() / _TilesetSprite.FrameWidth;
        auto h = GetHeight() / _TilesetSprite.FrameHeight;

        for (auto i = 0; i < w * h; i++) {
            _TilesetSprite.CurrentFrame = _Tiles[i];
            Math::TVector2 pos = {static_cast<float>(fmod(i, w)) * _TilesetSprite.FrameWidth, static_cast<float>(i / static_cast<int>(w)) * _TilesetSprite.FrameHeight};
            _TilesetSprite.Draw(pos, 0);
        }
    }
}