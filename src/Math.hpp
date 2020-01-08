/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef MATH_HPP
#define MATH_HPP

#include "Config.hpp"

// TODO: The Math library will be going through some rewriting soon.

namespace NerdThings::Ngine {
    struct Matrix;
    struct Angle;
    struct Transform2D;

    /**
     * A vector with 2 components.
     */
    struct Vector2 {
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
         * The X component
         */
        float X;

        /**
         * The Y component
         */
        float Y;

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
         * Translate using a matrix.
         *
         * @param matrix_ Matrix to transform with.
         * @return Transformed vector.
         */
        Vector2 Transform(Matrix matrix_) const;

        /**
         * Translate this vector by an angle.
         *
         * @param angle_ The angle to rotate by.
         */
        Vector2 Transform(const Vector2 &origin_, const Angle &angle_) const;

        bool operator==(const Vector2 &b_) const;
        bool operator!=(const Vector2 &b_) const;
        friend Vector2 operator+(const Vector2 &a_, const Vector2 &b_);
        void operator+=(const Vector2 &b_);
        void operator+=(float b_);
        friend Vector2 operator-(const Vector2 &a_, const Vector2 &b_);
        void operator-=(const Vector2 &b_);
        void operator-=(float b_);
        friend Vector2 operator*(const Vector2 &a_, const Vector2 &b_);
        void operator*=(const Vector2 &b_);
        void operator*=(float b_);
        friend Vector2 operator/(const Vector2 &a_, const Vector2 &b_);
        void operator/=(const Vector2 &b_);
        void operator/=(float b_);
    };

    /**
     * A vector with 3 components.
     */
    struct NEAPI Vector3 {
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

        // TODO: New transformation functions.

        /**
         * Translate using a matrix.
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

    /**
     * A 4x4 Matrix.
     */
    struct NEAPI Matrix {
        float M0, M4, M8, M12;
        float M1, M5, M9, M13;
        float M2, M6, M10, M14;
        float M3, M7, M11, M15;

        static const Matrix Identity;

        float Determinant() const;

        static Matrix Orthographic(float left_, float right_, float bottom_, float top_, float near_, float far_);

        static Matrix Rotate(const Angle &rotation_, const Vector3 &axis_);

        static Matrix RotateX(const Angle &rotation_);

        static Matrix RotateY(const Angle &rotation_);

        static Matrix RotateZ(const Angle &rotation_);

        static Matrix Scale(float x, float y, float z);

        std::vector<float> ToFloatArray() const;

        float Trace() const;

        static Matrix Translate(Vector3 translation_);

        static Matrix Translate(float x_, float y_, float z_);

        Matrix Transpose() const;

        Matrix Invert() const;

        Matrix Normalize() const;

        friend Matrix operator+(const Matrix left_, const Matrix right_) {
            auto result = Identity;

            result.M0 = left_.M0 + right_.M0;
            result.M1 = left_.M1 + right_.M1;
            result.M2 = left_.M2 + right_.M2;
            result.M3 = left_.M3 + right_.M3;
            result.M4 = left_.M4 + right_.M4;
            result.M5 = left_.M5 + right_.M5;
            result.M6 = left_.M6 + right_.M6;
            result.M7 = left_.M7 + right_.M7;
            result.M8 = left_.M8 + right_.M8;
            result.M9 = left_.M9 + right_.M9;
            result.M10 = left_.M10 + right_.M10;
            result.M11 = left_.M11 + right_.M11;
            result.M12 = left_.M12 + right_.M12;
            result.M13 = left_.M13 + right_.M13;
            result.M14 = left_.M14 + right_.M14;
            result.M15 = left_.M15 + right_.M15;

            return result;
        }

        void operator+=(const Matrix right_) {
            M0 += right_.M0;
            M1 += right_.M1;
            M2 += right_.M2;
            M3 += right_.M3;
            M4 += right_.M4;
            M5 += right_.M5;
            M6 += right_.M6;
            M7 += right_.M7;
            M8 += right_.M8;
            M9 += right_.M9;
            M10 += right_.M10;
            M11 += right_.M11;
            M12 += right_.M12;
            M13 += right_.M13;
            M14 += right_.M14;
            M15 += right_.M15;
        }

        friend Matrix operator-(const Matrix left_, const Matrix right_) {
            auto result = Identity;

            result.M0 = left_.M0 - right_.M0;
            result.M1 = left_.M1 - right_.M1;
            result.M2 = left_.M2 - right_.M2;
            result.M3 = left_.M3 - right_.M3;
            result.M4 = left_.M4 - right_.M4;
            result.M5 = left_.M5 - right_.M5;
            result.M6 = left_.M6 - right_.M6;
            result.M7 = left_.M7 - right_.M7;
            result.M8 = left_.M8 - right_.M8;
            result.M9 = left_.M9 - right_.M9;
            result.M10 = left_.M10 - right_.M10;
            result.M11 = left_.M11 - right_.M11;
            result.M12 = left_.M12 - right_.M12;
            result.M13 = left_.M13 - right_.M13;
            result.M14 = left_.M14 - right_.M14;
            result.M15 = left_.M15 - right_.M15;

            return result;
        }

