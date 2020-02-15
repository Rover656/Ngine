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

#include "Math.hpp"

#include <cmath>

namespace ngine {
    const Vector2 Vector2::UnitX = {1, 0};
    const Vector2 Vector2::UnitY = {0, 1};
    const Vector2 Vector2::Zero = {0, 0};

    Vector2 Vector2::clamp(const Vector2 &min, const Vector2 &max) {
        return Clamp(*this, min, max);
    }

    Vector2 Vector2::Clamp(const Vector2 &value, const Vector2 &min, const Vector2 &max) {
        return {
                std::clamp(value.X, min.X, max.X),
                std::clamp(value.Y, min.Y, max.Y)
        };
    }

    float Vector2::dot(const Vector2 &b) const {
        return Dot(*this, b);
    }

    float Vector2::Dot(const Vector2 &a, const Vector2 &b) {
        return (a.X * b.X) + (b.Y * b.Y);
    }

    float Vector2::distance(const Vector2 &b) const {
        return Distance(*this, b);
    }

    float Vector2::Distance(const Vector2 &a, const Vector2 &b) {
        const auto xdiff = a.X - b.X;
        const auto ydiff = a.Y - b.Y;
        return sqrtf(xdiff * xdiff + ydiff * ydiff);
    }

    float Vector2::magnitude() const {
        return sqrtf(magnitudeSquared());
    }

    float Vector2::magnitudeSquared() const {
        return (X * X) + (Y * Y);
    }

    Vector2 Vector2::transform(Matrix matrix) const {
        return {
                (X * matrix.M0) + (Y * matrix.M4) + matrix.M12,
                (X * matrix.M1) + (Y * matrix.M5) + matrix.M13
        };
    }

    Vector2 Vector2::transform(const Vector2 &origin, const Angle &angle) const {
        // Create return vector
        Vector2 ret = {X, Y};

        // Offset by origin
        ret.X -= origin.X;
        ret.Y -= origin.Y;

        // Rotate
        float xnew = ret.X * angle.Cos - ret.Y * angle.Sin;
        float ynew = ret.X * angle.Sin + ret.Y * angle.Cos;

        // Translate back
        ret.X = xnew + origin.X;
        ret.Y = ynew + origin.Y;
        return ret;
    }

    bool Vector2::operator==(const Vector2 &b) const {
        return X == b.X && Y == b.Y;
    }

    bool Vector2::operator!=(const Vector2 &b) const {
        return X != b.X || Y != b.Y;
    }

    Vector2 operator+(const Vector2 &a, const Vector2 &b) {
        return {
                a.X + b.X,
                a.Y + b.Y
        };
    }

    void Vector2::operator+=(const Vector2 &b) {
        X += b.X;
        Y += b.Y;
    }

    Vector2 operator-(const Vector2 &a, const Vector2 &b) {
        return {
                a.X - b.X,
                a.Y - b.Y
        };
    }

    void Vector2::operator-=(const Vector2 &b) {
        X -= b.X;
        Y -= b.Y;
    }

    Vector2 operator*(const Vector2 &a, const Vector2 &b) {
        return {
                a.X * b.X,
                a.Y * b.Y
        };
    }

    void Vector2::operator*=(const Vector2 &b) {
        X *= b.X;
        Y *= b.Y;
    }

    Vector2 operator/(const Vector2 &a, const Vector2 &b) {
        return {
                a.X / b.X,
                a.Y / b.Y
        };
    }

    void Vector2::operator/=(const Vector2 &b) {
        X /= b.X;
        Y /= b.Y;
    }

    const Vector3 Vector3::UnitX = {1, 0, 0};
    const Vector3 Vector3::UnitY = {0, 1, 0};
    const Vector3 Vector3::UnitZ = {0, 0, 1};
    const Vector3 Vector3::Zero = {0, 0, 0};

    Vector3 Vector3::clamp(const Vector3 &min, const Vector3 &max) {
        return Clamp(*this, min, max);
    }

    Vector3 Vector3::Clamp(const Vector3 &value, const Vector3 &min, const Vector3 &max) {
        return {
                std::clamp(value.X, min.X, max.X),
                std::clamp(value.Y, min.Y, max.Y),
                std::clamp(value.Z, min.Z, max.Z)
        };
    }

