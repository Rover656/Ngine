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

#ifndef ENTITYCONTAINER_H
#define ENTITYCONTAINER_H

#include "Ngine.h"

namespace NerdThings::Ngine {
    // We do this forward declaration instead of include because BaseEntity uses this.
    class NEAPI BaseEntity;

    /**
     * This is a wrapper class which helps with having child entities.
     */
    class NEAPI EntityContainer {
        // Private Fields

        /**
         * The counter for unnamed entities.
         */
        int _Counter = 0;

        /**
         * All of the entities in the scene.
         * Every entity has a name.
         */
        std::map<std::string, std::unique_ptr<BaseEntity>> _Entities;

        /**
         * Process the addition of a parent
         *
         * @param ent_ The entity that has been added.
         */
        virtual void ProcessChildAdded(BaseEntity *ent_) = 0;

        /**
         * Process the removal of a parent
         *
         * @param ent_ The entity that has been removed.
         */
        virtual void ProcessChildRemoved(BaseEntity *ent_) = 0;
    public:
        // Destructors

        ~EntityContainer();

        // Public Methods

        /**
         * Add an unnamed entity.
         *
         * @tparam EntityType The entity type. Must be derrived from NerdThings::Ngine::BaseEntity.
         * @param entity_ The entity to add.
         * @return An std::pair with first being name and second the entity
         */
        template <typename EntityType>
        std::pair<std::string, EntityType *> AddChild(EntityType *entity_) {
            // Cast to BaseEntity to ensure this is a valid type
            auto ent = dynamic_cast<BaseEntity*>(entity_);

            if (ent != nullptr) {
                std::string name = "Unique" + std::to_string(_Counter);
                _Entities.insert({name, std::unique_ptr<BaseEntity>(ent)});
                _Counter++;

                // Set parent
                ProcessChildAdded(ent);

                return std::make_pair(name, entity_);
            }

            return std::make_pair("", nullptr);
        }

        /**
         * Add an entity.
         *
         * @tparam EntityType The entity type. Must be derrived from NerdThings::Ngine::BaseEntity.
         * @param name_ The name of the entity.
         * @param entity_ The entity to add.
         * @return The entity, for chaining commands.
         */
        template <typename EntityType>
        EntityType *AddChild(std::string name_, EntityType *entity_) {
            // Check the name is not taken
            if (HasChild(name_))
                return nullptr;

            // Cast to BaseEntity to ensure this is a valid type
            auto ent = dynamic_cast<BaseEntity*>(entity_);

            if (ent != nullptr) {
                _Entities.insert({name_, std::unique_ptr<BaseEntity>(ent)});

                // Set parent
                ProcessChildAdded(ent);

                return entity_;
            }

            return nullptr;
        }

        /**
         * Count all of the entities by type
         *
         * @tparam EntityType The entity type. Must be derrived from NerdThings::Ngine::BaseEntity.
         * @return The number of children who match the selected type.
         */
        template <typename EntityType = BaseEntity>
        int CountChildren() {
            int c = 0;
            for (auto e : GetEntities()) {
                if (dynamic_cast<EntityType*>(e) != nullptr) c++;
            }
            return c;
        }

        /**
         * Get all of the entities of type.
         *
         * @tparam EntityType The entity type. Must be derrived from NerdThings::Ngine::BaseEntity.
         * @return A vector with all of the entities of the provided type.
         */
        template <typename EntityType = BaseEntity>
        std::vector<EntityType*> GetChildren() {
            std::vector<EntityType*> ents;
            for (auto it = _Entities.begin(); it != _Entities.end(); ++it) {
                auto ent = it->second.get();
                auto casted = dynamic_cast<EntityType*>(ent);
                if (casted != nullptr) ents.push_back(ent);
            }
            return ents;
        }

        /**
         * Get an entity by name.
         *
         * @tparam EntityType The entity type. Must be derrived from NerdThings::Ngine::BaseEntity.
         * @param name_ The name of the entity to get.
         * @return The entity, or null if not found.
         */
        template <typename EntityType = BaseEntity>
        EntityType *GetChild(const std::string &name_) {
            // Try to find the entity
            if (HasChild(name_)) {
                return dynamic_cast<EntityType*>(_Entities.at(name_).get()); // Will return null if its the wrong type
            }

            return nullptr;
        }

        /**
         * Test whether there is an entity by this name.
         *
         * @param name_ The entity to test for.
         * @return Whether the entity exists or not.
         */
        bool HasChild(const std::string &name_);

        /**
         * Remove an entity by name.
         *
         * @param name_ The name of the entity to remove.
         * @return Whether the entity was removed or not.
         */
        bool RemoveChild(const std::string &name_);

        /**
         * Remove entity by pointer.
         *
         * @param entity_ The entity to be removed.
         * @return Whether the entity was removed or not.
         */
        bool RemoveChild(BaseEntity *entity_);
    };
}

#endif //ENTITYCONTAINER_H
