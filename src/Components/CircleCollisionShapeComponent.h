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

#ifndef CIRCLECOLLISIONSHAPECOMPONENT_H
#define CIRCLECOLLISIONSHAPECOMPONENT_H

#include "../ngine.h"

#include "../Graphics/Renderer.h"
#include "../Physics/BoundingBox.h"
#include "../Physics/Circle.h"
#include "../Vector2.h"
#include "BaseCollisionShapeComponent.h"
#include "BoundingBoxCollisionShapeComponent.h"

namespace NerdThings::Ngine::Components {
    /*
     * Circle collider component
     */
    class CircleCollisionShapeComponent : public BaseCollisionShapeComponent {
        // Private Fields

        /*
         * The circle used
         */
        Physics::TCircle _Circle;

        /*
         * The circle radius
         */
        float _Radius;

        // Private Methods

        bool CollisionCheck(BaseCollisionShapeComponent *b) override {
            auto col = false;

            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);
            auto circle = dynamic_cast<CircleCollisionShapeComponent*>(b);

            if (bbox != nullptr) {
                // Bounding box collision
                auto bboxCol = bbox->GetBoundingBox();
                col = CollisionCheck(&bboxCol);
            } else if (circle != nullptr) {
                // Circle collision
                col = CollisionCheck(&circle->_Circle);
            }

            return col;
        }

        bool CollisionCheck(Physics::ICollisionShape *b) override {
            return _Circle.CheckCollision(b);
        }

        void DrawDebug() override {
            auto par = GetParent<BaseEntity>();

            // Determine color
            auto col = Graphics::TColor::Red;
            if (CheckCollision<BaseEntity>())
                col = Graphics::TColor::Green;

            // Draw the circle outline
            Graphics::Renderer::DrawCircleLines(_Circle.Center, _Circle.Radius, col);
        }

        bool IsCompatible(BaseCollisionShapeComponent *b) override {
            // We handle bounding boxes and circles here
            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);
            auto circle = dynamic_cast<CircleCollisionShapeComponent*>(b);

            if (bbox != nullptr)
                return true;
            if (circle != nullptr)
                return true;
            return false;
        }

        void Offset(TVector2 offset_) override {
            const auto par = GetParent<BaseEntity>();

            // Rebuild with offset
            _Circle = Physics::TCircle(par->GetPosition() - par->GetOrigin() + offset_, _Radius);
        }

        void UpdateShape(EntityTransformChangedEventArgs &e) override {
            const auto par = GetParent<BaseEntity>();
            _Circle = Physics::TCircle(par->GetPosition() - par->GetOrigin(), _Radius);
        }

    public:
        // Public Constructor(s)

        CircleCollisionShapeComponent(BaseEntity *parent_, float radius_, std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)), _Radius(radius_) {
            const auto par = GetParent<BaseEntity>();

            // Create circle
            SetRadius(_Radius);
        }

        // Public Methods

        Physics::TCircle GetCircle() const {
            return _Circle;
        }

        void SetRadius(float radius_) {
            const auto par = GetParent<BaseEntity>();
            _Radius = radius_;
            _Circle = Physics::TCircle(par->GetPosition() - par->GetOrigin(), _Radius);
        }
    };
}

#endif //CIRCLECOLLISIONSHAPECOMPONENT_H
