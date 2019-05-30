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

#include "../3rd-party/raylib/src/rlgl.h"

namespace NerdThings::Ngine {
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
            TVector2::FromRaylibVec(cam_.target),
            TVector2::FromRaylibVec(cam_.offset)
        };
    }

    #endif

    void TCamera::BeginCamera() const {
        EndMode2D(); //Ironic, isn't it.

        // This works how we want, raylib's doesn't work as well
        TMatrix ret = TMatrix::Identity;
        ret = ret * TMatrix::Translate(-Target.X, -Target.Y, 0);
        ret = ret * TMatrix::RotateZ(Rotation);
        ret = ret * TMatrix::Scale(Zoom, Zoom, 1);
        ret = ret * TMatrix::Translate(Origin.X, Origin.Y, 0);

        rlMultMatrixf(MatrixToFloat(*reinterpret_cast<Matrix*>(&ret))); //Hacky, but sure
    }

    void TCamera::EndCamera() const {
        EndMode2D();
    }
}
