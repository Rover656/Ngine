/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "../Config.hpp"

#include "../Math.hpp"
#include "GraphicsDevice.hpp"

namespace ngine::graphics {
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
        Camera(Vector2 position_ = Vector2::Zero, float rotation_ = 0, float zoom_ = 1,
               Vector2 origin_ = Vector2::Zero) : Position(position_), Rotation(rotation_), Zoom(zoom_),
                                              Origin(origin_) {}

        /**
         * Begin using the camera (internal).
         *
         * @note Internally, this starts using the translation matrix for this camera.
         */
        void beginCamera(GraphicsDevice *graphicsDevice_) const;

        /**
         * Finish using the camera (internal).
         *
         * @note Internally, this pops the translation matrix for this camera.
         */
        void endCamera(GraphicsDevice *graphicsDevice_) const;

        /**
         * Convert a screen/window position to a world position.
         *
         * @param pos_ A screen position to be converted.
         * @return The world coordinate (according to the camera).
         */
        Vector2 screenToWorld(Vector2 pos_);

        /**
         * Convert a world position to a screen/window position.
         *
         * @param pos_ The world position to locate on the screen.
         * @return The position on the screen.
         */
        Vector2 worldToScreen(Vector2 pos_);

        /**
         * Get the translation matrix.
         *
         * @return The translation matrix for the camera.
         */
        Matrix getTranslationMatrix() const;
    };
}

#endif //CAMERA2D_H
