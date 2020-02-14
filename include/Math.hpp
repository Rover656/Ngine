/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#ifndef MATH_HPP
#define MATH_HPP

#include "Config.hpp"

// TODO: The Math library will be going through some rewriting soon.

namespace ngine {
    struct Matrix;
    struct Angle;
    struct Transform2D;

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
         * @param val The value for the X and Y components.
         */
        explicit Vector2(float val) : X(val), Y(val) {}

        /**
         * Create a 2D vector
         *
         * @param x The X component.
         * @param y The Y component.
         */
        Vector2(float x, float y) : X(x), Y(y) {}

        /**
         * Clamp this vector.
         *
         * @param min The minimum value.
         * @param max The maximum value.
         * @return The clamped vector.
         */
        Vector2 clamp(const Vector2 &min, const Vector2 &max);

        /**
         * Clamp a vector.
         *
         * @param value The vector to clamp.
         * @param min The minimum value.
         * @param max The maximum value.
         * @return The clamped vector.
         */
        static Vector2 Clamp(const Vector2 &value, const Vector2 &min, const Vector2 &max);

        /**
         * Find the dot (scalar) product between this and another vector.
         *
         * @param b The other vector.
         * @return The dot (scalar) product.
         */
        float dot(const Vector2 &b) const;

        /**
         * Find the dot (scalar) product between two vectors.
         *
         * @param a The first vector.
         * @param b The seconds vector.
         * @return The dot (scalar) product.
         */
        static float Dot(const Vector2 &a, const Vector2 &b);

        /**
         * Find the distance between this and another vector.
         *
         * @param b The other vector.
         * @return The distance between the vectors.
         */
        float distance(const Vector2 &b) const;

        /**
         * Find the distance between two vectors.
         *
         * @param a The first vector.
         * @param b The second vector.
         * @return The distance between the two vectors.
         */
        static float Distance(const Vector2 &a, const Vector2 &b);

        /**
         * Find the magnitude of this vector
         *
         * @return The magnitude of this vector.
         * @note If using this for speed-critical comparisons, use MagnitudeSquared() instead.
         */
        float magnitude() const;

        /**
         * Find the magnitude of this vector squared.
         *
         * @return The magnitude squared.
         * @note Use this for speed-critical comparisons (doesn't use sqrt()).
         */
        float magnitudeSquared() const;

        /**
         * Translate using a matrix.
         *
         * @param matrix Matrix to transform with.
         * @return Transformed vector.
         */
        Vector2 transform(Matrix matrix) const;

        /**
         * Translate this vector by an angle.
         *
         * @param angle The angle to rotate by.
         */
        Vector2 transform(const Vector2 &origin, const Angle &angle) const;

        bool operator==(const Vector2 &b) const;

        bool operator!=(const Vector2 &b) const;

        friend Vector2 operator+(const Vector2 &a, const Vector2 &b);

        template <typename T>
        friend Vector2 operator+(const Vector2 &a, T b) {
            return {a.X + b, a.Y + b};
        }

        void operator+=(const Vector2 &b);

        template <typename T>
        void operator+=(T b) {
            X += b;
            Y += b;
        }

        friend Vector2 operator-(const Vector2 &a, const Vector2 &b);

        template <typename T>
        friend Vector2 operator-(const Vector2 &a, T b) {
            return {a.X - b, a.Y - b};
        }

        void operator-=(const Vector2 &b);

        template <typename T>
        void operator-=(T b) {
            X -= b;
            Y -= b;
        }

        friend Vector2 operator*(const Vector2 &a, const Vector2 &b);

        template <typename T>
        friend Vector2 operator*(const Vector2 &a, T b) {
            return {a.X * b, a.Y * b};
        }

        void operator*=(const Vector2 &b);

        template <typename T>
        void operator*=(T b) {
            X *= b;
            Y *= b;
        }

        friend Vector2 operator/(const Vector2 &a, const Vector2 &b);

        template <typename T>
        friend Vector2 operator/(const Vector2 &a, T b) {
            return {a.X / b, a.Y / b};
        }

        void operator/=(const Vector2 &b);

        template <typename T>
        void operator/=(T b) {
            X /= b;
            Y /= b;
        }
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
         * @param val The value for the X, Y and Z components.
         */
        explicit Vector3(float val) : X(val), Y(val), Z(val) {}

        /**
         * Create a 3D vector.
         *
         * @param x The X component.
         * @param y The Y component.
         * @param z The Z component.
         */
        Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

        /**
         * Clamp this vector.
         *
         * @param min The minimum value.
         * @param max The maximum value.
         * @return The clamped vector.
         */
        Vector3 clamp(const Vector3 &min, const Vector3 &max);

        /**
         * Clamp a vector.
         *
         * @param value The vector to clamp.
         * @param min The minimum value.
         * @param max The maximum value.
         * @return The clamped vector.
         */
        static Vector3 Clamp(const Vector3 &value, const Vector3 &min, const Vector3 &max);

