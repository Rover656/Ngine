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

            // Draw rectangle
            auto col = TColor::Red;
            if (CheckCollision<Core::BaseEntity>())
                col = TColor::Green;

            Graphics::Drawing::DrawLine(_BoundingBox.Min, { _BoundingBox.Max.X, _BoundingBox.Min.Y }, col);
            Graphics::Drawing::DrawLine({ _BoundingBox.Max.X, _BoundingBox.Min.Y }, _BoundingBox.Max, col);
            Graphics::Drawing::DrawLine(_BoundingBox.Max, { _BoundingBox.Min.X, _BoundingBox.Max.Y }, col);
            Graphics::Drawing::DrawLine({ _BoundingBox.Min.X, _BoundingBox.Max.Y }, _BoundingBox.Min, col);
        }

        bool IsCompatible(BaseCollisionShapeComponent *b) override {
            auto bbox = dynamic_cast<BoundingBoxCollisionShapeComponent*>(b);
            if (bbox != nullptr)
                return true;
            return false;
        }

        void Offset(TVector2 offset_) override {
            _BoundingBox.Offset(offset_);
        }

        void UpdateShape(EntityTransformChangedEventArgs &e) override {
            SetRectangle(CreateRectangle());
        }

        TRectangle CreateRectangle() const {
            const auto par = GetParent<Core::BaseEntity>();
            return TRectangle(par->GetPosition() - par->GetOrigin() + TVector2(_Rectangle.X, _Rectangle.Y), _Rectangle.Width, _Rectangle.Height);
        }
    public:

        // Public Constructors

        /*
         * Create a bounding box component
         */
        BoundingBoxCollisionShapeComponent(Core::BaseEntity *parent_, TRectangle rectangle_, std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, std::move(collisionGroup_)), _Rectangle(rectangle_) {
            const auto par = GetParent<Core::BaseEntity>();

            SetRectangle(CreateRectangle());
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
            _BoundingBox = TRectangle(par->GetPosition() - par->GetOrigin() + TVector2(_Rectangle.X, _Rectangle.Y), _Rectangle.Width, _Rectangle.Height).ToBoundingBox(par->GetRotation(), par->GetOrigin());
        }
    };
}

#endif //BOUNDINGBOXCOLLISIONSHAPECOMPONENT_H
