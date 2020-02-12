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

    TilesetRenderer::TilesetRenderer(const Tileset &tileset, int width, int height)
            : m_tileset(tileset), m_width(width), m_height(height),
              m_tiles(width * height) {
    }

    TilesetRenderer::TilesetRenderer(const Tileset &tileset, int width_, int height, std::vector<int> tiles)
            : m_tileset(tileset), m_width(width_), m_height(height) {
        if (tiles.size() != width_ * height) {
            throw std::runtime_error("Tile data does not match dimensions.");
        }
        m_tiles = tiles;
    }

    // Public Methods

    void TilesetRenderer::draw(graphics::Renderer *renderer, Vector2 pos, float scale, float rotation, Vector2 origin) {
        auto tileWidth = m_tileset.TileWidth;
        auto tileHeight = m_tileset.TileHeight;

        for (auto x = 0; x < m_width; x++) {
            for (auto y = 0; y < m_height; y++) {
                // Get tile position
                Vector2 p = {(pos.X + x * tileWidth) * scale, (pos.Y + y * tileHeight) * scale};

                // Don't know why this needs doubled, but sure
                auto tilePos = p - Vector2(-origin.X * tileWidth * 2, -origin.Y * tileHeight * 2);
                Vector2 tileOrigin = {-p.X, -p.Y};

                // Cheaty: Putting the wrong values in each argument for this to work
                m_tileset.drawTile(renderer, tilePos, getTileAt({(float) x, (float) y}), scale, rotation, tileOrigin);
            }
        }
    }

    void TilesetRenderer::draw(graphics::Renderer *renderer, Vector2 pos, Vector2 renderFrom, Vector2 renderTo, float scale) {
        // TODO: How do we rotate this stuff??

        // Get tile sizes
        auto tileWidth = m_tileset.TileWidth;
        auto tileHeight = m_tileset.TileHeight;

        // Tileset size
        auto tilesetWidth = m_width * scale;
        auto tilesetHeight = m_height * scale;

        // Correct coordinates
        if (renderFrom.X < pos.X) renderFrom.X = pos.X;
        if (renderFrom.Y < pos.Y) renderFrom.Y = pos.Y;
        if (renderFrom.X > pos.X + tilesetWidth * tileWidth) renderFrom.X = pos.X + tilesetWidth * tileWidth;
        if (renderFrom.Y > pos.Y + tilesetHeight * tileHeight) renderFrom.Y = pos.Y + tilesetHeight * tileHeight;

        if (renderTo.X < pos.X) renderTo.X = pos.X;
        if (renderTo.Y < pos.Y) renderTo.Y = pos.Y;
        if (renderTo.X > pos.X + tilesetWidth * tileWidth) renderTo.X = pos.X + tilesetWidth * tileWidth;
        if (renderTo.Y > pos.Y + tilesetHeight * tileHeight) renderTo.Y = pos.Y + tilesetHeight * tileHeight;

        // Get coordinates
        auto tXFrom = (int)floorf((renderFrom.X - pos.X) / tileWidth / scale);
        auto tYFrom = (int)floorf((renderFrom.Y - pos.Y) / tileHeight / scale);
        auto tXTo = (int)ceilf((renderTo.X - pos.X) / tileWidth / scale);
        auto tYTo = (int)ceilf((renderTo.Y - pos.Y) / tileHeight / scale);

        for (auto x = tXFrom; x < tXTo; x++) {
            for (auto y = tYFrom; y < tYTo; y++) {
                // Get tile position
                Vector2 p = {(pos.X + x * tileWidth) * scale, (pos.Y + y * tileHeight) * scale};
                m_tileset.drawTile(renderer, p, getTileAt({(float) x, (float) y}), scale);
            }
        }
    }

    std::vector<physics::Shape *>
    TilesetRenderer::getCollisionShapesFor(int tile, Rectangle range, Vector2 tilesetPosition) {
        std::vector<physics::Shape *> shapes;

        int sX = range.X;
        int sY = range.Y;
        int eX = range.X + range.Width;
        int eY = range.Y + range.Height;

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
                if (getTileAt({(float) x, (float) y}) == tile) {
                    // Get polygon with world coordinates
                    auto poly = Rectangle(x * m_tileset.TileWidth + tilesetPosition.X,
                                          y * m_tileset.TileWidth + tilesetPosition.Y,
                                          m_tileset.TileWidth, m_tileset.TileHeight).toPolygonPtr();
                    shapes.push_back(dynamic_cast<physics::Shape *>(poly));
                }
            }
        }

        return shapes;
    }

    std::vector<physics::Shape *>
    TilesetRenderer::getCollisionShapesFor(std::vector<int> tiles, Rectangle range,
                                           Vector2 tilesetPosition) {
        std::vector<physics::Shape *> shapes;

        int sX = range.X;
        int sY = range.Y;
        int eX = range.X + range.Width;
        int eY = range.Y + range.Height;

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
                if (std::find(tiles.begin(), tiles.end(), getTileAt({(float) x, (float) y})) != tiles.end()) {
                    // Get polygon with world coordinates
                    auto poly = Rectangle(x * m_tileset.TileWidth + tilesetPosition.X,
                                          y * m_tileset.TileWidth + tilesetPosition.Y,
                                          m_tileset.TileWidth, m_tileset.TileHeight).toPolygonPtr();
                    shapes.push_back(dynamic_cast<physics::Shape *>(poly));
                }
            }
        }

        return shapes;
    }

    std::vector<physics::Shape *>
    TilesetRenderer::getCollisionShapesFor(int min, int max, Rectangle range, Vector2 tilesetPosition) {
        std::vector<physics::Shape *> shapes;

        int sX = range.X;
        int sY = range.Y;
        int eX = range.X + range.Width;
        int eY = range.Y + range.Height;

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
                if (t >= min && t <= max) {
                    // Get polygon with world coordinates
                    auto poly = Rectangle(x * m_tileset.TileWidth + tilesetPosition.X,
                                          y * m_tileset.TileWidth + tilesetPosition.Y,
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

    int TilesetRenderer::getTileAt(Vector2 pos) {
        auto w = getWidth();
        auto h = getHeight();

        auto i = static_cast<int>(pos.X) + w * static_cast<int>(pos.Y);

        return m_tiles[i];
    }

    Tileset *TilesetRenderer::getTileset() {
        return &m_tileset;
    }

    int TilesetRenderer::getWidth() {
        return m_width;
    }

    void TilesetRenderer::setTileAt(Vector2 pos, int tile) {
        auto w = getWidth();
        auto h = getHeight();

        auto i = static_cast<int>(pos.X) + w * static_cast<int>(pos.Y);

        m_tiles[i] = tile;
    }

    void TilesetRenderer::setTileData(std::vector<int> data) {
        if (data.size() != getWidth() * getHeight()) {
            throw std::runtime_error("Tile data does not match dimensions.");
        }
        m_tiles = data;
    }
}