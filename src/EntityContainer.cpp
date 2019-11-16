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
    // Destructor

    EntityContainer::~EntityContainer() {
        // Clear
        _Entities.clear();
    }

    // Public Methods

    bool EntityContainer::HasChild(const std::string &name_) {
        return _Entities.find(name_) != _Entities.end();
    }

    bool EntityContainer::RemoveChild(const std::string &name_) {
        // Get the entity
        const auto ent = GetChild<BaseEntity>(name_);

        if (ent != nullptr) {
            // Unsubscribe from updates
            ent->UnsubscribeFromUpdate();

            // Remove parent
            ProcessChildRemoved(ent);

            // Remove entity from map
            _Entities.erase(name_);

            return true;
        }

        // We don't have this entity
        return false;
    }

    bool EntityContainer::RemoveChild(BaseEntity *entity_) {
        // Search for the entity
        for (const auto& ent : _Entities) {
            if (ent.second.get() == entity_) {
                return RemoveChild(ent.first);;
            }
        }

        // We don't have this entity
        return false;
    }
}
