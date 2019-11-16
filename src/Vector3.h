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

#ifndef VECTOR3_H
#define VECTOR3_H

#include "Ngine.h"

namespace NerdThings::Ngine {
    struct Matrix;

    /**
     * A vector with 3 components.
     */
    struct NEAPI Vector3 {
        /**
         * X unit vector
         */
        static const Vector3 UnitX;

        /**
         * Y unit vector
         */
        static const Vector3 UnitY;

        /**
         * Z unit vector
         */
        static const Vector3 UnitZ;

        /**
         * X component
         */
        float X;

        /**
         * Y component
         */
        float Y;

        /**
         * Z component
         */
        float Z;

        /**
         * Zero vector
         */
        static const Vector3 Zero;

        /**
         * Create a zero vector
         */
        Vector3() : X(0), Y(0), Z(0) {}

        /**
         * Create a vector with a single value.
         *
         * @param val_ The value for the X, Y and Z components.
         */
        explicit Vector3(float val_) : X(val_), Y(val_), Z(val_) {}

        /**
         * Create a 3D vector.
         *
         * @param x_ The X component.
         * @param y_ The Y component.
         * @param z_ The Z component.
         */
        Vector3(float x_, float y_, float z_) : X(x_), Y(y_), Z(z_) {}

        /**
         * Clamp this vector.
         *
         * @param min_ The minimum value.
         * @param max_ The maximum value.
         * @return The clamped vector.
         */
        Vector3 Clamp(const Vector3 &min_, const Vector3 &max_);

        /**
         * Clamp a vector.
         *
         * @param value_ The vector to clamp.
         * @param min_ The minimum value.
         * @param max_ The maximum value.
         * @return The clamped vector.
         */
        static Vector3 Clamp(const Vector3 &value_, const Vector3 &min_, const Vector3 &max_);

        /**
         * Find the dot (scalar) product between this and another vector.
         *
         * @param b_ The other vector.
         * @return The dot (scalar) product.
         */
        float Dot(const Vector3 &b_) const;

        /**
         * Find the dot (scalar) product between two vectors.
         *
         * @param a_ The first vector.
         * @param b_ The seconds vector.
         * @return The dot (scalar) product.
         */
        static float Dot(const Vector3 &a_, const Vector3 &b_);

        /**
         * Find the distance between this and another vector.
         *
         * @param b_ The other vector.
         * @return The distance between the vectors.
         */
        float Distance(const Vector3 &b_);

        /**
         * Find the distance between two vectors.
         *
         * @param a_ The first vector.
         * @param b_ The second vector.
         * @return The distance between the two vectors.
         */
        static float Distance(const Vector3 &a_, const Vector3 &b_);

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

        // TODO: Rotate??

        /**
         * Transform using a matrix.
         *
         * @param matrix_ Matrix to transform with.
         * @return Transformed vector.
         */
        Vector3 Transform(Matrix matrix_) const;

        bool operator==(const Vector3 &b_) const {
            return X == b_.X && Y == b_.Y && Z == b_.Z;
        }

        bool operator!=(const Vector3 &b_) const {
            return X != b_.X || Y != b_.Y || Z != b_.Z;
        }

        friend Vector3 operator+(Vector3 a_, const Vector3 &b_) {
            return {
                    a_.X + b_.X,
                    a_.Y + b_.Y,
                    a_.Z + b_.Z
            };
        }

        void operator+=(const Vector3 &b_) {
            X += b_.X;
            Y += b_.Y;
            Z += b_.Z;
        }

        void operator+=(const float b_) {
            X += b_;
            Y += b_;
            Z += b_;
        }

        friend Vector3 operator-(Vector3 a_, const Vector3 &b_) {
            return {
                    a_.X - b_.X,
                    a_.Y - b_.Y,
                    a_.Z - b_.Z
            };
        }

        void operator-=(const Vector3 &b_) {
            X -= b_.X;
            Y -= b_.Y;
            Z -= b_.Z;
        }

        void operator-=(const float b_) {
            X -= b_;
            Y -= b_;
            Z -= b_;
        }

        friend Vector3 operator*(Vector3 a_, const Vector3 &b_) {
            return {
                    a_.X * b_.X,
                    a_.Y * b_.Y,
                    a_.Z * b_.Z
            };
        }

        void operator*=(const Vector3 &b_) {
            X *= b_.X;
            Y *= b_.Y;
            Z *= b_.Z;
        }

        void operator*=(const float b_) {
            X *= b_;
            Y *= b_;
            Z *= b_;
        }

        friend Vector3 operator/(Vector3 a_, const Vector3 &b_) {
            return {
                    a_.X / b_.X,
                    a_.Y / b_.Y,
                    a_.Z / b_.Z
            };
        }

        void operator/=(const Vector3 &b_) {
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
