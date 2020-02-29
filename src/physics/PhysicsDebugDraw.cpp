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

#include "ngine/graphics/ShapeRenderer.hpp"

#include <box2d/box2d.h>

namespace ngine::physics {
    PhysicsDebugDraw::PhysicsDebugDraw(PhysicsWorld *world)
            : m_world(world), m_context(m_world->getContext()) {}

    void PhysicsDebugDraw::setRenderer(graphics::Renderer *renderer) {
        m_renderer = renderer;
    }

    void PhysicsDebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
        // Draw segments
        DrawSegment(vertices[vertexCount - 1], vertices[0], color);
        for (auto i = 0; i < vertexCount - 1; i++) {
            DrawSegment(vertices[i], vertices[i + 1], color);
        }
    }

    void PhysicsDebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
        // Convert vertices
        std::vector<Vector2> verts;
        verts.reserve(vertexCount);
        for (auto i = 0; i < vertexCount; i++) {
            verts.push_back(m_context->convertMetersToPixels(Vector2(vertices[i].x, vertices[i].y)));
        }

        // Draw
        graphics::Color fill = {color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, color.a * 0.5f};
        graphics::ShapeRenderer::drawPolygon(m_renderer, Vector2::Zero, verts, fill);
    }

    void PhysicsDebugDraw::DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) {
        auto c = m_context->convertMetersToPixels(Vector2(center.x, center.y));
        auto r = m_context->convertMetersToPixels(radius);
        graphics::ShapeRenderer::drawCircle(m_renderer, c, r,
                                            graphics::Color(color.r, color.g, color.b, color.a), true);
    }

    void
    PhysicsDebugDraw::DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) {
        auto c = m_context->convertMetersToPixels(Vector2(center.x, center.y));
        auto r = m_context->convertMetersToPixels(radius);
        graphics::Color fill = {color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, color.a * 0.5f};
        graphics::ShapeRenderer::drawCircle(m_renderer, c, r, fill);

        // Axis line
        auto p = c + Vector2(axis.x, axis.y) * r;
        graphics::ShapeRenderer::drawLine(m_renderer, c, p, {color.r, color.g, color.b, color.a});
    }

    void PhysicsDebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
        // Correct
        auto v1 = m_context->convertMetersToPixels(Vector2(p1.x, p1.y));
        auto v2 = m_context->convertMetersToPixels(Vector2(p2.x, p2.y));
        graphics::ShapeRenderer::drawLine(m_renderer, v1, v2, {color.r, color.g, color.b, color.a});
    }

    void PhysicsDebugDraw::DrawTransform(const b2Transform &xf) {
        b2Vec2 p1 = xf.p, p2;
        const float k_axisScale = 1;

        p2 = p1 + k_axisScale * xf.q.GetXAxis();
        DrawSegment(p1, p2, b2Color(1,0,0));

        p2 = p1 + k_axisScale * xf.q.GetYAxis();
        DrawSegment(p1,p2,b2Color(0,1,0));
    }

    void PhysicsDebugDraw::DrawPoint(const b2Vec2 &p, float size, const b2Color &color) {
        // Just draw a rectangle
        auto pos = m_context->convertMetersToPixels(Vector2(p.x, p.y));
        auto s = m_context->convertMetersToPixels(size);
        Rectangle rect(pos - s / 2.0f, s, s);
        graphics::ShapeRenderer::drawRectangle(m_renderer, rect, {color.r, color.g, color.b, color.a});
    }
}
