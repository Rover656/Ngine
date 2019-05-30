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

#define TYPE_DECL_HEADER
#include "../ngine.h"
#undef TYPE_DECL_HEADER

#include "CollisionShape.h"
#include "Vector2.h"

namespace NerdThings::Ngine {
    /*
     * A 2D Bounding Box
     */
    struct NEAPI TBoundingBox : public ICollisionShape {
    private:
        // Private Methods

        bool IsCompatible(ICollisionShape *shape_) override;

        bool RunCollisionCheck(ICollisionShape *shape_) override;
    public:
        // Public Fields

        /*
         * Maximum coordinate
         */
        TVector2 Max;

        /*
         * Minimum coordinate
         */
        TVector2 Min;

        // Public Constructor(s)

        /*
         * Create a default bounding box
         */
        TBoundingBox()
            : Max({}), Min({}) {}
    };
}

#endif