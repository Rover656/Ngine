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

#include "../ngine.h"

#include "Matrix.h"
#include "Vector2.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * A Camera
     */
    struct NEAPI TCamera {
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

        /*
         * Begin using the camera
         */
        void BeginCamera() const;

        /*
         * Finish using the camera
         */
        void EndCamera() const;

        /*
         * Convert a screen/window position to a world position
         */
        TVector2 ScreenToWorld(TVector2 pos_);

        /*
         * Convert a world position to a screen/window position
         */
        TVector2 WorldToScreen(TVector2 pos_);

        /*
         * Get the translation matrix
         */
        TMatrix GetTranslationMatrix() const;
    };
}

#endif //CAMERA2D_H
