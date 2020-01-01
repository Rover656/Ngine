/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef BOUNDINGBOX2D_H
#define BOUNDINGBOX2D_H

#include "../Config.hpp"

#include "../Math.hpp"
#include "Shape.hpp"

namespace NerdThings::Ngine::Physics {
    /*
     * A 2D Bounding Box
     */
    struct NEAPI BoundingBox : public Shape {
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