    float Vector3::dot(const Vector3 &b) const {
        return Dot(*this, b);
    }

    float Vector3::Dot(const Vector3 &a, const Vector3 &b) {
        return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
    }

    float Vector3::distance(const Vector3 &b) {
        return Distance(*this, b);
    }

    float Vector3::Distance(const Vector3 &a, const Vector3 &b) {
        const auto xdiff = a.X - b.X;
        const auto ydiff = a.Y - b.Y;
        const auto zdiff = a.Z - b.Z;
        return sqrtf(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
    }

    float Vector3::magnitude() const {
        return sqrtf(magnitudeSquared());
    }

    float Vector3::magnitudeSquared() const {
        return (X * X) + (Y * Y) + (Z * Z);
    }

    Vector3 Vector3::transform(Matrix matrix) const {
        return {
                (X * matrix.M0) + (Y * matrix.M4) + (Z * matrix.M8) + matrix.M12,
                (X * matrix.M1) + (Y * matrix.M5) + (Z * matrix.M9) + matrix.M13,
                (X * matrix.M2) + (Y * matrix.M6) + (Z * matrix.M10) + matrix.M14,
        };
    }

    const Matrix Matrix::Identity = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };

    float Matrix::Determinant() const {
        float result = {0};

        // Cache the matrix values (speed optimization)
        const auto a00 = M0, a01 = M1, a02 = M2, a03 = M3;
        const auto a10 = M4, a11 = M5, a12 = M6, a13 = M7;
        const auto a20 = M8, a21 = M9, a22 = M10, a23 = M11;
        const auto a30 = M12, a31 = M13, a32 = M14, a33 = M15;

        result = a30 * a21 * a12 * a03 - a20 * a31 * a12 * a03 - a30 * a11 * a22 * a03 + a10 * a31 * a22 * a03 +
                 a20 * a11 * a32 * a03 - a10 * a21 * a32 * a03 - a30 * a21 * a02 * a13 + a20 * a31 * a02 * a13 +
                 a30 * a01 * a22 * a13 - a00 * a31 * a22 * a13 - a20 * a01 * a32 * a13 + a00 * a21 * a32 * a13 +
                 a30 * a11 * a02 * a23 - a10 * a31 * a02 * a23 - a30 * a01 * a12 * a23 + a00 * a31 * a12 * a23 +
                 a10 * a01 * a32 * a23 - a00 * a11 * a32 * a23 - a20 * a11 * a02 * a33 + a10 * a21 * a02 * a33 +
                 a20 * a01 * a12 * a33 - a00 * a21 * a12 * a33 - a10 * a01 * a22 * a33 + a00 * a11 * a22 * a33;

        return result;
    }

    Matrix Matrix::Orthographic(float left, float right, float bottom, float top, float near_, float far_) {
        Matrix result = {0};

        result.M0 = 2.0f / (right - left);
        result.M1 = 0.0f;
        result.M2 = 0.0f;
        result.M3 = 0.0f;

        result.M4 = 0.0f;
        result.M5 = 2.0f / (top - bottom);
        result.M6 = 0.0f;
        result.M7 = 0.0f;

        result.M8 = 0.0f;
        result.M9 = 0.0f;
        result.M10 = -2.0f / (far_ - near_);
        result.M11 = 0.0f;

        result.M12 = -1.0f * (right + left) / (right - left);
        result.M13 = -1.0f * (top + bottom) / (top - bottom);
        result.M14 = -1.0f * (far_ + near_) / (far_ - near_);
        result.M15 = 1.0f;

        return result;
    }

