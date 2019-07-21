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

#ifndef MATRIX_H
#define MATRIX_H

#define TYPE_DECL_HEADER
#include "ngine.h"
#undef TYPE_DECL_HEADER

#include "Vector3.h"

namespace NerdThings::Ngine {
    /*
     * A mathematical matrix
     */
    struct NEAPI TMatrix {
        // Note: This is a copy of raymath.h in OO form

        // Public Fields

        // NOTE: Using raylib layout in case we need to pass matrices in the future
        float M0, M4, M8, M12;
        float M1, M5, M9, M13;
        float M2, M6, M10, M14;
        float M3, M7, M11, M15;

        static const TMatrix Identity;

        // Public Methods

        [[nodiscard]] float Determinant() const;

        static TMatrix Frustum(float left_, float right_, float bottom_, float top_, float near_, float far_);

        static TMatrix LookAt(Vector3 eye_, Vector3 target_, Vector3 up_);

        static TMatrix Orthographic(float left_, float right_, float bottom_, float top_, float near_, float far_);

        static TMatrix Perspective(float fovy_, float aspect_, float near_, float far_);

        static TMatrix Rotate(Vector3 axis_, float angle_);

        static TMatrix RotateX(float angle_);

        static TMatrix RotateY(float angle_);

        static TMatrix RotateZ(float angle_);

        static TMatrix Scale(float x, float y, float z);

        [[nodiscard]] float Trace() const;

        static TMatrix Translate(Vector3 translation_);

        static TMatrix Translate(float x_, float y_, float z_);

        [[nodiscard]] TMatrix Transpose() const;

        [[nodiscard]] TMatrix Invert() const;

        [[nodiscard]] TMatrix Normalize() const;

        // Operators

        friend TMatrix operator+(const TMatrix left_, const TMatrix right_) {
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

        void operator+=(const TMatrix right_) {
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

        friend TMatrix operator-(const TMatrix left_, const TMatrix right_) {
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

        void operator-=(const TMatrix right_) {
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

        friend TMatrix operator*(const TMatrix left_, const TMatrix right_) {
            TMatrix result = {0};

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
}

#endif //MATRIX_H
