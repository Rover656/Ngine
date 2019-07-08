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

#include "Camera.h"

#include <rlgl.h>

namespace NerdThings::Ngine::Graphics {
    // Public Methods

    #ifdef INCLUDE_RAYLIB

    Camera2D TCamera::ToRaylibCam() const {
        Camera2D cam;

        cam.rotation = Rotation;
        cam.offset = Origin.ToRaylibVec();
        cam.target = Target.ToRaylibVec();
        cam.zoom = Zoom;

        return cam;
    }

    TCamera TCamera::FromRaylibCam(const Camera2D &cam_) {
        return {
            cam_.rotation,
            cam_.zoom,
            Math::TVector2::FromRaylibVec(cam_.target),
            Math::TVector2::FromRaylibVec(cam_.offset)
        };
    }

    #endif

    void TCamera::BeginCamera() const {
        EndMode2D(); //Ironic, isn't it.

        // This works how we want, raylib's doesn't work as well
        auto mat = GetTranslationMatrix();

        rlMultMatrixf(MatrixToFloat(*reinterpret_cast<Matrix*>(&mat))); //Hacky, but sure
    }

    void TCamera::EndCamera() const {
        EndMode2D();
    }

    Math::TVector2 TCamera::ScreenToWorld(Math::TVector2 pos_) {
        return pos_.Transform(GetTranslationMatrix().Invert());
    }

    Math::TVector2 TCamera::WorldToScreen(Math::TVector2 pos_) {
        return pos_.Transform(GetTranslationMatrix());
    }

    Math::TMatrix TCamera::GetTranslationMatrix() const {
        Math::TMatrix ret = Math::TMatrix::Identity;
        ret = ret * Math::TMatrix::Translate(Target.X, Target.Y, 0);
        ret = ret * Math::TMatrix::RotateZ(Rotation);
        ret = ret * Math::TMatrix::Scale(Zoom, Zoom, 1);
        ret = ret * Math::TMatrix::Translate(-Origin.X, -Origin.Y, 0);
        return ret;
    }
}
