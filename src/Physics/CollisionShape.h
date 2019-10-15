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

#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include "../Vector2.h"

namespace NerdThings::Ngine::Physics {
    /*
     * A collision shape interface
     */
    struct ICollisionShape {
    private:
        // Private Methods

        /*
         * Is a shape able to collide with this type of shape
         */
        virtual bool IsCompatible(ICollisionShape *shape_) = 0;

        /*
         * Run a collision check
         */
        virtual bool RunCollisionCheck(ICollisionShape *shape_) = 0;

    public:
        // Destructor

        virtual ~ICollisionShape() = default;

        // Public fields

        /*
         * Check collision against another collision shape.
         */
        bool CheckCollision(ICollisionShape *shape_) {
            if (IsCompatible(shape_)) {
                return RunCollisionCheck(shape_);
            }
            if (shape_->IsCompatible(this)) {
                return shape_->RunCollisionCheck(this);
            }
            throw std::runtime_error("Two shapes are unable to collide with each other!");
        }
    };
}

#endif //COLLISIONSHAPE_H
