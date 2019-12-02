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

#include "EntityContainer.h"

#include "BaseEntity.h"

namespace NerdThings::Ngine {
    EntityContainer::EntityContainer(EntityContainer::ContainerType type_) : m_type(type_) {}

    EntityContainer::~EntityContainer() {
        // Delete entities
        for (auto ent : m_entities) delete ent.second;

        // Clear entities.
        m_entities.clear();
    }

    bool EntityContainer::RemoveChild(const std::string &name_) {
        // Get the entity
        const auto ent = GetChild<BaseEntity>(name_);

        if (ent != nullptr) {
            // Destroy the entity
            ent->_doDestroy();

            // Remove entity from map
            m_entities.erase(name_);

            // Delete entity from memory
            delete ent;
            return true;
        }

        return false;
    }

    bool EntityContainer::RemoveChild(BaseEntity *entity_) {
        // Search for the entity
        for (const auto &ent : m_entities) {
            if (ent.second == entity_)
                return RemoveChild(ent.first);
        }

        // We don't have this entity
        return false;
    }
}
