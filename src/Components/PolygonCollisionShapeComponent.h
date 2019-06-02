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
*       May require further review at a later date
*
**********************************************************************************************/

#ifndef POLYGONCOLLISIONSHAPECOMPONENT_H
#define POLYGONCOLLISIONSHAPECOMPONENT_H

#include "../ngine.h"

#include "../Graphics/Drawing.h"
#include "../Physics/BoundingBox.h"
#include "../Physics/Polygon.h"
#include "../Math/Vector2.h"
#include "BaseCollisionShapeComponent.h"
#include "CircleCollisionShapeComponent.h"

namespace NerdThings::Ngine::Components {
    /*
     * Polygon collider component
     */
    class PolygonCollisionShapeComponent : public BaseCollisionShapeComponent {
        // Private Fields

        /*
         * The polygon used
         */
        Physics::TPolygon _Polygon;

        /*
         * Polygon vertices
         */
        std::vector<Math::TVector2> _Vertices;

        // Private Methods

        bool CollisionCheck(BaseCollisionShapeComponent *b) override {
            auto col = false;

            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);
            auto circle = dynamic_cast<CircleCollisionShapeComponent*>(b);
            auto polygon = dynamic_cast<PolygonCollisionShapeComponent*>(b);

            if (bbox != nullptr) {
                auto box = bbox->GetBoundingBox();
                col = _Polygon.CheckCollision(&box);
            }

            if (circle != nullptr) {
                auto c = circle->GetCircle();
                col = _Polygon.CheckCollision(&c);
            }

            if (polygon != nullptr) {
                col = _Polygon.CheckCollision(&polygon->_Polygon);
            }

            return col;
        }

        void DrawDebug() override {
            // Determine color
            auto col = Graphics::TColor::Red;
            if (CheckCollision<Core::BaseEntity>())
                col = Graphics::TColor::Green;

            // Draw every vertex of the polygon
            Graphics::Drawing::DrawLine(_Polygon.Vertices[0], _Polygon.Vertices[1], col);
            for (auto i = 1; i < _Polygon.VertexCount - 1; i++) {
                Graphics::Drawing::DrawLine(_Polygon.Vertices[i], _Polygon.Vertices[i + 1], col);
            }
            Graphics::Drawing::DrawLine(_Polygon.Vertices[_Polygon.VertexCount - 1], _Polygon.Vertices[0], col);
        }

        bool IsCompatible(BaseCollisionShapeComponent *b) override {
            // We handle collisions with bounding boxes, circles and ourselves
            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);
            auto circle = dynamic_cast<CircleCollisionShapeComponent*>(b);
            auto polygon = dynamic_cast<PolygonCollisionShapeComponent*>(b);
            if (bbox != nullptr)
                return true;
            if (circle != nullptr)
                return true;
            if (polygon != nullptr)
                return true;

            return false;
        }

        void Offset(Math::TVector2 offset_) override {
            // This could be expensive in some cases, so only do it if necessary
            if (offset_.X != 0 && offset_.Y != 0) {
                const auto par = GetParent<Core::BaseEntity>();

                // Offset vertices
                std::vector<Math::TVector2> vertices(_Vertices.size());
                for (auto i = 0; i < _Vertices.size(); i++) {
                    vertices[i] = _Vertices[i];
                    vertices[i] = vertices[i].Rotate(par->GetOrigin(), par->GetRotation());
                    vertices[i] += par->GetPosition() - par->GetOrigin() + offset_;
                }
                _Polygon = Physics::TPolygon(vertices);
            }
        }

        void UpdateShape(EntityTransformChangedEventArgs &e) override {
            std::vector<Math::TVector2> vertices(_Vertices.size());
            for (auto i = 0; i < _Vertices.size(); i++) {
                vertices[i] = _Vertices[i];
                vertices[i] = vertices[i].Rotate(e.EntityOrigin, e.EntityRotation);
                vertices[i] += e.EntityPosition - e.EntityOrigin;
            }
            _Polygon = Physics::TPolygon(vertices);
        }

    public:

        // Public Constructor(s)

        PolygonCollisionShapeComponent(Core::BaseEntity *parent_, std::vector<Math::TVector2> vertices_,
                                       std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)), _Vertices(std::move(vertices_)) {
            const auto par = GetParent<Core::BaseEntity>();

            // Create polygon
            SetPolygon(Physics::TPolygon(_Vertices));
        }

        PolygonCollisionShapeComponent(Core::BaseEntity *parent_, Physics::TPolygon polygon_,
                                       std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)) {
            const auto par = GetParent<Core::BaseEntity>();

            SetPolygon(std::move(polygon_));
        }

        // Public Methods

        Physics::TPolygon GetPolygon() const {
            return _Polygon;
        }

        void SetPolygon(const Physics::TPolygon &polygon_) {
            const auto par = GetParent<Core::BaseEntity>();

            // Grab vertices
            _Vertices.resize(polygon_.VertexCount);
            for (auto i = 0; i < polygon_.VertexCount; i++) {
                _Vertices[i] = polygon_.Vertices[i];
            }

            std::vector<Math::TVector2> vertices(_Vertices.size());
            for (auto i = 0; i < _Vertices.size(); i++) {
                vertices[i] = _Vertices[i];
                vertices[i] = vertices[i].Rotate(par->GetOrigin(), par->GetRotation());
                vertices[i] += par->GetPosition() - par->GetOrigin();
            }
            _Polygon = Physics::TPolygon(vertices);
        }
    };
}

#endif //POLYGONCOLLISIONSHAPECOMPONENT_H
