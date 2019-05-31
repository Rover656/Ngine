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

#ifndef POLYGONCOLLISIONSHAPECOMPONENT_H
#define POLYGONCOLLISIONSHAPECOMPONENT_H

#include "../ngine.h"

#include "../Graphics/Drawing.h"
#include "../Types/BoundingBox.h"
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
        TPolygon _Polygon;

        /*
         * Polygon vertices
         */
        std::vector<TVector2> _Vertices;

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
            // Draw rectangle
            auto col = TColor::Red;
            if (CheckCollision<Core::BaseEntity>())
                col = TColor::Green;

            Graphics::Drawing::DrawLine(_Polygon.Vertices[0], _Polygon.Vertices[1], col);
            for (auto i = 1; i < _Polygon.VertexCount - 1; i++) {
                Graphics::Drawing::DrawLine(_Polygon.Vertices[i], _Polygon.Vertices[i + 1], col);
            }
            Graphics::Drawing::DrawLine(_Polygon.Vertices[_Polygon.VertexCount - 1], _Polygon.Vertices[0], col);
        }

        bool IsCompatible(BaseCollisionShapeComponent *b) override {
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

        void Offset(TVector2 offset_) override {
            _Polygon.Offset(offset_);
        }

        void UpdateShape(EntityTransformChangedEventArgs &e) override {
            SetPolygon(TPolygon(_Vertices));
        }

    public:

        // Public Constructor(s)

        PolygonCollisionShapeComponent(Core::BaseEntity *parent_, std::vector<TVector2> vertices_,
                                       std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)), _Vertices(vertices_) {
            const auto par = GetParent<Core::BaseEntity>();

            // Create polygon
            SetPolygon(TPolygon(_Vertices));
        }

        PolygonCollisionShapeComponent(Core::BaseEntity *parent_, TPolygon polygon_,
                                       std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)) {
            const auto par = GetParent<Core::BaseEntity>();

            SetPolygon(polygon_);
        }

        // Public Methods

        TPolygon GetPolygon() {
            return _Polygon;
        }

        void SetPolygon(TPolygon polygon_) {
            const auto par = GetParent<Core::BaseEntity>();

            // Grab vertices
            _Vertices.resize(polygon_.VertexCount);
            for (auto i = 0; i < polygon_.VertexCount; i++) {
                _Vertices[i] = polygon_.Vertices[i];
            }

            std::vector<TVector2> vertices(_Vertices.size());
            for (auto i = 0; i < _Vertices.size(); i++) {
                vertices[i] = _Vertices[i];
                vertices[i] = vertices[i].Rotate(par->GetOrigin(), par->GetRotation());
                vertices[i] += par->GetPosition();
            }
            _Polygon = TPolygon(vertices);
        }
    };
}

#endif //POLYGONCOLLISIONSHAPECOMPONENT_H
