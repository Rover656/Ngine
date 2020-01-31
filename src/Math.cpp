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

namespace Ngine {
    const Vector2 Vector2::UnitX = {1, 0};
    const Vector2 Vector2::UnitY = {0, 1};
    const Vector2 Vector2::Zero = {0, 0};

    Vector2 Vector2::Clamp(const Vector2 &min_, const Vector2 &max_) {
        return Clamp(*this, min_, max_);
    }

    Vector2 Vector2::Clamp(const Vector2 &value_, const Vector2 &min_, const Vector2 &max_) {
        return {
                std::clamp(value_.X, min_.X, max_.X),
                std::clamp(value_.Y, min_.Y, max_.Y)
        };
    }

    float Vector2::Dot(const Vector2 &b_) const {
        return Dot(*this, b_);
    }

    float Vector2::Dot(const Vector2 &a_, const Vector2 &b_) {
        return (a_.X * b_.X) + (b_.Y * b_.Y);
    }

    float Vector2::Distance(const Vector2 &b_) const {
        return Distance(*this, b_);
    }

    float Vector2::Distance(const Vector2 &a_, const Vector2 &b_) {
        const auto xdiff = a_.X - b_.X;
        const auto ydiff = a_.Y - b_.Y;
        return sqrtf(xdiff * xdiff + ydiff * ydiff);
    }

    float Vector2::Magnitude() const {
        return sqrtf(MagnitudeSquared());
    }

    float Vector2::MagnitudeSquared() const {
        return (X * X) + (Y * Y);
    }

    Vector2 Vector2::Transform(Matrix matrix_) const {
        return {
                (X * matrix_.M0) + (Y * matrix_.M4) + matrix_.M12,
                (X * matrix_.M1) + (Y * matrix_.M5) + matrix_.M13
        };
    }

    Vector2 Vector2::Transform(const Vector2 &origin_, const Angle &angle_) const {
        // Create return vector
        Vector2 ret = {X, Y};

        // Offset by origin
        ret.X -= origin_.X;
        ret.Y -= origin_.Y;

        // Rotate
        float xnew = ret.X * angle_.Cos - ret.Y * angle_.Sin;
        float ynew = ret.X * angle_.Sin + ret.Y * angle_.Cos;

        // Translate back
        ret.X = xnew + origin_.X;
        ret.Y = ynew + origin_.Y;
        return ret;
    }

    bool Vector2::operator==(const Vector2 &b_) const {
        return X == b_.X && Y == b_.Y;
    }

    bool Vector2::operator!=(const Vector2 &b_) const {
        return X != b_.X || Y != b_.Y;
    }

    Vector2 operator+(const Vector2 &a_, const Vector2 &b_) {
        return {
                a_.X + b_.X,
                a_.Y + b_.Y
        };
    }

    void Vector2::operator+=(const Vector2 &b_) {
        X += b_.X;
        Y += b_.Y;
    }

    void Vector2::operator+=(float b_) {
        X += b_;
        Y += b_;
    }

    Vector2 operator-(const Vector2 &a_, const Vector2 &b_) {
        return {
                a_.X - b_.X,
                a_.Y - b_.Y
        };
    }

    void Vector2::operator-=(const Vector2 &b_) {
        X -= b_.X;
        Y -= b_.Y;
    }

    void Vector2::operator-=(float b_) {
        X -= b_;
        Y -= b_;
    }

    Vector2 operator*(const Vector2 &a_, const Vector2 &b_) {
        return {
                a_.X * b_.X,
                a_.Y * b_.Y
        };
    }

    void Vector2::operator*=(const Vector2 &b_) {
        X *= b_.X;
        Y *= b_.Y;
    }

    void Vector2::operator*=(float b_) {
        X *= b_;
        Y *= b_;
    }

    Vector2 operator/(const Vector2 &a_, const Vector2 &b_) {
        return {
                a_.X / b_.X,
                a_.Y / b_.Y
        };
    }

    void Vector2::operator/=(const Vector2 &b_) {
        X /= b_.X;
        Y /= b_.Y;
    }

    void Vector2::operator/=(float b_) {
        X /= b_;
        Y /= b_;
    }

    const Vector3 Vector3::UnitX = {1, 0, 0};
    const Vector3 Vector3::UnitY = {0, 1, 0};
    const Vector3 Vector3::UnitZ = {0, 0, 1};
    const Vector3 Vector3::Zero = {0, 0, 0};

    Vector3 Vector3::Clamp(const Vector3 &min_, const Vector3 &max_) {
        return Clamp(*this, min_, max_);
    }

    Vector3 Vector3::Clamp(const Vector3 &value_, const Vector3 &min_, const Vector3 &max_) {
        return {
                std::clamp(value_.X, min_.X, max_.X),
                std::clamp(value_.Y, min_.Y, max_.Y),
                std::clamp(value_.Z, min_.Z, max_.Z)
        };
    }

    float Vector3::Dot(const Vector3 &b_) const {
        return Dot(*this, b_);
    }

    float Vector3::Dot(const Vector3 &a_, const Vector3 &b_) {
        return (a_.X * b_.X) + (a_.Y * b_.Y) + (a_.Z * b_.Z);
    }

    float Vector3::Distance(const Vector3 &b_) {
        return Distance(*this, b_);
    }

