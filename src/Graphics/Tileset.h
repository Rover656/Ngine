#ifndef TILESET_H
#define TILESET_H

#include "ngine.h"

#include "../Math/Vector2.h"
#include "Texture2D.h"

namespace NerdThings::Ngine::Graphics {
    // TODO: Animated tilesets
    /*
     * A tileset
     */
    struct NEAPI TTileset {
        // Public Fields

        /*
         * The tileset texture
         */
        TTexture2D *_Texture;

        /*
         * Tile width
         */
        float _TileHeight;

        /*
         * Tile height
         */
        float _TileWidth;

        // Public Constructor(s)

        TTileset(TTexture2D *texture_, float tileWidth_, float tileHeight_);

        // Public Methods

        void DrawTile(Math::TVector2 position_, int tile_);

        float GetTileHeight() const;

        float GetTileWidth() const;
    };
}

#endif //TILESET_H
