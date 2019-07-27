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

#ifndef VECTOR3_H
#define VECTOR3_H

#include "ngine.h"

namespace NerdThings::Ngine {
    struct NEAPI TMatrix;

    /*
     * A 3 component vector
     */
    struct NEAPI TVector3 {
        // Public fields

        /*
         * X unit vector
         */
        static const TVector3 UnitX;

        /*
         * Y unit vector
         */
        static const TVector3 UnitY;

        /*
         * Z unit vector
         */
        static const TVector3 UnitZ;

        /*
         * X component
         */
        float X;

        /*
         * Y component
         */
        float Y;

        /*
         * Z component
         */
        float Z;

        /*
         * Zero vector
         */
        static const TVector3 Zero;

        // Public Constructor(s)

        /*
         * Create a zero vector
         */
        TVector3() : X(0), Y(0), Z(0) {}

        /*
         * Create a vector with a single value
         */
        explicit TVector3(float val_) : X(val_), Y(val_), Z(val_) {}

        /*
         * Create a 3D vector
         */
        TVector3(float x_, float y_, float z_) : X(x_), Y(y_), Z(z_) {}

        // Public Methods

        // TODO: Stuff like clamp

        /*
         * Get the dot product of the two vectors
         */
        float Dot(TVector3 b_) const;

        /*
         * Magnitude
         */
        float Magnitude() const;

        /*
         * Find the magnitude of the vector squared.
         * Avoids sqrt for better comparisons.
         */
        float MagnitudeSquared() const;
        
        TVector3 Transform(TMatrix matrix_) const;
        
        // Operators

        // Operators

        bool operator==(const TVector3 &b_) const {
            return X == b_.X && Y == b_.Y && Z == b_.Z;
        }

        bool operator!=(const TVector3 &b_) const {
            return X != b_.X || Y != b_.Y || Z != b_.Z;
        }

        friend TVector3 operator+(TVector3 a_, const TVector3 &b_) {
            return {
                    a_.X + b_.X,
                    a_.Y + b_.Y,
                    a_.Z + b_.Z
            };
        }

        void operator+=(const TVector3 &b_) {
            X += b_.X;
            Y += b_.Y;
            Z += b_.Z;
        }

        void operator+=(const float b_) {
            X += b_;
            Y += b_;
            Z += b_;
        }

        friend TVector3 operator-(TVector3 a_, const TVector3 &b_) {
            return {
                    a_.X - b_.X,
                    a_.Y - b_.Y,
                    a_.Z - b_.Z
            };
        }

        void operator-=(const TVector3 &b_) {
            X -= b_.X;
            Y -= b_.Y;
            Z -= b_.Z;
        }

        void operator-=(const float b_) {
            X -= b_;
            Y -= b_;
            Z -= b_;
        }

        friend TVector3 operator*(TVector3 a_, const TVector3 &b_) {
            return {
                    a_.X * b_.X,
                    a_.Y * b_.Y,
                    a_.Z * b_.Z
            };
        }

        void operator*=(const TVector3 &b_) {
            X *= b_.X;
            Y *= b_.Y;
            Z *= b_.Z;
        }

        void operator*=(const float b_) {
            X *= b_;
            Y *= b_;
            Z *= b_;
        }

        friend TVector3 operator/(TVector3 a_, const TVector3 &b_) {
            return {
                    a_.X / b_.X,
                    a_.Y / b_.Y,
                    a_.Z / b_.Z
            };
        }

        void operator/=(const TVector3 &b_) {
            X /= b_.X;
            Y /= b_.Y;
            Z /= b_.Z;
        }

        void operator/=(const float b_) {
            X /= b_;
            Y /= b_;
            Z /= b_;
        }
        
        // TODO: Moar
    };
}

#endif //VECTOR3_H
