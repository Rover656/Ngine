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
     * Bounding Box Component
     */
    class BoundingBoxCollisionShapeComponent : public BaseCollisionShapeComponent {
        // Private Fields

        /*
         * The bounding box used
         */
        TBoundingBox _BoundingBox;

        /*
         * Bounding box height
         */
        float _Height;

        /*
         * The offset
         */
        TVector2 _Offset;

        /*
         * Bounding box width
         */
        float _Width;

        // Private Methods

        bool CollisionCheck(BaseCollisionShapeComponent *b) override {
            if (this == b) return false;

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

            Graphics::Drawing::DrawRectangleLines(par->GetPosition(), _Width, _Height, col, 1);
        }

        void Offset(TVector2 offset_) override {
            _BoundingBox.Offset(offset_);
        }

        void UpdateShape(EntityPositionChangedEventArgs &e) override {
            auto par = GetParent<Core::BaseEntity>();
            _BoundingBox = TRectangle(par->GetPosition() + _Offset, _Width, _Height).ToBoundingBox(0);
        }
    public:

        // Public Constructors

        /*
         * Create a bounding box component
         */
        BoundingBoxCollisionShapeComponent(Core::BaseEntity *parent_, float width_, float height_, TVector2 offset_ = TVector2::Zero, std::string collisionGroup_ = "General")
            : BaseCollisionShapeComponent(parent_, collisionGroup_), _Height(height_), _Offset(offset_), _Width(width_) {
            auto par = GetParent<Core::BaseEntity>();

            _BoundingBox = TRectangle(par->GetPosition() + _Offset, _Width, _Height).ToBoundingBox(0);
        }
    };
}

#endif //BOUNDINGBOXCOLLISIONSHAPECOMPONENT_H
