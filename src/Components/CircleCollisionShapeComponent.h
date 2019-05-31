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

#ifndef CIRCLECOLLISIONSHAPECOMPONENT_H
#define CIRCLECOLLISIONSHAPECOMPONENT_H

#include <utility>
#include "../ngine.h"

#include "../Graphics/Drawing.h"
#include "../Types/BoundingBox.h"
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
        TCircle _Circle;

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
                col = _Circle.CheckCollision(&bboxCol);
            } else if (circle != nullptr) {
                // Circle collision
                col = _Circle.CheckCollision(&circle->_Circle);
            }

            return col;
        }

        void DrawDebug() override {
            auto par = GetParent<Core::BaseEntity>();

            // Draw circle
            auto col = TColor::Red;
            if (CheckCollision<Core::BaseEntity>())
                col = TColor::Green;

            Graphics::Drawing::DrawCircleLines(_Circle.Center, _Circle.Radius, col);
        }

        bool IsCompatible(BaseCollisionShapeComponent *b) override {
            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);
            auto circle = dynamic_cast<CircleCollisionShapeComponent*>(b);

            if (bbox != nullptr)
                return true;
            if (circle != nullptr)
                return true;
            return false;
        }

        void Offset(TVector2 offset_) override {
            _Circle.Offset(offset_);
        }

        void UpdateShape(EntityTransformChangedEventArgs &e) override {
            SetRadius(_Radius);
        }

    public:
        // Public Constructor(s)

        CircleCollisionShapeComponent(Core::BaseEntity *parent_, float radius_, std::string collisionGroup_= "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)), _Radius(radius_) {
            const auto par = GetParent<Core::BaseEntity>();

            // Create circle
            SetRadius(_Radius);
        }

        // Public Methods

        TCircle GetCircle() const {
            return _Circle;
        }

        void SetRadius(float radius_) {
            const auto par = GetParent<Core::BaseEntity>();
            _Radius = radius_;
            _Circle = TCircle(par->GetPosition(), _Radius);
        }
    };
}

#endif //CIRCLECOLLISIONSHAPECOMPONENT_H
