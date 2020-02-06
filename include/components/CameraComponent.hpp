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

#ifndef CAMERACOMPONENT_HPP
#define CAMERACOMPONENT_HPP

#include "../Config.hpp"

#include "../graphics/Camera.hpp"
#include "../Component.hpp"
#include "../Entity.hpp"
#include "../Math.hpp"

namespace ngine::components {
    /**
     * Camera management component.
     */
    class NEAPI CameraComponent : public Component {
        /**
         * The internal camera.
         */
        graphics::Camera m_camera;

        /**
         * Reference to on position changed event.
         */
        EventAttachment<EntityTransformChangedEventArgs> m_transformChangeEvent;

        /**
         * Update camera parameters.
         */
        void _updateCamera(EntityTransformChangedEventArgs e);
    public:
        /**
         * Create a camera component.
         *
         * @param parent_ The parent we are attaching to.
         * @param zoom_ The camera zoom level.
         * @param origin_ The camera origin. (defines camera center).
         * @param rotation_ The camera rotation in degrees.
         */
        CameraComponent(Entity *parent_, float zoom_, Vector2 origin_ = Vector2::Zero,
                        float rotation_ = 0);
        virtual ~CameraComponent();

        /**
         * Mark this camera as the currently active camera.
         * Overrides any other active camera.
         */
        void activate();

        /**
         * Get the camera origin
         *
         * @return The camera origin
         */
        Vector2 getOrigin();

        /**
         * Set the camera origin.
         *
         * @param origin_ New camera origin.
         */
        void setOrigin(Vector2 origin_);

        /**
         * Get the camera rotation.
         *
         * @return The camera rotation in degrees.
         */
        float getRotation();

        /**
         * Set camera rotation.
         *
         * @param rotation_ New camera rotation.
         */
        void setRotation(float rotation_);

        /**
         * Get the camera zoom.
         *
         * @return The camera's zoom level.
         */
        float getZoom();

        /**
         * Set camera zoom.
         *
         * @param zoom_ New camera zoom;
         */
        void setZoom(float zoom_);
    };
}

#endif //CAMERACOMPONENT_HPP
