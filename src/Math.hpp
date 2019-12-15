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

#ifndef MATH_HPP
#define MATH_HPP

#include "Config.hpp"

// TEMP
#include "Matrix.hpp"
#include "Vector3.hpp"

namespace NerdThings::Ngine {
//    struct Matrix;
    struct Angle;
    struct Transform;

    struct Vector2 {
        /**
         * The X component
         */
        float X;

        /**
         * The Y component
         */
        float Y;

        /**
         * Unit Vector X
         */
        static const Vector2 UnitX;

        /**
         * Unit Vector Y
         */
        static const Vector2 UnitY;

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
         * Transform using a matrix.
         *
         * @param matrix_ Matrix to transform with.
         * @return Transformed vector.
         */
        Vector2 Transform(Matrix matrix_) const;

        /**
         * Transform this vector.
         *
         * @param transform_ The transformation to apply.
         */
        Vector2 Transform(const Ngine::Transform &transform_) const;

        /**
         * Transform this vector by an angle.
         *
         * @param angle_ The angle to rotate by.
         */
        Vector2 Transform(const Angle &angle_) const;

        /**
         * Rotate this point around another.
         *
         * @deprecated Please use Transform(Ngine::Transform &) instead.
         * @param center_ The center of rotation.
         * @param rotation_ The angle to rotate by in degrees.
         * @return The rotated vector.
         */
        Vector2 Rotate(const Vector2 &center_, float rotation_);

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
    };

//    struct Vector3 {
//
//    };
//
//    struct Matrix {
//
//    };

    struct Angle {
        /**
         * The sine of the angle.
         */
        float Sine = 0;

        /**
         * The cosine of the angle.
         */
        float Cos = 1;

        /**
         * Create an angle of 0 degrees.
         */
        Angle();

        /**
         * Create an angle.
         *
         * @param theta_ Size of the angle in degrees
         */
        Angle(float theta_);

        Angle &operator=(float theta_);
    };

    struct Transform {
        /**
         * The rotation of the given object.
         */
        Angle Rotation;

        /**
         * The position of the given object.
         */
        Vector2 Position;

        Transform();

        Transform(const Vector2 &pos_, const Angle *rot_);
    };
}

#endif //MATH_HPP
