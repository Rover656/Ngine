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

#include "PhysicsDebugDraw.hpp"

#include "graphics/ShapeRenderer.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics {
    PhysicsDebugDraw::PhysicsDebugDraw(PhysicsWorld *world)
            : m_world(world) {}

    void PhysicsDebugDraw::setRenderer(graphics::Renderer *renderer) {
        m_renderer = renderer;
    }

    void PhysicsDebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
        // TODO.
    }

    void PhysicsDebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
        // TODO.
    }

    void PhysicsDebugDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) {
        auto c = m_world->convertMetersToPixels(Vector2(center.x, center.y));
        auto r = m_world->convertMetersToPixels(radius);
        graphics::ShapeRenderer::DrawCircle(m_renderer, c, r,
                                            graphics::Color(color.r, color.g, color.b, color.a), true);
    }

    void
    PhysicsDebugDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) {
        auto c = m_world->convertMetersToPixels(Vector2(center.x, center.y));
        auto r = m_world->convertMetersToPixels(radius);
        graphics::ShapeRenderer::DrawCircle(m_renderer, {center.x, center.y}, r,
                                            graphics::Color(color.r, color.g, color.b, color.a));

        // Axis line
        auto p = c + Vector2(axis.x, axis.y) * r;
        graphics::ShapeRenderer::DrawLine(m_renderer, c, p, {color.r, color.g, color.b, color.a});
    }

    void PhysicsDebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
        auto v1 = m_world->convertMetersToPixels(Vector2(p1.x, p1.y));
        auto v2 = m_world->convertMetersToPixels(Vector2(p2.x, p2.y));
        graphics::ShapeRenderer::DrawLine(m_renderer, v1, v2, {color.r, color.g, color.b, color.a});
    }

    void PhysicsDebugDraw::DrawTransform(const b2Transform &xf) {

    }

    void PhysicsDebugDraw::DrawPoint(const b2Vec2 &p, float32 size, const b2Color &color) {
        // TODO: We don't actually have a function for this.
    }
}