    Matrix Matrix::Rotate(const Angle &rotation, const Vector3 &axis) {
        Matrix result = {0};

        auto x = axis.X, y = axis.Y, z = axis.Z;

        auto length = sqrtf(x * x + y * y + z * z);

        if ((length != 1.0f) && (length != 0.0f)) {
            length = 1.0f / length;
            x *= length;
            y *= length;
            z *= length;
        }

        const auto t = 1.0f - rotation.Cos;

        result.M0 = x * x * t + rotation.Cos;
        result.M1 = y * x * t + z * rotation.Sin;
        result.M2 = z * x * t - y * rotation.Sin;
        result.M3 = 0.0f;

        result.M4 = x * y * t - z * rotation.Sin;
        result.M5 = y * y * t + rotation.Cos;
        result.M6 = z * y * t + x * rotation.Sin;
        result.M7 = 0.0f;

        result.M8 = x * z * t + y * rotation.Sin;
        result.M9 = y * z * t - x * rotation.Sin;
        result.M10 = z * z * t + rotation.Cos;
        result.M11 = 0.0f;

        result.M12 = 0.0f;
        result.M13 = 0.0f;
        result.M14 = 0.0f;
        result.M15 = 1.0f;

        return result;
    }

    Matrix Matrix::RotateX(const Angle &rotation) {
        auto result = Identity;

        result.M5 = rotation.Cos;
        result.M6 = -rotation.Sin;
        result.M9 = rotation.Sin;
        result.M10 = rotation.Cos;

        return result;
    }

    Matrix Matrix::RotateY(const Angle &rotation) {
        auto result = Identity;

        result.M0 = rotation.Cos;
        result.M2 = rotation.Sin;
        result.M8 = -rotation.Sin;
        result.M10 = rotation.Cos;

        return result;
    }

    Matrix Matrix::RotateZ(const Angle &rotation) {
        auto result = Identity;

        result.M0 = rotation.Cos;
        result.M1 = -rotation.Sin;
        result.M4 = rotation.Sin;
        result.M5 = rotation.Cos;

        return result;
    }

    Matrix Matrix::Scale(float x, float y, float z) {
        const Matrix result = {
                x, 0.0f, 0.0f, 0.0f,
                0.0f, y, 0.0f, 0.0f,
                0.0f, 0.0f, z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };

        return result;
    }

    std::vector<float> Matrix::toFloatArray() const {
        auto buffer = std::vector<float>(16);

        buffer[0] = M0;
        buffer[1] = M1;
        buffer[2] = M2;
        buffer[3] = M3;
        buffer[4] = M4;
        buffer[5] = M5;
        buffer[6] = M6;
        buffer[7] = M7;
        buffer[8] = M8;
        buffer[9] = M9;
        buffer[10] = M10;
        buffer[11] = M11;
        buffer[12] = M12;
        buffer[13] = M13;
        buffer[14] = M14;
        buffer[15] = M15;

        return buffer;
    }

    float Matrix::trace() const {
        const auto result = (M0 + M5 + M10 + M15);
        return result;
    }

    Matrix Matrix::Translate(Vector3 translation) {
        return Translate(translation.X, translation.Y, translation.Z);
    }

    Matrix Matrix::Translate(float x, float y, float z) {
        const Matrix result = {
                1.0f, 0.0f, 0.0f, x,
                0.0f, 1.0f, 0.0f, y,
                0.0f, 0.0f, 1.0f, z,
                0.0f, 0.0f, 0.0f, 1.0f
        };

        return result;
    }

    Matrix Matrix::transpose() const {
        Matrix result = {0};

        result.M0 = M0;
        result.M1 = M4;
        result.M2 = M8;
        result.M3 = M12;
        result.M4 = M1;
        result.M5 = M5;
        result.M6 = M9;
        result.M7 = M13;
        result.M8 = M2;
        result.M9 = M6;
        result.M10 = M10;
        result.M11 = M14;
        result.M12 = M3;
        result.M13 = M7;
        result.M14 = M11;
        result.M15 = M15;

        return result;
    }

