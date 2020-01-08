/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "../Config.hpp"

#include "../Math.hpp"
#include "GraphicsDevice.hpp"

namespace NerdThings::Ngine::Graphics {
    /**
     * Defines a camera configuration.
     */
    struct NEAPI Camera {
        /**
         * Camera origin.
         */
        Vector2 Origin;

        /**
         * Camera position.
         */
        Vector2 Position;

        /**
         * Camera rotation.
         */
        float Rotation;

        /**
         * Camera zoom.
         */
        float Zoom;

        /**
         * Create a new camera.
         *
         * @param position_ Camera position.
         * @param rotation_ Camera rotation in degrees.
         * @param zoom_ Camera zoom level.
         * @param origin_ Camera origin.
         */
        Camera(Vector2 position_ = Vector2::Zero, float rotation_ = 0, float zoom_ = 0,
               Vector2 origin_ = Vector2::Zero) : Position(position_), Rotation(rotation_), Zoom(zoom_),
                                              Origin(origin_) {}

        /**
         * Begin using the camera (internal).
         *
         * @note Internally, this starts using the translation matrix for this camera.
         */
        void BeginCamera(GraphicsDevice *graphicsDevice_) const;

        /**
         * Finish using the camera (internal).
         *
         * @note Internally, this pops the translation matrix for this camera.
         */
        void EndCamera(GraphicsDevice *graphicsDevice_) const;

        /**
         * Convert a screen/window position to a world position.
         *
         * @param pos_ A screen position to be converted.
         * @return The world coordinate (according to the camera).
         */
        Vector2 ScreenToWorld(Vector2 pos_);

        /**
         * Convert a world position to a screen/window position.
         *
         * @param pos_ The world position to locate on the screen.
         * @return The position on the screen.
         */
        Vector2 WorldToScreen(Vector2 pos_);

        /**
         * Get the translation matrix.
         *
         * @return The translation matrix for the camera.
         */
        Matrix GetTranslationMatrix() const;
    };
}

#endif //CAMERA2D_H