    float Vector3::Distance(const Vector3 &a_, const Vector3 &b_) {
        const auto xdiff = a_.X - b_.X;
        const auto ydiff = a_.Y - b_.Y;
        const auto zdiff = a_.Z - b_.Z;
        return sqrtf(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
    }

    float Vector3::Magnitude() const {
        return sqrtf(MagnitudeSquared());
    }

    float Vector3::MagnitudeSquared() const {
        return (X * X) + (Y * Y) + (Z * Z);
    }

    Vector3 Vector3::Transform(Matrix matrix_) const {
        return {
                (X * matrix_.M0) + (Y * matrix_.M4) + (Z * matrix_.M8) + matrix_.M12,
                (X * matrix_.M1) + (Y * matrix_.M5) + (Z * matrix_.M9) + matrix_.M13,
                (X * matrix_.M2) + (Y * matrix_.M6) + (Z * matrix_.M10) + matrix_.M14,
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

    Matrix Matrix::Orthographic(const float left_, const float right_, const float bottom_, const float top_,
                                const float near_, const float far_) {
        Matrix result = {0};

        const auto rl = right_ - left_;
        const auto tb = top_ - bottom_;
        const auto fn = far_ - near_;

        result.M0 = 2.0f / rl;
        result.M1 = 0.0f;
        result.M2 = 0.0f;
        result.M3 = 0.0f;
        result.M4 = 0.0f;
        result.M5 = 2.0f / tb;
        result.M6 = 0.0f;
        result.M7 = 0.0f;
        result.M8 = 0.0f;
        result.M9 = 0.0f;
        result.M10 = -2.0f / fn;
        result.M11 = 0.0f;
        result.M12 = -(left_ + right_) / rl;
        result.M13 = -(top_ + bottom_) / tb;
        result.M14 = -(far_ + near_) / fn;
        result.M15 = 1.0f;

        return result;
    }

    Matrix Matrix::Rotate(const Angle &rotation_, const Vector3 &axis_) {
        Matrix result = {0};

        auto x = axis_.X, y = axis_.Y, z = axis_.Z;

        auto length = sqrtf(x * x + y * y + z * z);

        if ((length != 1.0f) && (length != 0.0f)) {
            length = 1.0f / length;
            x *= length;
            y *= length;
            z *= length;
        }

        const auto t = 1.0f - rotation_.Cos;

        result.M0 = x * x * t + rotation_.Cos;
        result.M1 = y * x * t + z * rotation_.Sin;
        result.M2 = z * x * t - y * rotation_.Sin;
        result.M3 = 0.0f;

        result.M4 = x * y * t - z * rotation_.Sin;
        result.M5 = y * y * t + rotation_.Cos;
        result.M6 = z * y * t + x * rotation_.Sin;
        result.M7 = 0.0f;

        result.M8 = x * z * t + y * rotation_.Sin;
        result.M9 = y * z * t - x * rotation_.Sin;
        result.M10 = z * z * t + rotation_.Cos;
        result.M11 = 0.0f;

        result.M12 = 0.0f;
        result.M13 = 0.0f;
        result.M14 = 0.0f;
        result.M15 = 1.0f;

        return result;
    }

    Matrix Matrix::RotateX(const Angle &rotation_) {
        auto result = Identity;

        result.M5 = rotation_.Cos;
        result.M6 = -rotation_.Sin;
        result.M9 = rotation_.Sin;
        result.M10 = rotation_.Cos;

        return result;
    }

    Matrix Matrix::RotateY(const Angle &rotation_) {
        auto result = Identity;

        result.M0 = rotation_.Cos;
        result.M2 = rotation_.Sin;
        result.M8 = -rotation_.Sin;
        result.M10 = rotation_.Cos;

        return result;
    }

    Matrix Matrix::RotateZ(const Angle &rotation_) {
        auto result = Identity;

        result.M0 = rotation_.Cos;
        result.M1 = -rotation_.Sin;
        result.M4 = rotation_.Sin;
        result.M5 = rotation_.Cos;

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

    std::vector<float> Matrix::ToFloatArray() const {
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

    float Matrix::Trace() const {
        const auto result = (M0 + M5 + M10 + M15);
        return result;
    }

    Matrix Matrix::Translate(Vector3 translation_) {
        return Translate(translation_.X, translation_.Y, translation_.Z);
    }

    Matrix Matrix::Translate(float x_, float y_, float z_) {
        const Matrix result = {
                1.0f, 0.0f, 0.0f, x_,
                0.0f, 1.0f, 0.0f, y_,
                0.0f, 0.0f, 1.0f, z_,
                0.0f, 0.0f, 0.0f, 1.0f
        };

        return result;
    }

    Matrix Matrix::Transpose() const {
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

    Matrix Matrix::Invert() const {
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

    Matrix Matrix::Normalize() const {
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

    Angle::Angle(float theta_) {
        Sin = sinf(DegToRad(theta_));
        Cos = cosf(DegToRad(theta_));
    }

    Angle::Angle(float sine_, float cos_)
            : Sin(sine_), Cos(cos_) {}

    Angle &Angle::operator=(float theta_) {
        Sin = sinf(DegToRad(theta_));
        Cos = cosf(DegToRad(theta_));
        return *this;
    }

    float Angle::GetDegrees() const {
        return RadToDeg(atan2f(Sin, Cos));
    }

    Angle operator+(Angle angle_, float theta_) {
        auto d = angle_.GetDegrees();
        return Angle(angle_.GetDegrees() + theta_);
    }

    Angle operator-(Angle angle_, float theta_) {
        return Angle(angle_.GetDegrees() - theta_);
    }

    Angle operator*(Angle angle_, float theta_) {
        return Angle(angle_.GetDegrees() * theta_);
    }

    Angle operator/(Angle angle_, float theta_) {
        return Angle(angle_.GetDegrees() / theta_);
    }

    Transform2D Transform2D::Identity = Transform2D({0, 0}, {0});

    Transform2D::Transform2D()
            : Rotation(0), Position(0, 0) {}

    Transform2D::Transform2D(const Vector2 &pos_, const Angle &rot_)
            : Rotation(rot_), Position(pos_) {}
}
