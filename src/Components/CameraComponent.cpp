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

#include "CameraComponent.h"

#include "../Graphics/Camera.h"
#include "../Entity.h"

namespace NerdThings::Ngine::Components {
    void CameraComponent::_updateCamera(EntityTransformChangedEventArgs e) {
        // Update the target
        m_camera.Position = e.EntityPosition;
    }

    CameraComponent::~CameraComponent() {
        m_transformChangeEvent.Detach();
    }

    CameraComponent::CameraComponent(Entity *parent_, const float zoom_, const Vector2 origin_,
                                     const float rotation_)
        : Component(parent_) {
        auto par = GetParent < Entity > ();

        // Setup camera
        m_camera = Graphics::Camera(par->GetPosition(), rotation_, zoom_, origin_);;

        // Attach to on position changed
        m_transformChangeEvent = par->OnTransformChanged +=
                new ClassMethodEventHandler<CameraComponent, EntityTransformChangedEventArgs>(this,
                                                                                              &CameraComponent::_updateCamera);
    }

    void CameraComponent::Activate() {
        GetScene()->SetActiveCamera(&m_camera);
    }

    Vector2 CameraComponent::GetOrigin() {
        return m_camera.Origin;
    }

    void CameraComponent::SetOrigin(const Vector2 origin_) {
        m_camera.Origin = origin_;
    }

    float CameraComponent::GetRotation() {
        return m_camera.Rotation;
    }

    void CameraComponent::SetRotation(const float rotation_) {
        m_camera.Rotation = rotation_;
    }

    float CameraComponent::GetZoom() {
        return m_camera.Zoom;
    }

    void CameraComponent::SetZoom(const float zoom_) {
        m_camera.Zoom = zoom_;
    }
}
