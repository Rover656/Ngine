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

#include "CircleShape.hpp"

#include "../../Graphics/Renderer.hpp"

#include <Box2D/Collision/Shapes/b2CircleShape.h>

namespace NerdThings::Ngine::Physics::Shapes {
    // Public Constructors

    CircleShape::CircleShape() : PhysicsShape() {
        _Shape = new b2CircleShape();
    }

    CircleShape::CircleShape(float radius_, Vector2 position_) : CircleShape() {
        SetRadius(radius_);
        SetPosition(position_);
    }

    CircleShape::CircleShape(b2CircleShape *circle_) : PhysicsShape(circle_) {}

    CircleShape::CircleShape(CircleShape &&old) : PhysicsShape(std::move(old)) {}

    CircleShape::CircleShape(const CircleShape &circle_)
            : PhysicsShape(circle_) {
        // Create my shape
        _Shape = new b2CircleShape();

        // Copy data
        SetRadius(circle_.GetRadius());
        SetPosition(circle_.GetPosition());
    }

    // Public Methods

    void CircleShape::DebugDraw(Graphics::Renderer *renderer_, float ppm_, Vector2 pos_, float rotation_) const {
        auto pos = GetPosition();
        pos *= ppm_;
        pos += pos_;
#ifndef USE_EXPERIMENTAL_RENDERER
        renderer_->DrawCircleLines(pos, GetRadius() * ppm_, Graphics::Color::Red);
#endif
    }

    Vector2 CircleShape::GetPosition() const {
        auto p = ((b2CircleShape *)_Shape)->m_p;
        return {p.x, p.y};
    }

    void CircleShape::SetPosition(Vector2 pos_) {
        ((b2CircleShape *) _Shape)->m_p = {pos_.X, pos_.Y};
    }
}