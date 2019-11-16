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

#ifndef VECTOR2_H
#define VECTOR2_H

#include "Ngine.h"

#include "Matrix.h"

namespace NerdThings::Ngine {
    /**
     * A vector with 2 components.
     */
    struct NEAPI Vector2 {
        /**
         * Unit Vector X
         */
        static const Vector2 UnitX;

        /**
         * Unit Vector Y
         */
        static const Vector2 UnitY;

        /**
         * The X component
         */
        float X;

        /**
         * The Y component
         */
        float Y;

        /**
         * Zero vector
         */
        static const Vector2 Zero;

        /**
         * Create a zero vector
         */
        Vector2() : X(0), Y(0) {}

        /**
         * Create a vector with a single value for all components.
         *
         * @param val_ The value for the X and Y components.
         */
        explicit Vector2(float val_) : X(val_), Y(val_) {}

        /**
         * Create a 2D vector
         *
         * @param x_ The X component.
         * @param y_ The Y component.
         */
        Vector2(float x_, float y_) : X(x_), Y(y_) {}

        // Public Methods

        /**
         * Clamp this vector.
         *
         * @param min_ The minimum value.
         * @param max_ The maximum value.
         * @return The clamped vector.
         */
        Vector2 Clamp(const Vector2 &min_, const Vector2 &max_);

        /**
         * Clamp a vector.
         *
         * @param value_ The vector to clamp.
         * @param min_ The minimum value.
         * @param max_ The maximum value.
         * @return The clamped vector.
         */
        static Vector2 Clamp(const Vector2 &value_, const Vector2 &min_, const Vector2 &max_);

        /**
         * Find the dot (scalar) product between this and another vector.
         *
         * @param b_ The other vector.
         * @return The dot (scalar) product.
         */
        float Dot(const Vector2 &b_) const;

        /**
         * Find the dot (scalar) product between two vectors.
         *
         * @param a_ The first vector.
         * @param b_ The seconds vector.
         * @return The dot (scalar) product.
         */
        static float Dot(const Vector2 &a_, const Vector2 &b_);

        /**
         * Find the distance between this and another vector.
         *
         * @param b_ The other vector.
         * @return The distance between the vectors.
         */
        float Distance(const Vector2 &b_) const;

        /**
         * Find the distance between two vectors.
         *
         * @param a_ The first vector.
         * @param b_ The second vector.
         * @return The distance between the two vectors.
         */
        static float Distance(const Vector2 &a_, const Vector2 &b_);

        /**
         * Find the magnitude of this vector
         *
         * @return The magnitude of this vector.
         * @note If using this for speed-critical comparisons, use MagnitudeSquared() instead.
         */
        float Magnitude() const;

        /**
         * Find the magnitude of this vector squared.
         *
         * @return The magnitude squared.
         * @note Use this for speed-critical comparisons (doesn't use sqrt()).
         */
        float MagnitudeSquared() const;

        /**
         * Rotate this point around another.
         *
         * @param center_ The center of rotation.
         * @param rotation_ The angle to rotate by in degrees.
         * @return The rotated vector.
         */
        Vector2 Rotate(const Vector2 &center_, float rotation_);

        /**
         * Transform using a matrix.
         *
         * @param matrix_ Matrix to transform with.
         * @return Transformed vector.
         */
        Vector2 Transform(Matrix matrix_) const;

        bool operator==(const Vector2 &b_) const {
            return X == b_.X && Y == b_.Y;
        }

        bool operator!=(const Vector2 &b_) const {
            return X != b_.X || Y != b_.Y;
        }

        friend Vector2 operator+(const Vector2 &a_, const Vector2 &b_) {
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

        friend Vector2 operator-(const Vector2 &a_, const Vector2 &b_) {
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

        friend Vector2 operator*(const Vector2 &a_, const Vector2 &b_) {
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

        friend Vector2 operator/(const Vector2 &a_, const Vector2 &b_) {
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
