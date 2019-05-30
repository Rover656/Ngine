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
        BeginMode2D(ToRaylibCam());
    }

    void TCamera::EndCamera() const {
        EndMode2D();
    }
}