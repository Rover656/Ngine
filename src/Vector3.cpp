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
    float Vector3::Dot(Vector3 b_) const {
        return (X * b_.X) + (Y * b_.Y) + (Z * b_.Z);
    }

    float Vector3::Magnitude() const {
        return sqrtf(MagnitudeSquared());
    }

    float Vector3::MagnitudeSquared() const {
        return (X * X) + (Y * Y);
    }

    Vector3 Vector3::Transform(Matrix matrix_) const {
        return {
                (X * matrix_.M0) + (Y * matrix_.M4) + (Z * matrix_.M8) + matrix_.M12,
                (X * matrix_.M1) + (Y * matrix_.M5) + (Z * matrix_.M9) + matrix_.M13,
                (X * matrix_.M2) + (Y * matrix_.M6) + (Z * matrix_.M10) + matrix_.M14,
        };
    }
}