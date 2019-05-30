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

#include "CameraComponent.h"

#include "../Core/BaseEntity.h"

namespace NerdThings::Ngine::Components {
    // Private Methods

    void CameraComponent::UpdateCamera(EntityPositionChangedEventArgs &e) {
        // Update target
        _Camera.Target = e.EntityPosition;
    }

    // Destructor

    CameraComponent::~CameraComponent() {
        _PosChangeEvent.UnBind();
    }

    // Public Constructors

    CameraComponent::CameraComponent(Core::BaseEntity *parent_, float zoom_, TVector2 origin_, float rotation_)
        : Component(parent_) {
        // Check our parent is valid
        if (parent_ == nullptr) {
            // TODO: Maybe add as a general error in component constructor
            throw std::runtime_error("A valid parent must be attached to this component.");
        }

        auto par = GetParent<Core::BaseEntity>();

        // Setup camera
        _Camera = TCamera(rotation_, zoom_, par->GetPosition(), origin_);;

        // Attach to on position changed
        _PosChangeEvent = par->OnPositionChanged.Bind(this, &CameraComponent::UpdateCamera);
    }

    // Public Methods

    void CameraComponent::Activate() {
        GetParent<Core::BaseEntity>()->ParentScene->SetActiveCamera(&_Camera);
    }

    void CameraComponent::SetOrigin(TVector2 origin_) {
        _Camera.Origin = origin_;
    }

    void CameraComponent::SetRotation(float rotation_) {
        _Camera.Rotation = rotation_;
    }

    void CameraComponent::SetZoom(float zoom_) {
        _Camera.Zoom = zoom_;
    }
}
