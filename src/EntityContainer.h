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

#include "ngine.h"

namespace NerdThings::Ngine {
    // We do this forward declaration instead of include because BaseEntity uses this.
    class NEAPI BaseEntity;

    /*
     * An Entity Container, this just provides functions for entity management.
     * This is not normally used by a game.
     */
    class NEAPI EntityContainer {
        // Private Fields

        /*
         * The counter for unnamed entities
         */
        int _Counter = 0;

        /*
         * All of the entities in the scene.
         * Every entity has a name.
         */
        std::map<std::string, std::unique_ptr<BaseEntity>> _Entities;

        // Private Methods

        /*
         * Remove an entity parent
         */
        virtual void RemoveEntityParent(BaseEntity *ent_) = 0;

        /*
         * Set the entity parent
         */
        virtual void SetEntityParent(BaseEntity *ent_) = 0;
    public:
        // Public Methods

        /*
         * Add an entity without a name.
         * This will give the entity name a numerical value.
         * Returns pair of name and entity if success, null for both if fail
         */
        template <typename EntityType>
        std::pair<std::string, EntityType *> AddEntity(EntityType *entity_) {
            // Cast to BaseEntity to ensure this is a valid type
            auto ent = dynamic_cast<BaseEntity*>(entity_);

            if (ent != nullptr) {
                std::string name = "Unique" + std::to_string(_Counter);
                _Entities.insert({name, std::unique_ptr<BaseEntity>(ent)});
                _Counter++;

                // Set parent
                SetEntityParent(ent);

                return std::make_pair(name, entity_);
            }

            return std::make_pair("", nullptr);
        }

        /*
         * Add an entity.
         * Returns entity if success, null if fail
         */
        template <typename EntityType>
        EntityType *AddEntity(std::string name_, EntityType *entity_) {
            // Check the name is not taken
            if (HasEntity(name_))
                return nullptr;

            // Cast to BaseEntity to ensure this is a valid type
            auto ent = dynamic_cast<BaseEntity*>(entity_);

            if (ent != nullptr) {
                _Entities.insert({name_, std::unique_ptr<BaseEntity>(ent)});

                // Set parent
                SetEntityParent(ent);

                return entity_;
            }

            return nullptr;
        }

        /*
         * Count all of the entities by type
         */
        template <typename EntityType>
        int CountEntitiesOfType() {
            int c = 0;
            for (auto e : GetEntities()) {
                if (dynamic_cast<EntityType*>(e) != nullptr) c++;
            }
            return c;
        }

        /*
         * Get all of the entities
         */
        std::vector<BaseEntity*> GetEntities();

        /*
         * Get all of the entities of type
         */
        template <typename EntityType>
        std::vector<EntityType*> GetEntitiesByType() {
            std::vector<EntityType*> ents;
            for (auto e : _Entities) {
                auto t = dynamic_cast<EntityType*>(e.second.get());
                if (t != nullptr) ents.push_back(t);
            }
            return ents;
        }

        /*
         * Get an entity by name.
         */
        template <typename EntityType>
        EntityType *GetEntity(const std::string &name_) {
            // Try to find the entity
            if (HasEntity(name_)) {
                return dynamic_cast<EntityType*>(_Entities.at(name_).get()); // Will return null if its the wrong type
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

        /*
         * Remove entity by pointer.
         */
        bool RemoveEntity(BaseEntity *entity_);
    };
}

#endif //ENTITYCONTAINER_H
