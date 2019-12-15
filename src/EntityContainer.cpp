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

#include "EntityContainer.hpp"

#include "Entity.hpp"

namespace NerdThings::Ngine {
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

    bool EntityContainer::RemoveChild(const std::string &name_, bool delete_) {
        // Get the entity
        const auto ent = GetChild<Entity>(name_);

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

    bool EntityContainer::RemoveChild(Entity *entity_, bool delete_) {
        // Search for the entity
        for (const auto &ent : m_entities) {
            if (ent.second == entity_)
                return RemoveChild(ent.first, delete_);
        }

        return false;
    }
}
