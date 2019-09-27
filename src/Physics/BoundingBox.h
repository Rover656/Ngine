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

#ifndef BOUNDINGBOX2D_H
#define BOUNDINGBOX2D_H

#include "../ngine.h"

#include "../Vector2.h"
#include "CollisionShape.h"

namespace NerdThings::Ngine::Physics {
    /*
     * A 2D Bounding Box
     */
    struct NEAPI BoundingBox : public ICollisionShape {
    private:
        // Private Methods

        bool IsCompatible(ICollisionShape *shape_) override;

        bool RunCollisionCheck(ICollisionShape *shape_) override;
    public:
        // Public Fields

        /*
         * Maximum coordinate
         */
        Vector2 Max;

        /*
         * Minimum coordinate
         */
        Vector2 Min;

        // Public Constructor(s)

        /*
         * Create a default bounding box
         */
        BoundingBox()
            : Max({}), Min({}) {}

        // Public Methods

#ifdef INCLUDE_BOX2D
        b2PolygonShape ToB2Shape();
#endif
    };
}

#endif
