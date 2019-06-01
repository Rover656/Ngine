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

#ifndef ENTITYCONTAINER_H
#define ENTITYCONTAINER_H

#include "../ngine.h"

namespace NerdThings::Ngine::Core {
    // We do this forward declaration instead of include because BaseEntity uses this.
    class NEAPI BaseEntity;

    /*
     * An Entity Container, this just provides functions for entity management.
     * This is not normally used by a game.
     */
    class NEAPI EntityContainer {
        // Private Fields

        /*
         * All of the entities in the scene.
         * Every entity has a name.
         */
        std::map<std::string, BaseEntity*> _Entities;

        // Private Methods

        /*
         * Remove an entity parent
         */
        virtual void RemoveEntityParent(BaseEntity* ent_) = 0;

        /*
         * Set the entity parent
         */
        virtual void SetEntityParent(BaseEntity* ent_) = 0;
    public:
        // Public Methods

        /*
         * Add an entity.
         * Returns entity if success, null if fail
         */
        template <typename EntityType>
        EntityType* AddEntity(std::string name_, EntityType *entity_) {
            // Check the name is not taken
            if (HasEntity(name_))
                return nullptr;

            // Cast to BaseEntity to ensure this is a valid type
            auto ent = dynamic_cast<BaseEntity*>(entity_);

            if (ent != nullptr) {
                _Entities.insert({name_, ent});

                // Set parent
                SetEntityParent(ent);

                return entity_;
            }

            return nullptr;
        }

        /*
         * Get all of the entities
         */
        std::vector<BaseEntity*> GetEntities();

        /*
         * Get an entity by name.
         */
        template <typename EntityType>
        EntityType *GetEntity(const std::string &name_) {
            // Try to find the entity
            if (HasEntity(name_)) {
                return dynamic_cast<EntityType*>(_Entities.at(name_)); // Will return null if its the wrong type
            }

            return nullptr;
        }

        /*
         * Test whether there is an entity by this name.
         */
        bool HasEntity(const std::string &name_);

        /*
         * Remove an entity by name.
         * Returns success or fail.
         */
        bool RemoveEntity(const std::string &name_);
    };
}

#endif //ENTITYCONTAINER_H
