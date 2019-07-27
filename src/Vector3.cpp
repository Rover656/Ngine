#include "Vector3.h"

#include "Matrix.h"

namespace NerdThings::Ngine {
    float TVector3::Dot(TVector3 b_) const {
        return (X * b_.X) + (Y * b_.Y) + (Z * b_.Z);
    }

    float TVector3::Magnitude() const {
        return sqrtf(MagnitudeSquared());
    }

    float TVector3::MagnitudeSquared() const {
        return (X * X) + (Y * Y);
    }

    TVector3 TVector3::Transform(TMatrix matrix_) const {
        return {
                (X * matrix_.M0) + (Y * matrix_.M4) + (Z * matrix_.M8) + matrix_.M12,
                (X * matrix_.M1) + (Y * matrix_.M5) + (Z * matrix_.M9) + matrix_.M13,
                (X * matrix_.M2) + (Y * matrix_.M6) + (Z * matrix_.M10) + matrix_.M14,
        };
    }
}