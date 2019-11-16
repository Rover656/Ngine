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

#include "Vector3.h"

#include "Matrix.h"

namespace NerdThings::Ngine {
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
}