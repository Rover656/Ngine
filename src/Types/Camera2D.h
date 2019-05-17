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

#include "Vector2.h"

namespace Ngine {
    namespace Types {
        /*
         * A 2D Camera
         */
        struct TCamera2D {
            // Public Fields

            /*
             * Camera offset/origin
             */
            TVector2 Offset;

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

            TCamera2D(float rotation_ = 0, float zoom_ = 0) : Rotation(rotation_), Zoom(zoom_) {}

            // Public Methods

            #ifdef INCLUDE_RAYLIB
            /*
             * Convert to raylib camera
             */
            Camera2D ToRaylibCam() const {
                Camera2D cam;

                cam.rotation = Rotation;
                cam.offset = Offset.ToRaylibVec();
                cam.target = Target.ToRaylibVec();
                cam.zoom = Zoom;

                return cam;
            }

            /*
             * Convert from raylib camera
             */
            static TCamera2D FromRaylibCam(const Camera2D &cam) {
                TCamera2D tcam;

                tcam.Offset = TVector2::FromRaylibVec(cam.offset);
                tcam.Rotation = cam.rotation;
                tcam.Target = TVector2::FromRaylibVec(cam.target);
                tcam.Zoom = cam.zoom;

                return tcam;
            }
            #endif
        };
    }
}

#endif //CAMERA2D_H
