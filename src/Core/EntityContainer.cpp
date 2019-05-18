/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "EntityContainer.h"

#include "BaseEntity.h"

namespace Ngine::Core {
    // Public Methods

    // TODO: Test that this all works...

    template <typename EntityType>
    bool EntityContainer::AddEntity(const std::string name_, EntityType *entity_) {
        // Check the name is not taken
        if (HasEntity(name_))
            return false;

        // Cast to BaseEntity to ensure this is a valid type
        auto ent = dynamic_cast<BaseEntity*>(entity_);

        if (ent != nullptr) {
            _Entities.insert({ name_, ent });
            return true;
        }

        return false;
    }

    std::vector<BaseEntity *> EntityContainer::GetEntities() {
        std::vector<BaseEntity*> vec;

        for (auto it = _Entities.begin(); it != _Entities.end(); ++it) {
            vec.push_back(it->second);
        }

        return vec;
    }

    template <typename EntityType>
    EntityType *EntityContainer::GetEntity(const std::string name_) {
        // Try to find the entity
        if (HasEntity(name_)) {
            return dynamic_cast<EntityType*>(_Entities.at(name_)); // Will return null if its the wrong type
        }

        return nullptr;
    }

    bool EntityContainer::HasEntity(const std::string &name_) {
        return _Entities.find(name_) != _Entities.end();
    }

    bool EntityContainer::RemoveEntity(const std::string &name_) {
        // Check that we actually have an entity by the name
        if (HasEntity(name_)) {
            // Remove entity from map
            _Entities.erase(name_);

            return true;
        }

        // We don't have this entity
        return false;
    }
}