        /**
         * Find the dot (scalar) product between this and another vector.
         *
         * @param b The other vector.
         * @return The dot (scalar) product.
         */
        float dot(const Vector3 &b) const;

        /**
         * Find the dot (scalar) product between two vectors.
         *
         * @param a The first vector.
         * @param b The seconds vector.
         * @return The dot (scalar) product.
         */
        static float Dot(const Vector3 &a, const Vector3 &b);

        /**
         * Find the distance between this and another vector.
         *
         * @param b The other vector.
         * @return The distance between the vectors.
         */
        float distance(const Vector3 &b);

        /**
         * Find the distance between two vectors.
         *
         * @param a The first vector.
         * @param b The second vector.
         * @return The distance between the two vectors.
         */
        static float Distance(const Vector3 &a, const Vector3 &b);

        /**
         * Find the magnitude of this vector
         *
         * @return The magnitude of this vector.
         * @note If using this for speed-critical comparisons, use MagnitudeSquared() instead.
         */
        float magnitude() const;

        /**
         * Find the magnitude of this vector squared.
         *
         * @return The magnitude squared.
         * @note Use this for speed-critical comparisons (doesn't use sqrt()).
         */
        float magnitudeSquared() const;

        // TODO: New transformation functions.

        /**
         * Translate using a matrix.
         *
         * @param matrix Matrix to transform with.
         * @return Transformed vector.
         */
        Vector3 transform(Matrix matrix) const;

        bool operator==(const Vector3 &b) const {
            return X == b.X && Y == b.Y && Z == b.Z;
        }

        bool operator!=(const Vector3 &b) const {
            return X != b.X || Y != b.Y || Z != b.Z;
        }

        friend Vector3 operator+(const Vector3 &a, const Vector3 &b) {
            return {
                    a.X + b.X,
                    a.Y + b.Y,
                    a.Z + b.Z
            };
        }

        void operator+=(const Vector3 &b) {
            X += b.X;
            Y += b.Y;
            Z += b.Z;
        }

        void operator+=(float b) {
            X += b;
            Y += b;
            Z += b;
        }

        friend Vector3 operator-(const Vector3 &a, const Vector3 &b) {
            return {
                    a.X - b.X,
                    a.Y - b.Y,
                    a.Z - b.Z
            };
        }

        void operator-=(const Vector3 &b) {
            X -= b.X;
            Y -= b.Y;
            Z -= b.Z;
        }

        void operator-=(float b) {
            X -= b;
            Y -= b;
            Z -= b;
        }

        friend Vector3 operator*(const Vector3 &a, const Vector3 &b) {
            return {
                    a.X * b.X,
                    a.Y * b.Y,
                    a.Z * b.Z
            };
        }

        void operator*=(const Vector3 &b) {
            X *= b.X;
            Y *= b.Y;
            Z *= b.Z;
        }

        void operator*=(float b) {
            X *= b;
            Y *= b;
            Z *= b;
        }

        friend Vector3 operator/(const Vector3 &a, const Vector3 &b) {
            return {
                    a.X / b.X,
                    a.Y / b.Y,
                    a.Z / b.Z
            };
        }

        void operator/=(const Vector3 &b) {
            X /= b.X;
            Y /= b.Y;
            Z /= b.Z;
        }

