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

#ifndef ENTITY2D_H
#define ENTITY2D_H

// Include ngine
#include "../ngine.h"

#include "BaseEntity.h"

namespace NerdThings::Ngine::Core {
    /*
     * A 2D Entity
     */
    class NEAPI Entity2D : public BaseEntity {
    public:
        // Public Constructor(s)

        /*
         * Create a 2D Entity
         */
        Entity2D(Scene *parentScene_);

        /*
         * Create a 2D Entity at a position
         */
        Entity2D(Scene *parentScene_, const TVector2 &_position);

        // Public Methods

        /*
         * Get the entity position
         */
        [[nodiscard]] TVector2 GetPosition() const;

        void MoveBy(TVector2 moveBy_);

        /*
         * Set the entity position
         */
        void SetPosition(TVector2 position_);

    protected:
        // Protected Fields

        /*
         * The entity position
         */
        TVector2 _Position;
    };
}

#endif //ENTITY2D_H
