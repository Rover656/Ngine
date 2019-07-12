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

#include "TilesetCanvas.h"

#include <cmath>

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    TilesetCanvas::TilesetCanvas(const TTileset& tileset_, float width_, float height_)
            : _Tileset(tileset_), Canvas(width_ * tileset_.GetTileWidth(), height_ * tileset_.GetTileHeight()), _Tiles(width_ * height_) {
        ReDraw();
    }

    TilesetCanvas::TilesetCanvas(const TTileset& tileset_, float width_, float height_, std::vector<int> tiles_)
            : _Tileset(tileset_), Canvas(width_ * tileset_.GetTileWidth(), height_ * tileset_.GetTileHeight()) {
        if (tiles_.size() != width_ * height_) {
            throw std::runtime_error("Tile data does not match dimensions.");
        }
        _Tiles = tiles_;

        ReDraw();
    }

    // Public Methods

    std::vector<Physics::ICollisionShape *> TilesetCanvas::GetCollisionShapesFor(int tile_, Math::TRectangle range_, Math::TVector2 tilesetPosition_) {
        // EXPERIMENTAL, MAY NOT BE WORKING YET NOR BE PERFORMANT
        std::vector<Physics::ICollisionShape *> shapes;

        int sX = range_.X;
        int sY = range_.Y;
        int eX = range_.X + range_.Width;
        int eY = range_.Y + range_.Height;

        for (auto x = sX; x < eX; x++) {
            for (auto y = sY; y < eY; y++) {
                if (GetTileAt({(float)x, (float)y}) == tile_) {
                    // Get polygon with world coordinates
                    auto poly = Math::TRectangle(x * _Tileset.GetTileWidth() + tilesetPosition_.X, y * _Tileset.GetTileWidth() + tilesetPosition_.Y, _Tileset.GetTileWidth(), _Tileset.GetTileHeight()).ToPolygonPtr();
                    shapes.push_back(dynamic_cast<Physics::ICollisionShape *>(poly));
                }
            }
        }

        return shapes;
    }

    int TilesetCanvas::GetTileAt(Math::TVector2 pos_) {
        auto w = GetWidth() / _Tileset.GetTileWidth();
        auto h = GetHeight() / _Tileset.GetTileHeight();

        auto i = static_cast<int>(pos_.X) + w * static_cast<int>(pos_.Y);

        return _Tiles[i];
    }

    TTileset *TilesetCanvas::GetTileset() {
        return &_Tileset;
    }

    void TilesetCanvas::SetTileAt(Math::TVector2 pos_, int tile_) {
        auto w = GetWidth() / _Tileset.GetTileWidth();
        auto h = GetHeight() / _Tileset.GetTileHeight();

        auto i = static_cast<int>(pos_.X) + w * static_cast<int>(pos_.Y);

        _Tiles[i] = tile_;

        ReDraw();
    }

    void TilesetCanvas::SetTileData(std::vector<int> data_) {
        if (data_.size() != GetWidth() / _Tileset.GetTileWidth() * GetHeight() / _Tileset.GetTileHeight()) {
            throw std::runtime_error("Tile data does not match dimensions.");
        }
        _Tiles = data_;
        ReDraw();
    }

    // Protected Method(s)

    void TilesetCanvas::RenderTargetRedraw() {
        auto w = GetWidth() / _Tileset.GetTileWidth();
        auto h = GetHeight() / _Tileset.GetTileHeight();

        for (auto i = 0; i < w * h; i++) {
            Math::TVector2 pos = {static_cast<float>(fmod(i, w)) * _Tileset.GetTileWidth(), static_cast<float>(i / static_cast<int>(w)) * _Tileset.GetTileHeight()};
            _Tileset.DrawTile(pos, _Tiles[i]);
        }
    }
}