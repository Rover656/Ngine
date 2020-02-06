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

#include "physics/shapes/CircleShape.hpp"

#include "graphics/Renderer.hpp"
#include "graphics/ShapeRenderer.hpp"

#include <Box2D/Collision/Shapes/b2CircleShape.h>

namespace ngine::physics::shapes {
    // Public Constructors

    CircleShape::CircleShape() : PhysicsShape() {
        _Shape = new b2CircleShape();
    }

    CircleShape::CircleShape(float radius_, Vector2 position_) : CircleShape() {
        setRadius(radius_);
        setPosition(position_);
    }

    CircleShape::CircleShape(b2CircleShape *circle_) : PhysicsShape(circle_) {}

    CircleShape::CircleShape(CircleShape &&old) : PhysicsShape(std::move(old)) {}

    CircleShape::CircleShape(const CircleShape &circle_)
            : PhysicsShape(circle_) {
        // Create my shape
        _Shape = new b2CircleShape();

        // Copy data
        setRadius(circle_.getRadius());
        setPosition(circle_.getPosition());
    }

    // Public Methods

    void CircleShape::debugDraw(graphics::Renderer *renderer_, float ppm_, Vector2 pos_, float rotation_) const {
        auto pos = getPosition();
        pos *= ppm_;
        pos += pos_;

        graphics::ShapeRenderer::DrawCircle(renderer_, pos, getRadius() * ppm_, graphics::Color::Red, true);
    }

    Vector2 CircleShape::getPosition() const {
        auto p = ((b2CircleShape *)_Shape)->m_p;
        return {p.x, p.y};
    }

    void CircleShape::setPosition(Vector2 pos_) {
        ((b2CircleShape *) _Shape)->m_p = {pos_.X, pos_.Y};
    }
}