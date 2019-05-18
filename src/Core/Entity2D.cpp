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

#include "Entity2D.h"

namespace NerdThings::Ngine::Core {
    // Public Constructor(s)

    Entity2D::Entity2D() {
        _Position = TVector2();
    }

    Entity2D::Entity2D(const TVector2 &_position) {
        _Position = _position;
    }

    // Public Methods

    TVector2 Entity2D::GetPosition() const {
        return _Position;
    }

    void Entity2D::SetPosition(TVector2 _position) {
        _Position = _position;
    }
}