        void operator-=(const Matrix right_) {
            M0 -= right_.M0;
            M1 -= right_.M1;
            M2 -= right_.M2;
            M3 -= right_.M3;
            M4 -= right_.M4;
            M5 -= right_.M5;
            M6 -= right_.M6;
            M7 -= right_.M7;
            M8 -= right_.M8;
            M9 -= right_.M9;
            M10 -= right_.M10;
            M11 -= right_.M11;
            M12 -= right_.M12;
            M13 -= right_.M13;
            M14 -= right_.M14;
            M15 -= right_.M15;
        }

        friend Matrix operator*(const Matrix left_, const Matrix right_) {
            Matrix result = {0};

            result.M0 = left_.M0 * right_.M0 + left_.M1 * right_.M4 + left_.M2 * right_.M8 + left_.M3 * right_.M12;
            result.M1 = left_.M0 * right_.M1 + left_.M1 * right_.M5 + left_.M2 * right_.M9 + left_.M3 * right_.M13;
            result.M2 = left_.M0 * right_.M2 + left_.M1 * right_.M6 + left_.M2 * right_.M10 + left_.M3 * right_.M14;
            result.M3 = left_.M0 * right_.M3 + left_.M1 * right_.M7 + left_.M2 * right_.M11 + left_.M3 * right_.M15;
            result.M4 = left_.M4 * right_.M0 + left_.M5 * right_.M4 + left_.M6 * right_.M8 + left_.M7 * right_.M12;
            result.M5 = left_.M4 * right_.M1 + left_.M5 * right_.M5 + left_.M6 * right_.M9 + left_.M7 * right_.M13;
            result.M6 = left_.M4 * right_.M2 + left_.M5 * right_.M6 + left_.M6 * right_.M10 + left_.M7 * right_.M14;
            result.M7 = left_.M4 * right_.M3 + left_.M5 * right_.M7 + left_.M6 * right_.M11 + left_.M7 * right_.M15;
            result.M8 = left_.M8 * right_.M0 + left_.M9 * right_.M4 + left_.M10 * right_.M8 + left_.M11 * right_.M12;
            result.M9 = left_.M8 * right_.M1 + left_.M9 * right_.M5 + left_.M10 * right_.M9 + left_.M11 * right_.M13;
            result.M10 = left_.M8 * right_.M2 + left_.M9 * right_.M6 + left_.M10 * right_.M10 + left_.M11 * right_.M14;
            result.M11 = left_.M8 * right_.M3 + left_.M9 * right_.M7 + left_.M10 * right_.M11 + left_.M11 * right_.M15;
            result.M12 = left_.M12 * right_.M0 + left_.M13 * right_.M4 + left_.M14 * right_.M8 + left_.M15 * right_.M12;
            result.M13 = left_.M12 * right_.M1 + left_.M13 * right_.M5 + left_.M14 * right_.M9 + left_.M15 * right_.M13;
            result.M14 = left_.M12 * right_.M2 + left_.M13 * right_.M6 + left_.M14 * right_.M10 + left_.M15 * right_.
                    M14;
            result.M15 = left_.M12 * right_.M3 + left_.M13 * right_.M7 + left_.M14 * right_.M11 + left_.M15 * right_.
                    M15;

            return result;
        }
    };

    /**
     * Describes an angle.
     */
    struct Angle {
        /**
         * The sine of the angle.
         */
        float Sin = 0;

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

        Angle(float sine_, float cos_);

        Angle &operator=(float theta_);

        /**
         * Get this angle in degrees.
         *
         * @return This angle in degrees.
         */
        float GetDegrees() const;

        friend Angle operator+(Angle angle_, float theta_);
        friend Angle operator-(Angle angle_, float theta_);
        friend Angle operator*(Angle angle_, float theta_);
        friend Angle operator/(Angle angle_, float theta_);
    };

    /**
     * Describes a 2D transformation.
     */
    struct Transform2D {
        /**
         * Identity transformation.
         *
         * @todo Is this the correct name?
         */
        static Transform2D Identity;

        /**
         * The rotation of the given object.
         */
        Angle Rotation;

        /**
         * The position of the given object.
         */
        Vector2 Position;

        Transform2D();
        Transform2D(const Vector2 &pos_, const Angle &rot_);
    };
}

#endif //MATH_HPP
