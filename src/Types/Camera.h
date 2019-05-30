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

#ifndef CAMERA2D_H
#define CAMERA2D_H

#define TYPE_DECL_HEADER
#include "../ngine.h"
#undef TYPE_DECL_HEADER

#include "Vector2.h"

namespace NerdThings::Ngine {
    /*
     * A Camera
     */
    struct TCamera {
        // Public Fields

        /*
         * Camera offset/origin
         */
        TVector2 Origin;

        /*
         * Camera rotation
         */
        float Rotation;

        /*
         * Camera target
         */
        TVector2 Target;

        /*
         * Camera zoom
         */
        float Zoom;

        // Public Constructor(s)

        TCamera(float rotation_ = 0, float zoom_ = 0, TVector2 target_ = TVector2(),
                TVector2 origin_ = TVector2()) : Rotation(rotation_), Zoom(zoom_), Target(target_),
                                                 Origin(origin_) {}

        // Public Methods

        #ifdef INCLUDE_RAYLIB

        /*
         * Convert to raylib camera
         */
        [[nodiscard]] Camera2D ToRaylibCam() const;

        /*
         * Convert from raylib camera
         */
        static TCamera FromRaylibCam(const Camera2D &cam_);

        #endif

        /*
         * Begin using the camera
         */
        void BeginCamera() const;

        /*
         * Finish using the camera
         */
        void EndCamera() const;
    };
}

#endif //CAMERA2D_H
