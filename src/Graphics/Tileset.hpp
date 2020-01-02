/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
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
