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

#include "../Matrix.h"
#include "../Vector2.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * A Camera
     */
    struct NEAPI Camera {
        // Public Fields

        /*
         * Camera offset/origin
         */
        Vector2 Origin;

        /*
         * Camera position
         */
        Vector2 Position;

        /*
         * Camera rotation
         */
        float Rotation;

        /*
         * Camera zoom
         */
        float Zoom;

        // Public Constructor(s)

        Camera(Vector2 position_ = Vector2(), float rotation_ = 0, float zoom_ = 0,
               Vector2 origin_ = Vector2()) : Position(position_), Rotation(rotation_), Zoom(zoom_),
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
        Vector2 ScreenToWorld(Vector2 pos_);

        /*
         * Convert a world position to a screen/window position
         */
        Vector2 WorldToScreen(Vector2 pos_);

        /*
         * Get the translation matrix
         */
        Matrix GetTranslationMatrix() const;
    };
}

#endif //CAMERA2D_H
