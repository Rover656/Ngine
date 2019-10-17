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

#include "TilesetRenderer.h"

#include <cmath>

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    TilesetRenderer::TilesetRenderer(const Tileset &tileset_, float width_, float height_)
            : _Tileset(tileset_), _Width(width_), _Height(height_),
              _Tiles(width_ * height_) {
    }

    TilesetRenderer::TilesetRenderer(const Tileset &tileset_, float width_, float height_, std::vector<int> tiles_)
            : _Tileset(tileset_), _Width(width_), _Height(height_) {
        if (tiles_.size() != width_ * height_) {
            throw std::runtime_error("Tile data does not match dimensions.");
        }
        _Tiles = tiles_;
    }

    // Public Methods

    void TilesetRenderer::Draw(Vector2 pos_) {
        auto tileWidth = _Tileset.GetTileWidth();
        auto tileHeight = _Tileset.GetTileHeight();

        for (auto x = 0; x < _Width; x++) {
            for (auto y = 0; y < _Height; y++) {
                Vector2 pos = {pos_.X + x * tileWidth, pos_.Y + y * tileHeight};
                _Tileset.DrawTile(pos, GetTileAt({(float)x, (float)y}));
            }
        }
    }

    void TilesetRenderer::Draw(Vector2 pos_, Vector2 renderFrom_, Vector2 renderTo_) {
        auto tileWidth = _Tileset.GetTileWidth();
        auto tileHeight = _Tileset.GetTileHeight();

        // Correct coordinates
        if (renderFrom_.X < pos_.X) renderFrom_.X = pos_.X;
        if (renderFrom_.Y < pos_.Y) renderFrom_.Y = pos_.Y;
        if (renderFrom_.X > pos_.X + _Width * tileWidth) renderFrom_.X = pos_.X  + _Width * tileWidth;
        if (renderFrom_.Y > pos_.Y + _Height * tileHeight) renderFrom_.Y = pos_.Y + _Height * tileHeight;

        if (renderTo_.X < pos_.X) renderTo_.X = pos_.X;
        if (renderTo_.Y < pos_.Y) renderTo_.Y = pos_.Y;
        if (renderTo_.X > pos_.X + _Width * tileWidth) renderTo_.X = pos_.X + _Width * tileWidth;
        if (renderTo_.Y > pos_.Y + _Height * tileHeight) renderTo_.Y = pos_.Y + _Height * tileHeight;

        // Get coordinates
        auto tXFrom = (int)floorf((renderFrom_.X - pos_.X) / tileWidth);
        auto tYFrom = (int)floorf((renderFrom_.Y - pos_.Y) / tileHeight);
        auto tXTo = (int)ceilf((renderTo_.X - pos_.X) / tileWidth);
        auto tYTo = (int)ceilf((renderTo_.Y - pos_.Y) / tileHeight);

        for (auto x = tXFrom; x < tXTo; x++) {
            for (auto y = tYFrom; y < tYTo; y++) {
                Vector2 pos = {pos_.X + x * tileWidth, pos_.Y + y * tileHeight};
                _Tileset.DrawTile(pos, GetTileAt({(float)x, (float)y}));
            }
        }
    }

    std::vector<Physics::ICollisionShape *>
    TilesetRenderer::GetCollisionShapesFor(int tile_, Rectangle range_, Vector2 tilesetPosition_) {
        std::vector<Physics::ICollisionShape *> shapes;

        int sX = range_.X;
        int sY = range_.Y;
        int eX = range_.X + range_.Width;
        int eY = range_.Y + range_.Height;

        // Validate
        auto w = GetWidth();
        auto h = GetHeight();

        if (sX < 0) sX = 0;
        if (sY < 0) sY = 0;
        if (sX >= w) sX = w - 1;
        if (sY >= h) sX = h - 1;

        if (eX < 0) eX = 0;
        if (eY < 0) eY = 0;
        if (eX >= w) eX = w - 1;
        if (eY >= h) eY = h - 1;

        for (auto x = sX; x <= eX; x++) {
            for (auto y = sY; y <= eY; y++) {
                if (GetTileAt({(float) x, (float) y}) == tile_) {
                    // Get polygon with world coordinates
                    auto poly = Rectangle(x * _Tileset.GetTileWidth() + tilesetPosition_.X,
                                                 y * _Tileset.GetTileWidth() + tilesetPosition_.Y,
                                          _Tileset.GetTileWidth(), _Tileset.GetTileHeight()).ToPolygonPtr();
                    shapes.push_back(dynamic_cast<Physics::ICollisionShape *>(poly));
                }
            }
        }

        return shapes;
    }

    std::vector<Physics::ICollisionShape *>
    TilesetRenderer::GetCollisionShapesFor(std::vector<int> tiles_, Rectangle range_,
                                           Vector2 tilesetPosition_) {
        std::vector<Physics::ICollisionShape *> shapes;

        int sX = range_.X;
        int sY = range_.Y;
        int eX = range_.X + range_.Width;
        int eY = range_.Y + range_.Height;

        // Validate
        auto w = GetWidth() / _Tileset.GetTileWidth();
        auto h = GetHeight() / _Tileset.GetTileHeight();

        if (sX < 0) sX = 0;
        if (sY < 0) sY = 0;
        if (sX >= w) sX = w - 1;
        if (sY >= h) sX = h - 1;

        if (eX < 0) eX = 0;
        if (eY < 0) eY = 0;
        if (eX >= w) eX = w - 1;
        if (eY >= h) eY = h - 1;

        for (auto x = sX; x <= eX; x++) {
            for (auto y = sY; y <= eY; y++) {
                if (std::find(tiles_.begin(), tiles_.end(), GetTileAt({(float) x, (float) y})) != tiles_.end()) {
                    // Get polygon with world coordinates
                    auto poly = Rectangle(x * _Tileset.GetTileWidth() + tilesetPosition_.X,
                                                 y * _Tileset.GetTileWidth() + tilesetPosition_.Y,
                                          _Tileset.GetTileWidth(), _Tileset.GetTileHeight()).ToPolygonPtr();
                    shapes.push_back(dynamic_cast<Physics::ICollisionShape *>(poly));
                }
            }
        }

        return shapes;
    }

    std::vector<Physics::ICollisionShape *>
    TilesetRenderer::GetCollisionShapesFor(int min_, int max_, Rectangle range_, Vector2 tilesetPosition_) {
        std::vector<Physics::ICollisionShape *> shapes;

        int sX = range_.X;
        int sY = range_.Y;
        int eX = range_.X + range_.Width;
        int eY = range_.Y + range_.Height;

        // Validate
        auto w = GetWidth() / _Tileset.GetTileWidth();
        auto h = GetHeight() / _Tileset.GetTileHeight();

        if (sX < 0) sX = 0;
        if (sY < 0) sY = 0;
        if (sX >= w) sX = w - 1;
        if (sY >= h) sX = h - 1;

        if (eX < 0) eX = 0;
        if (eY < 0) eY = 0;
        if (eX >= w) eX = w - 1;
        if (eY >= h) eY = h - 1;

        for (auto x = sX; x <= eX; x++) {
            for (auto y = sY; y <= eY; y++) {
                auto t = GetTileAt({(float) x, (float) y});
                if (t >= min_ && t <= max_) {
                    // Get polygon with world coordinates
                    auto poly = Rectangle(x * _Tileset.GetTileWidth() + tilesetPosition_.X,
                                                 y * _Tileset.GetTileWidth() + tilesetPosition_.Y,
                                          _Tileset.GetTileWidth(), _Tileset.GetTileHeight()).ToPolygonPtr();
                    shapes.push_back(dynamic_cast<Physics::ICollisionShape *>(poly));
                }
            }
        }

        return shapes;
    }

    int TilesetRenderer::GetHeight() {
        return _Height;
    }

    int TilesetRenderer::GetTileAt(Vector2 pos_) {
        auto w = GetWidth();
        auto h = GetHeight();

        auto i = static_cast<int>(pos_.X) + w * static_cast<int>(pos_.Y);

        return _Tiles[i];
    }

    Tileset *TilesetRenderer::GetTileset() {
        return &_Tileset;
    }

    int TilesetRenderer::GetWidth() {
        return _Width;
    }

    void TilesetRenderer::SetTileAt(Vector2 pos_, int tile_) {
        auto w = GetWidth();
        auto h = GetHeight();

        auto i = static_cast<int>(pos_.X) + w * static_cast<int>(pos_.Y);

        _Tiles[i] = tile_;
    }

    void TilesetRenderer::SetTileData(std::vector<int> data_) {
        if (data_.size() != GetWidth() * GetHeight()) {
            throw std::runtime_error("Tile data does not match dimensions.");
        }
        _Tiles = data_;
    }
}