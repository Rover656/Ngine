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

#include "Matrix.h"

namespace NerdThings::Ngine {
    /*
     * A vector with 2 Components
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
        Vector2 ToRaylibVec() const {
            return {X, Y};
        }

        /*
         * Convert from a raylib vector
         */
        static TVector2 FromRaylibVec(const Vector2 &vec) {
            return {vec.x, vec.y};
        }
        #endif

        [[nodiscard]] TVector2 Transform(TMatrix matrix_) const {
            return {
                (X * matrix_.M0) + (Y * matrix_.M1) + matrix_.M3,
                (X * matrix_.M4) + (Y * matrix_.M5) + matrix_.M6
            };
        }

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
