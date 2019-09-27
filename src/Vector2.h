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
    struct NEAPI Vector2 {
        // Public Fields

        /*
         * Unit Vector X
         */
        static const Vector2 UnitX;

        /*
         * Unit Vector Y
         */
        static const Vector2 UnitY;

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
        static const Vector2 Zero;

        // Public Constructor(s)

        /*
         * Create a zero vector
         */
        Vector2() : X(0), Y(0) {}

        /*
         * Create a vector with a single value for all components
         */
        explicit Vector2(float val_) : X(val_), Y(val_) {}

        /*
         * Create a 2D vector
         */
        Vector2(float x_, float y_) : X(x_), Y(y_) {}

        // Public Methods

        /*
         * Clamp a vector
         */
        static Vector2 Clamp(Vector2 value_, Vector2 min_, Vector2 max_);

        /*
         * Find the dot product between two vectors
         */
        float Dot(Vector2 b_) const;

        /*
         * Find the distance between two vectors
         */
        float Distance(Vector2 b_) const;

        /*
         * Find the magnitude of a vector
         */
        float Magnitude() const;

        /*
         * Find the magnitude of a vector squared.
         * Does not use sqrt so it is better for comparisons.
         */
        float MagnitudeSquared() const;

        /*
         * Rotate this point around another
         */
        Vector2 Rotate(Vector2 center, float rotation_);

        /*
         * Transform by matrix
         */
        Vector2 Transform(Matrix matrix_) const;

        // Operators

        bool operator==(const Vector2 &b_) const {
            return X == b_.X && Y == b_.Y;
        }

        bool operator!=(const Vector2 &b_) const {
            return X != b_.X || Y != b_.Y;
        }

        friend Vector2 operator+(Vector2 a_, const Vector2 &b_) {
            return {
                a_.X + b_.X,
                a_.Y + b_.Y
            };
        }

        void operator+=(const Vector2 &b_) {
            X += b_.X;
            Y += b_.Y;
        }

        void operator+=(const float b_) {
            X += b_;
            Y += b_;
        }

        friend Vector2 operator-(Vector2 a_, const Vector2 &b_) {
            return {
                a_.X - b_.X,
                a_.Y - b_.Y
            };
        }

        void operator-=(const Vector2 &b_) {
            X -= b_.X;
            Y -= b_.Y;
        }

        void operator-=(const float b_) {
            X -= b_;
            Y -= b_;
        }

        friend Vector2 operator*(Vector2 a_, const Vector2 &b_) {
            return {
                a_.X * b_.X,
                a_.Y * b_.Y
            };
        }

        void operator*=(const Vector2 &b_) {
            X *= b_.X;
            Y *= b_.Y;
        }

        void operator*=(const float b_) {
            X *= b_;
            Y *= b_;
        }

        friend Vector2 operator/(Vector2 a_, const Vector2 &b_) {
            return {
                a_.X / b_.X,
                a_.Y / b_.Y
            };
        }

        void operator/=(const Vector2 &b_) {
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
