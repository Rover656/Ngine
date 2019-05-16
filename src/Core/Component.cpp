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

#include "Component.h"

namespace Ngine {
    namespace Core {
        // Public Methods

        BaseEntity *Component::GetParent() const {
            return _ParentEntity;
        }

        bool Component::HasParent() const {
            return _ParentEntity != nullptr;
        }

        void Component::OnAttach(BaseEntity* _attachedEntity) {
            _ParentEntity = _attachedEntity;
        }

        void Component::OnDetach() {
            _ParentEntity = nullptr;
        }

        void Component::OnDraw() {
            // Do nothing
        }

        void Component::OnUpdate() {
            // Do nothing
        }

        // Protected Constructor(s)

        Component::Component() {
            // Does nothing atm.
        }
    }
}