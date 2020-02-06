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

#include "graphics/TilesetRenderer.hpp"

#include <cmath>

namespace ngine::graphics {
    // Public Constructor(s)

    TilesetRenderer::TilesetRenderer(const Tileset &tileset_, int width_, int height_)
            : m_tileset(tileset_), m_width(width_), m_height(height_),
              m_tiles(width_ * height_) {
    }

    TilesetRenderer::TilesetRenderer(const Tileset &tileset_, int width_, int height_, std::vector<int> tiles_)
            : m_tileset(tileset_), m_width(width_), m_height(height_) {
        if (tiles_.size() != width_ * height_) {
            throw std::runtime_error("Tile data does not match dimensions.");
        }
        m_tiles = tiles_;
    }

    // Public Methods

    void TilesetRenderer::draw(graphics::Renderer *renderer_, Vector2 pos_, float scale_, float rotation_, Vector2 origin_) {
        auto tileWidth = m_tileset.TileWidth;
        auto tileHeight = m_tileset.TileHeight;

        for (auto x = 0; x < m_width; x++) {
            for (auto y = 0; y < m_height; y++) {
                // Get tile position
                Vector2 pos = {(pos_.X + x * tileWidth) * scale_, (pos_.Y + y * tileHeight) * scale_};

                // Don't know why this needs doubled, but sure
                auto tilePos = pos_ - Vector2(-origin_.X * tileWidth * 2, -origin_.Y * tileHeight * 2);
                Vector2 tileOrigin = {-pos.X, -pos.Y};

                // Cheaty: Putting the wrong values in each argument for this to work
                m_tileset.drawTile(renderer_, tilePos, getTileAt({(float) x, (float) y}), scale_, rotation_, tileOrigin);
            }
        }
    }

    void TilesetRenderer::draw(graphics::Renderer *renderer_, Vector2 pos_, Vector2 renderFrom_, Vector2 renderTo_, float scale_) {
        // TODO: How do we rotate this stuff??

        // Get tile sizes
        auto tileWidth = m_tileset.TileWidth;
        auto tileHeight = m_tileset.TileHeight;

        // Tileset size
        auto tilesetWidth = m_width * scale_;
        auto tilesetHeight = m_height * scale_;

        // Correct coordinates
        if (renderFrom_.X < pos_.X) renderFrom_.X = pos_.X;
        if (renderFrom_.Y < pos_.Y) renderFrom_.Y = pos_.Y;
        if (renderFrom_.X > pos_.X + tilesetWidth * tileWidth) renderFrom_.X = pos_.X  + tilesetWidth * tileWidth;
        if (renderFrom_.Y > pos_.Y + tilesetHeight * tileHeight) renderFrom_.Y = pos_.Y + tilesetHeight * tileHeight;

        if (renderTo_.X < pos_.X) renderTo_.X = pos_.X;
        if (renderTo_.Y < pos_.Y) renderTo_.Y = pos_.Y;
        if (renderTo_.X > pos_.X + tilesetWidth * tileWidth) renderTo_.X = pos_.X + tilesetWidth * tileWidth;
        if (renderTo_.Y > pos_.Y + tilesetHeight * tileHeight) renderTo_.Y = pos_.Y + tilesetHeight * tileHeight;

        // Get coordinates
        auto tXFrom = (int)floorf((renderFrom_.X - pos_.X) / tileWidth / scale_);
        auto tYFrom = (int)floorf((renderFrom_.Y - pos_.Y) / tileHeight / scale_);
        auto tXTo = (int)ceilf((renderTo_.X - pos_.X) / tileWidth / scale_);
        auto tYTo = (int)ceilf((renderTo_.Y - pos_.Y) / tileHeight / scale_);

        for (auto x = tXFrom; x < tXTo; x++) {
            for (auto y = tYFrom; y < tYTo; y++) {
                // Get tile position
                Vector2 pos = {(pos_.X + x * tileWidth) * scale_, (pos_.Y + y * tileHeight) * scale_};
                m_tileset.drawTile(renderer_, pos, getTileAt({(float) x, (float) y}), scale_);
            }
        }
    }

