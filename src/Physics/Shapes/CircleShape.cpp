/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#include "CircleShape.hpp"

#include "../../Graphics/Renderer.hpp"
#include "../../Graphics/ShapeRenderer.hpp"

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

        Graphics::ShapeRenderer::DrawCircle(renderer_, pos, GetRadius() * ppm_, Graphics::Color::Red, true);
    }

    Vector2 CircleShape::GetPosition() const {
        auto p = ((b2CircleShape *)_Shape)->m_p;
        return {p.x, p.y};
    }

    void CircleShape::SetPosition(Vector2 pos_) {
        ((b2CircleShape *) _Shape)->m_p = {pos_.X, pos_.Y};
    }
}