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

#include "Graphics/Renderer.h"
#include "../Physics/BoundingBox.h"
#include "../Physics/Polygon.h"
#include "../Vector2.h"
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
        Physics::Polygon _Polygon;

        /*
         * Polygon vertices
         */
        std::vector<Vector2> _Vertices;

        // Private Methods

        bool CollisionCheck(BaseCollisionShapeComponent *b) override {
            auto col = false;

            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);
            auto circle = dynamic_cast<CircleCollisionShapeComponent*>(b);
            auto polygon = dynamic_cast<PolygonCollisionShapeComponent*>(b);

            if (bbox != nullptr) {
                auto box = bbox->GetBoundingBox();
                col = CollisionCheck(&box);
            }

            if (circle != nullptr) {
                auto c = circle->GetCircle();
                col = CollisionCheck(&c);
            }

            if (polygon != nullptr) {
                col = CollisionCheck(&polygon->_Polygon);
            }

            return col;
        }

        bool CollisionCheck(Physics::ICollisionShape *b) override {
            return _Polygon.CheckCollision(b);
        }

        void DrawDebug() override {
            // Determine color
            auto col = Graphics::Color::Red;
            if (CheckCollision<BaseEntity>())
                col = Graphics::Color::Green;

            // Draw every vertex of the polygon
            Graphics::Renderer::DrawLine(_Polygon.Vertices[0], _Polygon.Vertices[1], col);
            for (auto i = 1; i < _Polygon.VertexCount - 1; i++) {
                Graphics::Renderer::DrawLine(_Polygon.Vertices[i], _Polygon.Vertices[i + 1], col);
            }
            Graphics::Renderer::DrawLine(_Polygon.Vertices[_Polygon.VertexCount - 1], _Polygon.Vertices[0], col);
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

        void Offset(Vector2 offset_) override {
            const auto par = GetParent<BaseEntity>();

            // Offset vertices
            std::vector<Vector2> vertices(_Vertices.size());
            for (auto i = 0; i < _Vertices.size(); i++) {
                vertices[i] = _Vertices[i];
                vertices[i] = vertices[i].Rotate(par->GetOrigin(), par->GetRotation());
                vertices[i] += par->GetPosition() - par->GetOrigin() + offset_;
            }
            _Polygon = Physics::Polygon(vertices);
        }

        void UpdateShape(EntityTransformChangedEventArgs &e) override {
            std::vector<Vector2> vertices(_Vertices.size());
            for (auto i = 0; i < _Vertices.size(); i++) {
                vertices[i] = _Vertices[i];
                vertices[i] = vertices[i].Rotate(e.EntityOrigin, e.EntityRotation);
                vertices[i] += e.EntityPosition - e.EntityOrigin;
            }
            _Polygon = Physics::Polygon(vertices);
        }

    public:

        // Public Constructor(s)

        PolygonCollisionShapeComponent(BaseEntity *parent_, std::vector<Vector2> vertices_,
                                       std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)), _Vertices(std::move(vertices_)) {
            const auto par = GetParent<BaseEntity>();

            // Create polygon
            SetPolygon(Physics::Polygon(_Vertices));
        }

        PolygonCollisionShapeComponent(BaseEntity *parent_, Physics::Polygon polygon_,
                                       std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)) {
            const auto par = GetParent<BaseEntity>();

            SetPolygon(std::move(polygon_));
        }

        // Public Methods

        Physics::Polygon GetPolygon() const {
            return _Polygon;
        }

        std::vector<Vector2> GetSourceVertices() {
            return _Vertices;
        }

        void SetPolygon(const Physics::Polygon &polygon_) {
            const auto par = GetParent<BaseEntity>();

            // Grab vertices
            _Vertices.resize(polygon_.VertexCount);
            for (auto i = 0; i < polygon_.VertexCount; i++) {
                _Vertices[i] = polygon_.Vertices[i];
            }

            std::vector<Vector2> vertices(_Vertices.size());
            for (auto i = 0; i < _Vertices.size(); i++) {
                vertices[i] = _Vertices[i];
                vertices[i] = vertices[i].Rotate(par->GetOrigin(), par->GetRotation());
                vertices[i] += par->GetPosition() - par->GetOrigin();
            }
            _Polygon = Physics::Polygon(vertices);
        }
    };
}

#endif //POLYGONCOLLISIONSHAPECOMPONENT_H
