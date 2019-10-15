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

#ifndef BOUNDINGBOXCOLLISIONSHAPECOMPONENT_H
#define BOUNDINGBOXCOLLISIONSHAPECOMPONENT_H

#include "../Ngine.h"

#include "../Graphics/Color.h"
#include "Graphics/Renderer.h"
#include "../Rectangle.h"
#include "BaseCollisionShapeComponent.h"

namespace NerdThings::Ngine::Components {
    /*
     * Bounding Box Collider Component
     */
    class BoundingBoxCollisionShapeComponent : public BaseCollisionShapeComponent {
        // Private Fields

        /*
         * The bounding box used
         */
        Physics::BoundingBox _BoundingBox;

        /*
         * The rectangle to use
         */
        Rectangle _Rectangle;

        // Private Methods

        bool CollisionCheck(BaseCollisionShapeComponent *b) override {
            auto col = false;

            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);

            if (bbox != nullptr) {
                //bbox col
                col = CollisionCheck(&bbox->_BoundingBox);
            }

            return col;
        }

        bool CollisionCheck(Physics::ICollisionShape *b) override {
            return _BoundingBox.CheckCollision(b);
        }

        void DrawDebug() override {
            auto par = GetParent<BaseEntity>();

            // Determine color
            auto col = Graphics::Color::Red;
            if (CheckCollision<BaseEntity>())
                col = Graphics::Color::Green;

            // Draw each vertex of the bounding box
            Graphics::Renderer::DrawLine(_BoundingBox.Min, {_BoundingBox.Max.X, _BoundingBox.Min.Y}, col);
            Graphics::Renderer::DrawLine({_BoundingBox.Max.X, _BoundingBox.Min.Y}, _BoundingBox.Max, col);
            Graphics::Renderer::DrawLine(_BoundingBox.Max, {_BoundingBox.Min.X, _BoundingBox.Max.Y}, col);
            Graphics::Renderer::DrawLine({_BoundingBox.Min.X, _BoundingBox.Max.Y}, _BoundingBox.Min, col);

            // Extra to understand how bounding boxes work
            if (DebugDrawInternalRectangle)
                Graphics::Renderer::DrawRectangle(
                        Rectangle(par->GetPosition() + Vector2(_Rectangle.X, _Rectangle.Y), _Rectangle.Width,
                                  _Rectangle.Height), Graphics::Color::Orange, par->GetRotation(),
                        par->GetOrigin());
        }

        bool IsCompatible(BaseCollisionShapeComponent *b) override {
            // Due to how we laid out things, we can only check against other bounding boxes
            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);
            if (bbox != nullptr)
                return true;
            return false;
        }

        void Offset(Vector2 offset_) override {
            // Recreate with a positional offset
            auto par = GetParent<BaseEntity>();
            _BoundingBox = Rectangle(
                    par->GetPosition() - par->GetOrigin() + offset_ + Vector2(_Rectangle.X, _Rectangle.Y),
                _Rectangle.Width, _Rectangle.Height).ToBoundingBox(par->GetRotation(), par->GetOrigin());
        }

        void UpdateShape(EntityTransformChangedEventArgs &e) override {
            // Recreate with new information
            _BoundingBox = Rectangle(
                    e.EntityPosition - e.EntityOrigin + Vector2(_Rectangle.X, _Rectangle.Y),
                _Rectangle.Width, _Rectangle.Height).ToBoundingBox(
                e.EntityRotation, e.EntityOrigin);
        }

    public:

        // Public Fields

        /*
         * Whether or not to draw the internal rectangle
         */
        bool DebugDrawInternalRectangle = true;

        // Public Constructors

        /*
         * Create a bounding box component
         */
        BoundingBoxCollisionShapeComponent(BaseEntity *parent_, Rectangle rectangle_,
                                           std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)), _Rectangle(rectangle_) {
            const auto par = GetParent<BaseEntity>();
            SetRectangle(rectangle_);
        }

        // Public Methods

        /*
         * Get the bounding box
         */
        [[nodiscard]] Physics::BoundingBox GetBoundingBox() const {
            return _BoundingBox;
        }

        /*
         * Set the bounding box height
         */
        void SetRectangle(Rectangle rectangle_) {
            const auto par = GetParent<BaseEntity>();
            _Rectangle = rectangle_;
            _BoundingBox = Rectangle(
                    par->GetPosition() - par->GetOrigin() + Vector2(_Rectangle.X, _Rectangle.Y),
                _Rectangle.Width, _Rectangle.Height).ToBoundingBox(
                par->GetRotation(), par->GetOrigin());
        }
    };
}

#endif //BOUNDINGBOXCOLLISIONSHAPECOMPONENT_H
