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

    public:
        // Public Methods

        /*
         * Add an entity.
         * Returns success or fail.
         */
        template <typename EntityType>
        bool AddEntity(std::string name_, EntityType *entity_);

        /*
         * Get all of the entities
         */
        std::vector<BaseEntity*> GetEntities();

        /*
         * Get an entity by name.
         */
        template <typename EntityType>
        EntityType *GetEntity(std::string name_);

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
