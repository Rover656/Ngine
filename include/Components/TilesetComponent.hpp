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

#ifndef TILESETCOMPONENT_HPP
#define TILESETCOMPONENT_HPP

#include "../Config.hpp"

#include "../Graphics/TilesetRenderer.hpp"
#include "../Entity.hpp"
#include "../Component.hpp"

namespace Ngine::Components {
    /**
     * A tileset renderer component.
     * Attaches a tileset renderer to an entity for easy rendering.
     */
    class TilesetComponent : public Component {
        /**
         * The tileset renderer.
         */
        Graphics::TilesetRenderer *m_tileset;

        /**
         * Use the cull area instead of the viewport for rendering tiles.
         */
        bool m_useCullArea = false;
    public:
        /**
         * Create a tileset component with a tileset renderer.
         *
         * @note The lifecycle of the tileset renderer will be managed by this component now.
         * @param parent_ The entity we are attaching to.
         * @param tileset_ The tileset renderer we will be using.
         */
        TilesetComponent(Entity *parent_, Graphics::TilesetRenderer *tileset_);
        virtual ~TilesetComponent();

        /**
         * Get the tileset renderer.
         *
         * @return The tileset renderer.
         */
        Graphics::TilesetRenderer *GetTileset();

        /**
         * Get a read-only pointer to the tileset renderer.
         *
         * @return The (read-only) tileset renderer.
         */
        const Graphics::TilesetRenderer *GetTileset() const;

        /**
         * Determine whether we render tiles only inside the cull area, or in the viewport.
         *
         * @return true for rendering only in cull area, false for viewport.
         */
        bool GetCullAreaDraw() const;

        /**
         * Set whether or not we render tiles inside the cull area, or viewport
         *
         * @note Normally, rendering only in viewport is the best option, but if you have black-bars or something, cull area is better.
         * @param flag_ true for rendering only in cull area, false for viewport.
         */
        void SetCullAreaDraw(bool flag_);

        /**
         * Draw the tileset (called by engine).
         */
        void Draw(Graphics::Renderer *renderer_) override;
    };
}

#endif //TILESETCOMPONENT_HPP
