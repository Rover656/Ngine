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

#include "BaseEntity.h"

namespace Ngine {
    namespace Core {
        // Public Methods

        template <typename ComponentType>
        bool BaseEntity::AddComponent(const std::string name_, ComponentType *component_) {
            // Check the name is not taken
            if (HasComponent(name_))
                return false;

            // Cast to component to ensure this is valid
            auto comp = dynamic_cast<Component*>(component_);

            if (comp != nullptr) {
                _Components.insert({name_, comp});
                return true;
            }

            return false;
        }

        void BaseEntity::Draw() {
            // Will do stuff soon
        }

        template <typename ComponentType>
        ComponentType *BaseEntity::GetComponent(const std::string name_) {
            // Try to find the component
            if (HasComponent(name_)) {
                return dynamic_cast<ComponentType*>(_Components.at(name_)); // Will return null if its the wrong type
            }

            return nullptr;
        }

        std::vector<Component *> BaseEntity::GetComponents() {
            std::vector<Component*> vec;

            for (auto it = _Components.begin(); it != _Components.end(); ++it) {
                vec.push_back(it->second);
            }

            return vec;
        }

        bool BaseEntity::HasComponent(const std::string &name_) {
            return _Components.find(name_) != _Components.end();
        }

        bool BaseEntity::RemoveComponent(const std::string &name_) {
            // Check that we actually have a component by the name
            if (HasComponent(name_)) {
                // Remove component from map
                _Components.erase(name_);

                return true;
            }

            // We don't have this component
            return false;
        }

        void BaseEntity::Update() {
            // Do nothing for now... will run some stuff soon
        }

        // Protected Constructor(s)

        BaseEntity::BaseEntity() = default; // May implement something later on
    }
}
