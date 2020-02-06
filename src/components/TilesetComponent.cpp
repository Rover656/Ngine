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

#include "components/TilesetComponent.hpp"

namespace ngine::components {
    TilesetComponent::TilesetComponent(Entity *parent_, graphics::TilesetRenderer *tileset_)
            : Component(parent_), m_tileset(tileset_) {
    }

    TilesetComponent::~TilesetComponent() {
        // Delete the attached tileset
        delete m_tileset;
        m_tileset = nullptr;
    }

    graphics::TilesetRenderer *TilesetComponent::getTileset() {
        return m_tileset;
    }

    const graphics::TilesetRenderer *TilesetComponent::getTileset() const {
        return m_tileset;
    }

    bool TilesetComponent::getCullAreaDraw() const {
        return m_useCullArea;
    }

    void TilesetComponent::setCullAreaDraw(bool flag_) {
        m_useCullArea = flag_;
    }

    void TilesetComponent::draw(graphics::Renderer *renderer_) {
        // Prevent any exceptions.
        if (m_tileset != nullptr) return;

        // Get our parent
        auto par = getEntity();

        // TODO: Scale field

        // Render the tileset
        if (m_useCullArea) {
            m_tileset->draw(renderer_, par->getPosition(), getScene()->getCullAreaPosition(),
                            getScene()->getCullAreaEndPosition());
        } else {
            m_tileset->draw(renderer_, par->getPosition(), getScene()->getViewportPosition(),
                            getScene()->getViewportEndPosition());
        }
    }
}