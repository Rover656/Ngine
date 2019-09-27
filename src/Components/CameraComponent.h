/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*   
*   File reviewed on 01/06/2019 by R.M
*
**********************************************************************************************/

#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "../ngine.h"

#include "../Graphics/Camera.h"
#include "../BaseEntity.h"
#include "../Component.h"
#include "../Vector2.h"

namespace NerdThings::Ngine::Components {
    /*
     * Camera component
     */
    class NEAPI CameraComponent : public Component {
        // Private Fields

        /*
         * The internal camera
         */
        Graphics::Camera _Camera;

        /*
         * Reference to on position changed event
         */
        EventHandleRef<EntityTransformChangedEventArgs> *_TransformChangeEvent;

        // Private Methods

        /*
         * Update camera parameters
         */
        void UpdateCamera(EntityTransformChangedEventArgs &e);
    public:

        // Public Constructors

        /*
         * Create a camera component
         */
        CameraComponent(BaseEntity *parent_, float zoom_, Vector2 origin_ = Vector2::Zero,
                        float rotation_ = 0);

        // Destructor

        virtual ~CameraComponent();

        // Public Fields

        /*
         * Mark this camera as the currently active camera.
         * Overrides any other active camera
         */
        void Activate();

        /*
         * Set the camera origin
         */
        void SetOrigin(Vector2 origin_);

        /*
         * Set camera rotation
         */
        void SetRotation(float rotation_);

        /*
         * Set camera zoom
         */
        void SetZoom(float zoom_);
    };
}

#endif //CAMERACOMPONENT_H
