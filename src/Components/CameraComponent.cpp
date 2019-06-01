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

#include "CameraComponent.h"

#include "../Core/BaseEntity.h"

namespace NerdThings::Ngine::Components {
    // Private Methods

    void CameraComponent::UpdateCamera(EntityTransformChangedEventArgs &e) {
        // Update the target
        _Camera.Target = e.EntityPosition;

        // We ignore scale and rotation as this is independently controlled
    }

    // Destructor

    CameraComponent::~CameraComponent() {
        _TransformChangeEvent.UnBind();
    }

    // Public Constructors

    CameraComponent::CameraComponent(Core::BaseEntity *parent_, const float zoom_, const TVector2 origin_,
                                     const float rotation_)
        : Component(parent_) {
        auto par = GetParent<Core::BaseEntity>();

        // Setup camera
        _Camera = TCamera(rotation_, zoom_, par->GetPosition(), origin_);;

        // Attach to on position changed
        _TransformChangeEvent = par->OnTransformChanged.Bind(this, &CameraComponent::UpdateCamera);
    }

    // Public Methods

    void CameraComponent::Activate() {
        GetParent<Core::BaseEntity>()->GetParentScene()->SetActiveCamera(&_Camera);
    }

    void CameraComponent::SetOrigin(const TVector2 origin_) {
        _Camera.Origin = origin_;
    }

    void CameraComponent::SetRotation(const float rotation_) {
        _Camera.Rotation = rotation_;
    }

    void CameraComponent::SetZoom(const float zoom_) {
        _Camera.Zoom = zoom_;
    }
}
