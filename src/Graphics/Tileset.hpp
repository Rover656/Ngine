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

#ifndef TILESET_HPP
#define TILESET_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "Texture2D.hpp"

namespace NerdThings::Ngine::Graphics {
    /**
     * Information about a tileset.
     *
     * @todo Animated tilesets.
     * @todo Document after finishing API.
     */
    struct NEAPI Tileset {
        /**
         * The tileset texture.
         */
        Texture2D *Texture;

        /**
         * Tile width.
         */
        float TileHeight;

        /**
         * Tile height.
         */
        float TileWidth;

        Tileset(Texture2D *texture_, float tileWidth_, float tileHeight_);

        void DrawTile(Graphics::Renderer *renderer_, Vector2 position_, int tile_, float scale_ = 1.0f, float rotation_ = 0, Vector2 origin_ = Vector2::Zero);
    };
}

#endif //TILESET_HPP