    Matrix Matrix::invert() const {
        Matrix result = {0};

        // Cache the matrix values (speed optimization)
        const auto a00 = M0, a01 = M1, a02 = M2, a03 = M3;
        const auto a10 = M4, a11 = M5, a12 = M6, a13 = M7;
        const auto a20 = M8, a21 = M9, a22 = M10, a23 = M11;
        const auto a30 = M12, a31 = M13, a32 = M14, a33 = M15;

        const auto b00 = a00 * a11 - a01 * a10;
        const auto b01 = a00 * a12 - a02 * a10;
        const auto b02 = a00 * a13 - a03 * a10;
        const auto b03 = a01 * a12 - a02 * a11;
        const auto b04 = a01 * a13 - a03 * a11;
        const auto b05 = a02 * a13 - a03 * a12;
        const auto b06 = a20 * a31 - a21 * a30;
        const auto b07 = a20 * a32 - a22 * a30;
        const auto b08 = a20 * a33 - a23 * a30;
        const auto b09 = a21 * a32 - a22 * a31;
        const auto b10 = a21 * a33 - a23 * a31;
        const auto b11 = a22 * a33 - a23 * a32;

        // Calculate the invert determinant (inlined to avoid double-caching)
        const auto invDet = 1.0f / (b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06);

        result.M0 = (a11 * b11 - a12 * b10 + a13 * b09) * invDet;
        result.M1 = (-a01 * b11 + a02 * b10 - a03 * b09) * invDet;
        result.M2 = (a31 * b05 - a32 * b04 + a33 * b03) * invDet;
        result.M3 = (-a21 * b05 + a22 * b04 - a23 * b03) * invDet;
        result.M4 = (-a10 * b11 + a12 * b08 - a13 * b07) * invDet;
        result.M5 = (a00 * b11 - a02 * b08 + a03 * b07) * invDet;
        result.M6 = (-a30 * b05 + a32 * b02 - a33 * b01) * invDet;
        result.M7 = (a20 * b05 - a22 * b02 + a23 * b01) * invDet;
        result.M8 = (a10 * b10 - a11 * b08 + a13 * b06) * invDet;
        result.M9 = (-a00 * b10 + a01 * b08 - a03 * b06) * invDet;
        result.M10 = (a30 * b04 - a31 * b02 + a33 * b00) * invDet;
        result.M11 = (-a20 * b04 + a21 * b02 - a23 * b00) * invDet;
        result.M12 = (-a10 * b09 + a11 * b07 - a12 * b06) * invDet;
        result.M13 = (a00 * b09 - a01 * b07 + a02 * b06) * invDet;
        result.M14 = (-a30 * b03 + a31 * b01 - a32 * b00) * invDet;
        result.M15 = (a20 * b03 - a21 * b01 + a22 * b00) * invDet;

        return result;
    }

    Matrix Matrix::normalize() const {
        Matrix result = {0};

        const auto det = Determinant();

        result.M0 = M0 / det;
        result.M1 = M1 / det;
        result.M2 = M2 / det;
        result.M3 = M3 / det;
        result.M4 = M4 / det;
        result.M5 = M5 / det;
        result.M6 = M6 / det;
        result.M7 = M7 / det;
        result.M8 = M8 / det;
        result.M9 = M9 / det;
        result.M10 = M10 / det;
        result.M11 = M11 / det;
        result.M12 = M12 / det;
        result.M13 = M13 / det;
        result.M14 = M14 / det;
        result.M15 = M15 / det;

        return result;
    }

    Angle::Angle() {}

    Angle::Angle(float theta) {
        Sin = sinf(DegToRad(theta));
        Cos = cosf(DegToRad(theta));
    }

    Angle::Angle(float sine, float cos)
            : Sin(sine), Cos(cos) {}

    Angle &Angle::operator=(float theta) {
        Sin = sinf(DegToRad(theta));
        Cos = cosf(DegToRad(theta));
        return *this;
    }

    float Angle::getDegrees() const {
        return RadToDeg(atan2f(Sin, Cos));
    }

    Angle Angle::operator-() const {
        return Angle(-getDegrees());
    }

    Angle operator+(Angle angle, float theta) {
        auto d = angle.getDegrees();
        return Angle(angle.getDegrees() + theta);
    }

    Angle operator-(Angle angle, float theta) {
        return Angle(angle.getDegrees() - theta);
    }

    Angle operator*(Angle angle, float theta) {
        return Angle(angle.getDegrees() * theta);
    }

    Angle operator/(Angle angle, float theta) {
        return Angle(angle.getDegrees() / theta);
    }

    Transform2D Transform2D::Identity = Transform2D({0, 0}, {0});

    Transform2D::Transform2D()
            : Rotation(0), Position(0, 0) {}

    Transform2D::Transform2D(const Vector2 &pos, const Angle &rot)
            : Rotation(rot), Position(pos) {}
}
