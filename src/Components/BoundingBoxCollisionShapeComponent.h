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

#ifndef BOUNDINGBOXCOLLISIONSHAPECOMPONENT_H
#define BOUNDINGBOXCOLLISIONSHAPECOMPONENT_H

#include "../ngine.h"

#include "../Graphics/Drawing.h"
#include "../Types/BoundingBox.h"
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
        TBoundingBox _BoundingBox;

        /*
         * The rectangle to use
         */
        TRectangle _Rectangle;

        // Private Methods

        bool CollisionCheck(BaseCollisionShapeComponent *b) override {
            auto col = false;

            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);

            if (bbox != nullptr) {
                //bbox col
                col = _BoundingBox.CheckCollision(&bbox->_BoundingBox);
            }

            return col;
        }

        void DrawDebug() override {
            auto par = GetParent<Core::BaseEntity>();

            // Determine color
            auto col = TColor::Red;
            if (CheckCollision<Core::BaseEntity>())
                col = TColor::Green;

            // Draw each vertex of the bounding box
            Graphics::Drawing::DrawLine(_BoundingBox.Min, {_BoundingBox.Max.X, _BoundingBox.Min.Y}, col);
            Graphics::Drawing::DrawLine({_BoundingBox.Max.X, _BoundingBox.Min.Y}, _BoundingBox.Max, col);
            Graphics::Drawing::DrawLine(_BoundingBox.Max, {_BoundingBox.Min.X, _BoundingBox.Max.Y}, col);
            Graphics::Drawing::DrawLine({_BoundingBox.Min.X, _BoundingBox.Max.Y}, _BoundingBox.Min, col);

            // Extra to understand how bounding boxes work
            if (DebugDrawInternalRectangle)
                Graphics::Drawing::DrawRectangle(
                    TRectangle(par->GetPosition() + TVector2(_Rectangle.X, _Rectangle.Y), _Rectangle.Width,
                               _Rectangle.Height), TColor::Orange, par->GetRotation(), par->GetOrigin());
        }

        bool IsCompatible(BaseCollisionShapeComponent *b) override {
            // Due to how we laid out things, we can only check against other bounding boxes
            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);
            if (bbox != nullptr)
                return true;
            return false;
        }

        void Offset(TVector2 offset_) override {
            // Recreate with a positional offset
            auto par = GetParent<Core::BaseEntity>();
            _BoundingBox = TRectangle(
                par->GetPosition() - par->GetOrigin() + offset_ + TVector2(_Rectangle.X, _Rectangle.Y),
                _Rectangle.Width, _Rectangle.Height).ToBoundingBox(par->GetRotation(), par->GetOrigin());
        }

        void UpdateShape(EntityTransformChangedEventArgs &e) override {
            // Recreate with new information
            _BoundingBox = TRectangle(e.EntityPosition - e.EntityOrigin + TVector2(_Rectangle.X, _Rectangle.Y),
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
        BoundingBoxCollisionShapeComponent(Core::BaseEntity *parent_, TRectangle rectangle_,
                                           std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)), _Rectangle(rectangle_) {
            const auto par = GetParent<Core::BaseEntity>();
            SetRectangle(rectangle_);
        }

        // Public Methods

        /*
         * Get the bounding box
         */
        [[nodiscard]] TBoundingBox GetBoundingBox() const {
            return _BoundingBox;
        }

        /*
         * Set the bounding box height
         */
        void SetRectangle(TRectangle rectangle_) {
            const auto par = GetParent<Core::BaseEntity>();
            _Rectangle = rectangle_;
            _BoundingBox = TRectangle(par->GetPosition() - par->GetOrigin() + TVector2(_Rectangle.X, _Rectangle.Y),
                                      _Rectangle.Width, _Rectangle.Height).ToBoundingBox(
                par->GetRotation(), par->GetOrigin());
        }
    };
}

#endif //BOUNDINGBOXCOLLISIONSHAPECOMPONENT_H
