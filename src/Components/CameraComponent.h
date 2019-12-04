/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "../Ngine.h"

#include "../Graphics/Camera.h"
#include "../Entity.h"
#include "../Component.h"
#include "../Vector2.h"

namespace NerdThings::Ngine::Components {
    /**
     * Camera management component.
     */
    class NEAPI CameraComponent : public Component {
        /**
         * The internal camera.
         */
        Graphics::Camera m_camera;

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
        void Activate();

        /**
         * Get the camera origin
         *
         * @return The camera origin
         */
        Vector2 GetOrigin();

        /**
         * Set the camera origin.
         *
         * @param origin_ New camera origin.
         */
        void SetOrigin(Vector2 origin_);

        /**
         * Get the camera rotation.
         *
         * @return The camera rotation in degrees.
         */
        float GetRotation();

        /**
         * Set camera rotation.
         *
         * @param rotation_ New camera rotation.
         */
        void SetRotation(float rotation_);

        /**
         * Get the camera zoom.
         *
         * @return The camera's zoom level.
         */
        float GetZoom();

        /**
         * Set camera zoom.
         *
         * @param zoom_ New camera zoom;
         */
        void SetZoom(float zoom_);
    };
}

#endif //CAMERACOMPONENT_H
