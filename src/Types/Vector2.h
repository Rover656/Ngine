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

#ifndef VECTOR2_H
#define VECTOR2_H

namespace Ngine {
    namespace Types {
        // Not using NEAPI because we will implement all this in the header
        /*
         * A 2 Component Vector
         */
        struct TVector2 {
            // Public Fields

            /*
             * The X component
             */
            float X;

            /*
             * The Y component
             */
            float Y;

            // Public Constructor(s)

            /*
             * Create a zero vector
             */
            TVector2() : X(0), Y(0) {}

            /*
             * Create a vector with a single value for all components
             */
            TVector2(float val_) : X(val_), Y(val_) {}

            /*
             * Create a 2D vector
             */
            TVector2(float x_, float y_) : X(x_), Y(y_) {}

            // Public Methods

#ifdef INCLUDE_RAYLIB
            /*
             * Convert to a raylib vector
             */
            Vector2 ToRaylibVec() const {
                return { X, Y };
            }
#endif

            //TODO: Operators/Maths
        };
    }
}

#endif