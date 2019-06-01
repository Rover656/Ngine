/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*   
*   File reviewed on 01/06/2019 by R.M
*
**********************************************************************************************/

#include "EntityContainer.h"

#include "BaseEntity.h"

namespace NerdThings::Ngine::Core {
    // Public Methods

    std::vector<BaseEntity *> EntityContainer::GetEntities() {
        std::vector<BaseEntity*> vec;

        for (auto it = _Entities.begin(); it != _Entities.end(); ++it) {
            vec.push_back(it->second);
        }

        return vec;
    }

    bool EntityContainer::HasEntity(const std::string &name_) {
        return _Entities.find(name_) != _Entities.end();
    }

    bool EntityContainer::RemoveEntity(const std::string &name_) {
        // Get the entity
        const auto ent = GetEntity<BaseEntity>(name_);

        if (ent != nullptr) {
            // Remove parent
            RemoveEntityParent(ent);

            // Remove entity from map
            _Entities.erase(name_);

            return true;
        }

        // We don't have this entity
        return false;
    }
}
