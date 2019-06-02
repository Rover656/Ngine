/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Vector2.h"

#include <cmath>

namespace NerdThings::Ngine::Math {
    // Public Fields

    const TVector2 TVector2::UnitX = {1, 0};
    const TVector2 TVector2::UnitY = {0, 1};
    const TVector2 TVector2::Zero = {0, 0};

    // Public Fields

    #ifdef INCLUDE_RAYLIB

    Vector2 TVector2::ToRaylibVec() const {
        return {X, Y};
    }

    TVector2 TVector2::FromRaylibVec(const Vector2 &vec) {
        return {vec.x, vec.y};
    }

    #endif

    TVector2 TVector2::Clamp(TVector2 value_, TVector2 min_, TVector2 max_) {
        return {
            std::clamp(value_.X, min_.X, max_.X),
            std::clamp(value_.Y, min_.Y, max_.Y)
        };
    }

    float TVector2::Dot(TVector2 b_) const {
        return (X * b_.X) + (Y * b_.Y);
    }

    float TVector2::Distance(TVector2 b_) const {
        const auto xdiff = X - b_.X;
        const auto ydiff = Y - b_.Y;

        return sqrtf(xdiff * xdiff + ydiff * ydiff);;
    }

    float TVector2::Magnitude() const {
        return sqrtf(MagnitudeSquared());
    }

    float TVector2::MagnitudeSquared() const {
        return (X * X) + (Y * Y);
    }

    struct TVector2 TVector2::Rotate(TVector2 center, float rotation_) {
        TVector2 ret = {X, Y};

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

    TVector2 TVector2::Transform(TMatrix matrix_) const {
        return {
            (X * matrix_.M0) + (Y * matrix_.M1) + matrix_.M3,
            (X * matrix_.M4) + (Y * matrix_.M5) + matrix_.M6
        };
    }

}
