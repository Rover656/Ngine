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

#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

namespace NerdThings::Ngine {
    /*
     * A collision shape interface
     */
    class ICollisionShape {
    public:
        // Destructor

        virtual ~ICollisionShape() = default;

        // Public fields

        /*
         * Check collision against another collision shape.
         */
        virtual bool CheckCollision(ICollisionShape* shape_) = 0;
    };
}

#endif //COLLISIONSHAPE_H
