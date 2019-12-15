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

#include "Math.hpp"

namespace NerdThings::Ngine {
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

    Vector2 Vector2::Transform(const Ngine::Transform &transform_) const {
        auto rotated = Transform(transform_.Rotation);
        return Vector2(rotated.X + transform_.Position.X,
                      rotated.Y + transform_.Position.Y);
    }

    Vector2 Vector2::Transform(const Angle &angle_) const {
        return Vector2(angle_.Cos * X - angle_.Sine * Y,
                       angle_.Sine * X + angle_.Cos * Y);
    }

    struct Vector2 Vector2::Rotate(const Vector2 &center_, float rotation_) {
        Vector2 ret = {X, Y};

        float s = sinf(DegToRad(rotation_));
        float c = cosf(DegToRad(rotation_));

        // Translate point back to origin
        ret.X -= center_.X;
        ret.Y -= center_.Y;

        // Rotate point
        float xnew = ret.X * c - ret.Y * s;
        float ynew = ret.X * s + ret.Y * c;

        // Translate point back
        ret.X = xnew + center_.X;
        ret.Y = ynew + center_.Y;
        return ret;
    }

    Angle::Angle() {}

    Angle::Angle(float theta_) {
        Sine = sinf(DegToRad(theta_));
        Cos = cosf(DegToRad(theta_));
    }

    Angle &Angle::operator=(float theta_) {
        Sine = sinf(DegToRad(theta_));
        Cos = cosf(DegToRad(theta_));
        return *this;
    }

    Transform::Transform()
    : Rotation(0), Position(0, 0) {}

    Transform::Transform(const Vector2 &pos_, const Angle *rot_) {

    }
}