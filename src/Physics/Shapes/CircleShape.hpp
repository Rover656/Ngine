/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef CIRCLESHAPE_HPP
#define CIRCLESHAPE_HPP

#include "../../Config.hpp"

#include "PhysicsShape.hpp"

class b2CircleShape;

namespace NerdThings::Ngine::Physics::Shapes {
    class NEAPI CircleShape : public PhysicsShape {
    public:
        // Public Constructors

        /*
         * Create a circle shape.
         */
        CircleShape();

        /*
         * Create a circle shape.
         */
        explicit CircleShape(float radius_, Vector2 position_ = Vector2::Zero);

        /*
         * Create a circle shape from a Box2D owned circle.
         */
        explicit CircleShape(b2CircleShape *circle_);

        /*
         * Move constructor
         */
        CircleShape(CircleShape &&old);

        /*
         * Copy constructor
         */
        CircleShape(const CircleShape &circle_);

        // Public Methods

        void DebugDraw(float ppm_, Vector2 pos_, float angle_) const override;

        /*
         * Get circle position
         */
        Vector2 GetPosition() const;

        /*
         * Set circle position
         */
        void SetPosition(Vector2 pos_);
    };
}

#endif //CIRCLESHAPE_HPP