        void operator/=(float b) {
            X /= b;
            Y /= b;
            Z /= b;
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

        static Matrix Orthographic(float left, float right, float bottom, float top, float near_, float far_);

        static Matrix Rotate(const Angle &rotation, const Vector3 &axis);

        static Matrix RotateX(const Angle &rotation);

        static Matrix RotateY(const Angle &rotation);

        static Matrix RotateZ(const Angle &rotation);

        static Matrix Scale(float x, float y, float z);

        std::vector<float> toFloatArray() const;

        float trace() const;

        static Matrix Translate(Vector3 translation);

        static Matrix Translate(float x, float y, float z);

        Matrix transpose() const;

        Matrix invert() const;

        Matrix normalize() const;

        friend Matrix operator+(const Matrix &left, const Matrix &right) {
            auto result = Identity;

            result.M0 = left.M0 + right.M0;
            result.M1 = left.M1 + right.M1;
            result.M2 = left.M2 + right.M2;
            result.M3 = left.M3 + right.M3;
            result.M4 = left.M4 + right.M4;
            result.M5 = left.M5 + right.M5;
            result.M6 = left.M6 + right.M6;
            result.M7 = left.M7 + right.M7;
            result.M8 = left.M8 + right.M8;
            result.M9 = left.M9 + right.M9;
            result.M10 = left.M10 + right.M10;
            result.M11 = left.M11 + right.M11;
            result.M12 = left.M12 + right.M12;
            result.M13 = left.M13 + right.M13;
            result.M14 = left.M14 + right.M14;
            result.M15 = left.M15 + right.M15;

            return result;
        }

        void operator+=(const Matrix &right) {
            M0 += right.M0;
            M1 += right.M1;
            M2 += right.M2;
            M3 += right.M3;
            M4 += right.M4;
            M5 += right.M5;
            M6 += right.M6;
            M7 += right.M7;
            M8 += right.M8;
            M9 += right.M9;
            M10 += right.M10;
            M11 += right.M11;
            M12 += right.M12;
            M13 += right.M13;
            M14 += right.M14;
            M15 += right.M15;
        }

        friend Matrix operator-(const Matrix &left, const Matrix &right) {
            auto result = Identity;

            result.M0 = left.M0 - right.M0;
            result.M1 = left.M1 - right.M1;
            result.M2 = left.M2 - right.M2;
            result.M3 = left.M3 - right.M3;
            result.M4 = left.M4 - right.M4;
            result.M5 = left.M5 - right.M5;
            result.M6 = left.M6 - right.M6;
            result.M7 = left.M7 - right.M7;
            result.M8 = left.M8 - right.M8;
            result.M9 = left.M9 - right.M9;
            result.M10 = left.M10 - right.M10;
            result.M11 = left.M11 - right.M11;
            result.M12 = left.M12 - right.M12;
            result.M13 = left.M13 - right.M13;
            result.M14 = left.M14 - right.M14;
            result.M15 = left.M15 - right.M15;

            return result;
        }

        void operator-=(const Matrix &right) {
            M0 -= right.M0;
            M1 -= right.M1;
            M2 -= right.M2;
            M3 -= right.M3;
            M4 -= right.M4;
            M5 -= right.M5;
            M6 -= right.M6;
            M7 -= right.M7;
            M8 -= right.M8;
            M9 -= right.M9;
            M10 -= right.M10;
            M11 -= right.M11;
            M12 -= right.M12;
            M13 -= right.M13;
            M14 -= right.M14;
            M15 -= right.M15;
        }

        friend Matrix operator*(const Matrix &left, const Matrix &right) {
            Matrix result = {0};

            result.M0  = left.M0 * right.M0  + left.M4 * right.M1  + left.M8  * right.M2 + left.M12 * right.M3;
            result.M1  = left.M1 * right.M0  + left.M5 * right.M1  + left.M9  * right.M2 + left.M13 * right.M3;
            result.M2  = left.M2 * right.M0  + left.M6 * right.M1  + left.M10 * right.M2 + left.M14 * right.M3;
            result.M3  = left.M3 * right.M0  + left.M7 * right.M1  + left.M11 * right.M2 + left.M15 * right.M3;

            result.M4  = left.M0 * right.M4  + left.M4 * right.M5  + left.M8  * right.M6 + left.M12 * right.M7;
            result.M5  = left.M1 * right.M4  + left.M5 * right.M5  + left.M9  * right.M6 + left.M13 * right.M7;
            result.M6  = left.M2 * right.M4  + left.M6 * right.M5  + left.M10 * right.M6 + left.M14 * right.M7;
            result.M7  = left.M3 * right.M4  + left.M7 * right.M5  + left.M11 * right.M6 + left.M15 * right.M7;

            result.M8  = left.M0 * right.M8  + left.M4 * right.M9  + left.M8  * right.M10 + left.M12 * right.M11;
            result.M9  = left.M1 * right.M8  + left.M5 * right.M9  + left.M9  * right.M10 + left.M13 * right.M11;
            result.M10 = left.M2 * right.M8  + left.M6 * right.M9  + left.M10 * right.M10 + left.M14 * right.M11;
            result.M11 = left.M3 * right.M8  + left.M7 * right.M9  + left.M11 * right.M10 + left.M15 * right.M11;

            result.M12 = left.M0 * right.M12 + left.M4 * right.M13 + left.M8  * right.M14 + left.M12 * right.M15;
            result.M13 = left.M1 * right.M12 + left.M5 * right.M13 + left.M9  * right.M14 + left.M13 * right.M15;
            result.M14 = left.M2 * right.M12 + left.M6 * right.M13 + left.M10 * right.M14 + left.M14 * right.M15;
            result.M15 = left.M3 * right.M12 + left.M7 * right.M13 + left.M11 * right.M14 + left.M15 * right.M15;
            return result;
        }
    };

    /**
     * Describes an angle.
     */
    struct NEAPI Angle {
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
         * @param theta Size of the angle in degrees
         */
        Angle(float theta);

        Angle(float sine, float cos);

        Angle &operator=(float theta);

        /**
         * Get this angle in degrees.
         *
         * @return This angle in degrees.
         */
        float getDegrees() const;

        friend NEAPI Angle operator+(Angle angle, float theta);
        friend NEAPI Angle operator-(Angle angle, float theta);
        friend NEAPI Angle operator*(Angle angle, float theta);
        friend NEAPI Angle operator/(Angle angle, float theta);
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
        Transform2D(const Vector2 &pos, const Angle &rot);
    };
}

#endif //MATH_HPP
