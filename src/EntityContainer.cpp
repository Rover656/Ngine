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

#include "EntityContainer.hpp"

#include "Entity.hpp"

namespace ngine {
    void EntityContainer::_addEntity(Entity *entity_) {
        if (m_type == ENTITY) {
            auto meEnt = (Entity*) this;
            entity_->m_parentEntity = meEnt;
            entity_->_addToScene(meEnt->m_parentScene);
        } else {
            auto meScene = (Scene*) this;
            entity_->_addToScene(meScene);
        }
    }

    EntityContainer::EntityContainer(EntityContainer::ContainerType type_) : m_type(type_) {}

    EntityContainer::~EntityContainer() {
        // Delete entities
        for (const auto& ent : m_entities) delete ent.second;

        // Clear entities.
        m_entities.clear();
    }

    bool EntityContainer::removeChild(const std::string &name_, bool delete_) {
        // Get the entity
        const auto ent = getChild<Entity>(name_);

        if (ent != nullptr) {
            // Destroy the entity
            ent->_removeFromScene();

            // Remove entity from map
            m_entities.erase(name_);

            // Delete entity from memory
            if (delete_) delete ent;
            return true;
        }

        return false;
    }

    bool EntityContainer::removeChild(Entity *entity_, bool delete_) {
        // Search for the entity
        for (const auto &ent : m_entities) {
            if (ent.second == entity_)
                return removeChild(ent.first, delete_);
        }

        return false;
    }
}
