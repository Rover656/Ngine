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

#include "Vector2.h"

#include <cmath>

namespace NerdThings::Ngine {
    // Public Fields

    const Vector2 Vector2::UnitX = {1, 0};
    const Vector2 Vector2::UnitY = {0, 1};
    const Vector2 Vector2::Zero = {0, 0};

    // Public Fields

    Vector2 Vector2::Clamp(Vector2 value_, Vector2 min_, Vector2 max_) {
        return {
                std::clamp(value_.X, min_.X, max_.X),
                std::clamp(value_.Y, min_.Y, max_.Y)
        };
    }

    float Vector2::Dot(Vector2 b_) const {
        return (X * b_.X) + (Y * b_.Y);
    }

    float Vector2::Distance(Vector2 b_) const {
        const auto xdiff = X - b_.X;
        const auto ydiff = Y - b_.Y;

        return sqrtf(xdiff * xdiff + ydiff * ydiff);;
    }

    float Vector2::Magnitude() const {
        return sqrtf(MagnitudeSquared());
    }

    float Vector2::MagnitudeSquared() const {
        return (X * X) + (Y * Y);
    }

    struct Vector2 Vector2::Rotate(Vector2 center, float rotation_) {
        Vector2 ret = {X, Y};

        float s = sinf(rotation_);
        float c = cosf(rotation_);

        // Translate point back to origin
        ret.X -= center.X;
        ret.Y -= center.Y;

        // Rotate point
        float xnew = ret.X * c - ret.Y * s;
        float ynew = ret.X * s + ret.Y * c;

        // Translate point back
        ret.X = xnew + center.X;
        ret.Y = ynew + center.Y;
        return ret;
    }

    Vector2 Vector2::Transform(Matrix matrix_) const {
        return {
                (X * matrix_.M0) + (Y * matrix_.M4) + matrix_.M12,
                (X * matrix_.M1) + (Y * matrix_.M5) + matrix_.M13
        };
    }

}
