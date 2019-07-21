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

#include "ngine.h"

#include "Matrix.h"

namespace NerdThings::Ngine {
    /*
     * A vector with 2 Components
     */
    struct NEAPI TVector2 {
        // Public Fields

        /*
         * Unit Vector X
         */
        static const TVector2 UnitX;

        /*
         * Unit Vector Y
         */
        static const TVector2 UnitY;

        /*
         * The X component
         */
        float X;

        /*
         * The Y component
         */
        float Y;

        /*
         * Zero vector
         */
        static const TVector2 Zero;

        // Public Constructor(s)

        /*
         * Create a zero vector
         */
        TVector2() : X(0), Y(0) {}

        /*
         * Create a vector with a single value for all components
         */
        explicit TVector2(float val_) : X(val_), Y(val_) {}

        /*
         * Create a 2D vector
         */
        TVector2(float x_, float y_) : X(x_), Y(y_) {}

        // Public Methods

        #ifdef INCLUDE_RAYLIB

        /*
         * Convert to a raylib vector
         */
        Vector2 ToRaylibVec() const;

        /*
         * Convert from a raylib vector
         */
        static TVector2 FromRaylibVec(const Vector2 &vec);

        #endif

        /*
         * Clamp a vector
         */
        static TVector2 Clamp(TVector2 value_, TVector2 min_, TVector2 max_);

        /*
         * Find the dot product between two vectors
         */
        [[nodiscard]] float Dot(TVector2 b_) const;

        /*
         * Find the distance between two vectors
         */
        [[nodiscard]] float Distance(TVector2 b_) const;

        /*
         * Find the magnitude of a vector
         */
        [[nodiscard]] float Magnitude() const;

        /*
         * Find the magnitude of a vector squared.
         * Does not use sqrt so it is better for comparisons.
         */
        [[nodiscard]] float MagnitudeSquared() const;

        /*
         * Rotate this point around another
         */
        TVector2 Rotate(TVector2 center, float rotation_);

        /*
         * Transform by matrix
         */
        [[nodiscard]] TVector2 Transform(TMatrix matrix_) const;

        // Operators

        bool operator==(const TVector2 &b_) const {
            return X == b_.X && Y == b_.Y;
        }

        bool operator!=(const TVector2 &b_) const {
            return X != b_.X || Y != b_.Y;
        }

        friend TVector2 operator+(TVector2 a_, const TVector2 &b_) {
            return {
                a_.X + b_.X,
                a_.Y + b_.Y
            };
        }

        void operator+=(const TVector2 &b_) {
            X += b_.X;
            Y += b_.Y;
        }

        void operator+=(const float b_) {
            X += b_;
            Y += b_;
        }

        friend TVector2 operator-(TVector2 a_, const TVector2 &b_) {
            return {
                a_.X - b_.X,
                a_.Y - b_.Y
            };
        }

        void operator-=(const TVector2 &b_) {
            X -= b_.X;
            Y -= b_.Y;
        }

        void operator-=(const float b_) {
            X -= b_;
            Y -= b_;
        }

        friend TVector2 operator*(TVector2 a_, const TVector2 &b_) {
            return {
                a_.X * b_.X,
                a_.Y * b_.Y
            };
        }

        void operator*=(const TVector2 &b_) {
            X *= b_.X;
            Y *= b_.Y;
        }

        void operator*=(const float b_) {
            X *= b_;
            Y *= b_;
        }

        friend TVector2 operator/(TVector2 a_, const TVector2 &b_) {
            return {
                a_.X / b_.X,
                a_.Y / b_.Y
            };
        }

        void operator/=(const TVector2 &b_) {
            X /= b_.X;
            Y /= b_.Y;
        }

        void operator/=(const float b_) {
            X /= b_;
            Y /= b_;
        }

        //TODO: More Operators/Maths
    };
}

#endif //VECTOR2_H
