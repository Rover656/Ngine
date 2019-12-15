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

#ifndef BOUNDINGBOX2D_H
#define BOUNDINGBOX2D_H

#include "../Config.hpp"

#include "../Math.hpp"
#include "CollisionShape.hpp"

namespace NerdThings::Ngine::Physics {
    /*
     * A 2D Bounding Box
     */
    struct NEAPI BoundingBox : public ICollisionShape {
        /*
         * Maximum coordinate
         */
        Vector2 Max;

        /*
         * Minimum coordinate
         */
        Vector2 Min;

        /*
         * Create a default bounding box
         */
        BoundingBox()
            : Max({}), Min({}) {}
    };
}

#endif
