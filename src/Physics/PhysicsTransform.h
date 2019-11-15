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

#ifndef PHYSICSTRANSFORM_H
#define PHYSICSTRANSFORM_H

#include "../Ngine.h"

#include "../Vector2.h"

namespace NerdThings::Ngine::Physics {
    struct NEAPI PhysicsTransform {
        float Angle;
        Vector2 Position;
    };
}

#endif //PHYSICSTRANSFORM_H