    std::vector<physics::Shape *>
    TilesetRenderer::getCollisionShapesFor(int tile_, Rectangle range_, Vector2 tilesetPosition_) {
        std::vector<physics::Shape *> shapes;

        int sX = range_.X;
        int sY = range_.Y;
        int eX = range_.X + range_.Width;
        int eY = range_.Y + range_.Height;

        // Validate
        auto w = getWidth();
        auto h = getHeight();

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
                if (getTileAt({(float) x, (float) y}) == tile_) {
                    // Get polygon with world coordinates
                    auto poly = Rectangle(x * m_tileset.TileWidth + tilesetPosition_.X,
                                          y * m_tileset.TileWidth + tilesetPosition_.Y,
                                          m_tileset.TileWidth, m_tileset.TileHeight).toPolygonPtr();
                    shapes.push_back(dynamic_cast<physics::Shape *>(poly));
                }
            }
        }

        return shapes;
    }

    std::vector<physics::Shape *>
    TilesetRenderer::getCollisionShapesFor(std::vector<int> tiles_, Rectangle range_,
                                           Vector2 tilesetPosition_) {
        std::vector<physics::Shape *> shapes;

        int sX = range_.X;
        int sY = range_.Y;
        int eX = range_.X + range_.Width;
        int eY = range_.Y + range_.Height;

        // Validate
        auto w = getWidth() / m_tileset.TileWidth;
        auto h = getHeight() / m_tileset.TileHeight;

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
                if (std::find(tiles_.begin(), tiles_.end(), getTileAt({(float) x, (float) y})) != tiles_.end()) {
                    // Get polygon with world coordinates
                    auto poly = Rectangle(x * m_tileset.TileWidth + tilesetPosition_.X,
                                          y * m_tileset.TileWidth + tilesetPosition_.Y,
                                          m_tileset.TileWidth, m_tileset.TileHeight).toPolygonPtr();
                    shapes.push_back(dynamic_cast<physics::Shape *>(poly));
                }
            }
        }

        return shapes;
    }

    std::vector<physics::Shape *>
    TilesetRenderer::getCollisionShapesFor(int min_, int max_, Rectangle range_, Vector2 tilesetPosition_) {
        std::vector<physics::Shape *> shapes;

        int sX = range_.X;
        int sY = range_.Y;
        int eX = range_.X + range_.Width;
        int eY = range_.Y + range_.Height;

        // Validate
        auto w = getWidth() / m_tileset.TileWidth;
        auto h = getHeight() / m_tileset.TileHeight;

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
                auto t = getTileAt({(float) x, (float) y});
                if (t >= min_ && t <= max_) {
                    // Get polygon with world coordinates
                    auto poly = Rectangle(x * m_tileset.TileWidth + tilesetPosition_.X,
                                          y * m_tileset.TileWidth + tilesetPosition_.Y,
                                          m_tileset.TileWidth, m_tileset.TileHeight).toPolygonPtr();
                    shapes.push_back(dynamic_cast<physics::Shape *>(poly));
                }
            }
        }

        return shapes;
    }

    int TilesetRenderer::getHeight() {
        return m_height;
    }

    int TilesetRenderer::getTileAt(Vector2 pos_) {
        auto w = getWidth();
        auto h = getHeight();

        auto i = static_cast<int>(pos_.X) + w * static_cast<int>(pos_.Y);

        return m_tiles[i];
    }

    Tileset *TilesetRenderer::getTileset() {
        return &m_tileset;
    }

    int TilesetRenderer::getWidth() {
        return m_width;
    }

    void TilesetRenderer::setTileAt(Vector2 pos_, int tile_) {
        auto w = getWidth();
        auto h = getHeight();

        auto i = static_cast<int>(pos_.X) + w * static_cast<int>(pos_.Y);

        m_tiles[i] = tile_;
    }

    void TilesetRenderer::setTileData(std::vector<int> data_) {
        if (data_.size() != getWidth() * getHeight()) {
            throw std::runtime_error("Tile data does not match dimensions.");
        }
        m_tiles = data_;
    }
}