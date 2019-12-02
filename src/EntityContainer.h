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
    class BaseEntity;

    /**
     * This is a wrapper class which helps with having child entities.
     */
    class NEAPI EntityContainer {
    protected:
        /**
         * A container type. Used for running actions on children.
         */
        enum ContainerType {
            ENTITY = 0,
            SCENE
        };
    private:
        /**
         * The container type.
         */
        ContainerType m_type = ENTITY;

        /**
         * The counter for unnamed entities.
         */
        int m_counter = 0;

        /**
         * All of the entities in the scene.
         * Every entity has a name.
         */
        std::map<std::string, BaseEntity *> m_entities;
    protected:
        /**
         * Initialize the entity container.
         *
         * @param type_ The container type.
         */
        explicit EntityContainer(ContainerType type_);
    public:
        ~EntityContainer();

        /**
         * Add an unnamed entity.
         *
         * @note The entity's lifecycle is now managed by this container.
         * @tparam EntityType The entity type. Must be derrived from NerdThings::Ngine::BaseEntity.
         * @param entity_ The entity to add.
         * @return An std::pair with first being name and second the entity
         */
        template <class EntityType = BaseEntity>
        std::pair<std::string, EntityType *> AddChild(EntityType *entity_) {
            // Cast to BaseEntity to ensure this is a valid type
            auto ent = dynamic_cast<BaseEntity*>(entity_);

            if (ent != nullptr) {
                std::string name = "Unique" + std::to_string(m_counter);
                m_entities.insert({name, std::unique_ptr<BaseEntity>(ent)});
                m_counter++;

                // Setup entity
                if (m_type == ENTITY) {
                    auto meEnt = (BaseEntity*) this;
                    ent->m_parentEntity = meEnt;
                    ent->m_parentScene = meEnt->m_parentScene;
                } else {
                    auto meScene = (Scene*)this;
                    ent->m_parentScene = meScene;
                }

                ent->m_parentScene->_addEntity(ent);

                return std::make_pair(name, entity_);
            }

            return std::make_pair("", nullptr);
        }

        /**
         * Add an entity.
         *
         * @note The entity's lifecycle is now managed by this container.
         * @tparam EntityType The entity type. Must be derrived from NerdThings::Ngine::BaseEntity.
         * @param name_ The name of the entity.
         * @param entity_ The entity to add.
         * @return The entity, for chaining commands.
         */
        template <class EntityType = BaseEntity>
        EntityType *AddChild(std::string name_, EntityType *entity_) {
            // Check the name is not taken
            if (HasChild(name_))
                return nullptr;

            // Cast to BaseEntity to ensure this is a valid type
            auto ent = dynamic_cast<BaseEntity*>(entity_);

            if (ent != nullptr) {
                m_entities.insert({name_, ent});

                // Setup entity
                if (m_type == ENTITY) {
                    auto meEnt = (BaseEntity*) this;
                    ent->m_parentEntity = meEnt;
                    ent->m_parentScene = meEnt->m_parentScene;
                } else {
                    auto meScene = (Scene*)this;
                    ent->m_parentScene = meScene;
                }

                ent->m_parentScene->_addEntity(ent);

                return entity_;
            }

            return nullptr;
        }

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

        /**
         * Get an entity by name.
         *
         * @tparam EntityType The entity type. Must be derrived from NerdThings::Ngine::BaseEntity.
         * @param name_ The name of the entity to get.
         * @return The entity, or null if not found.
         */
        template <class EntityType = BaseEntity>
        EntityType *GetChild(const std::string &name_) {
            // Try to find the entity
            if (HasChild(name_)) {
                return dynamic_cast<EntityType*>(m_entities.at(name_)); // Will return null if its the wrong type
            }

            return nullptr;
        }

        /**
         * Count all of the entities by type
         *
         * @tparam EntityType The entity type. Must be derrived from NerdThings::Ngine::BaseEntity.
         * @return The number of children who match the selected type.
         */
        template <class EntityType = BaseEntity>
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
        template <class EntityType = BaseEntity>
        std::vector<EntityType*> GetChildren() {
            std::vector<EntityType*> ents;
            for (auto it = m_entities.begin(); it != m_entities.end(); ++it) {
                auto ent = it->second;
                auto casted = dynamic_cast<EntityType*>(ent);
                if (casted != nullptr) ents.push_back(ent);
            }
            return ents;
        }

        /**
         * Test whether there is an entity by this name.
         *
         * @param name_ The entity to test for.
         * @return Whether the entity exists or not.
         */
        template<class EntityType = BaseEntity>
        bool HasChild(const std::string &name_) {
            auto exists = m_entities.find(name_) != m_entities.end();
            if (!exists) return false;
            return dynamic_cast<EntityType *>(m_entities[name_]) != nullptr;
        }
    };
}

#endif //ENTITYCONTAINER_H
