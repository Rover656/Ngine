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

#include "Components/CameraComponent.hpp"

#include "Entity.hpp"

namespace Ngine::Components {
    void CameraComponent::_updateCamera(EntityTransformChangedEventArgs e) {
        // Update the target
        m_camera.Position = e.EntityTransformation.Position;
    }

    CameraComponent::~CameraComponent() {
        m_transformChangeEvent.Detach();
    }

    CameraComponent::CameraComponent(Entity *parent_, const float zoom_, const Vector2 origin_,
                                     const float rotation_)
        : Component(parent_) {
        auto par = GetEntity < Entity > ();

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
