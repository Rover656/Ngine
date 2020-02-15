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

#include "physics/PhysicsShape.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics {
    PhysicsShape::PhysicsShape(const PhysicsContext *context, b2Shape *shape)
            : m_context(context) {
        // Save shape
        m_shape = shape;
    }

    PhysicsShape::~PhysicsShape() {
        // Delete the shape if we allocated it
        if (m_allocatedByUs)
            delete m_shape;
    }

    PhysicsShape::Type PhysicsShape::getType() const {
        switch (m_shape->GetType()) {
            case b2Shape::e_circle:
                return Type::Circle;
            case b2Shape::e_edge:
                return Type::Edge;
            case b2Shape::e_polygon:
                return Type::Polygon;
            case b2Shape::e_chain:
                return Type::Chain;
        }
    }

    int PhysicsShape::getChildCount() const {
        return m_shape->GetChildCount();
    }

    bool PhysicsShape::testPoint(const Transform2D &transform, const Vector2 &pos) {
        // Build transform
        b2Transform t;
        auto tp = m_context->convertPixelsToMeters(transform.Position);
        t.p = {tp.X, tp.Y};
        t.q = b2Rot(DegToRad(transform.Rotation.getDegrees()));

        // Convert position and test point
        auto p = m_context->convertPixelsToMeters(pos);
        return m_shape->TestPoint(t, {p.X, p.Y});
    }

    CirclePhysicsShape::CirclePhysicsShape(const PhysicsContext *context, b2CircleShape *circle)
            : PhysicsShape(context, circle) {}

    CirclePhysicsShape::CirclePhysicsShape(const PhysicsContext *context, float radius, const Vector2 &position)
            : PhysicsShape(context, new b2CircleShape()) {
        // Mark as manually allocated
        m_allocatedByUs = true;

        // Setup
        setRadius(radius);
        setPosition(position);
    }

    float CirclePhysicsShape::getRadius() {
        return m_context->convertMetersToPixels(((b2CircleShape *) m_shape)->m_radius);
    }

    void CirclePhysicsShape::setRadius(float radius) {
        ((b2CircleShape *) m_shape)->m_radius = m_context->convertPixelsToMeters(radius);
    }

    Vector2 CirclePhysicsShape::getPosition() {
        auto p = ((b2CircleShape *) m_shape)->m_p;
        return m_context->convertMetersToPixels(Vector2(p.x, p.y));
    }

    void CirclePhysicsShape::setPosition(const Vector2 &position) {
        auto p = m_context->convertPixelsToMeters(position);
        ((b2CircleShape *) m_shape)->m_p = {p.X, p.Y};
    }

    PolygonPhysicsShape::PolygonPhysicsShape(const PhysicsContext *context, b2PolygonShape *polygon)
            : PhysicsShape(context, polygon) {}

    PolygonPhysicsShape::PolygonPhysicsShape(const PhysicsContext *context, float width, float height)
            : PhysicsShape(context, new b2PolygonShape()) {
        // Mark as manually allocated
        m_allocatedByUs = true;

        // Set vertices
        setAsBox(width, height);
    }

    PolygonPhysicsShape::PolygonPhysicsShape(const PhysicsContext *context, const std::vector<Vector2> &vertices)
            : PhysicsShape(context, new b2PolygonShape()) {
        // Mark as manually allocated
        m_allocatedByUs = true;

        // Set vertices
        setVertices(vertices);
    }

    Vector2 PolygonPhysicsShape::getCentroid() {
        auto c = ((b2PolygonShape *) m_shape)->m_centroid;
        return m_context->convertMetersToPixels(Vector2(c.x, c.y));
    }

    void PolygonPhysicsShape::setCentroid(const Vector2 &centroid) {
        auto c = m_context->convertPixelsToMeters(centroid);
        ((b2PolygonShape *) m_shape)->m_centroid = {c.X, c.Y};
    }

    std::vector<Vector2> PolygonPhysicsShape::getVertices() {
        auto poly = (b2PolygonShape *) m_shape;
        std::vector<Vector2> vertices(poly->m_count);
        for (auto i = 0; i < poly->m_count; i++) {
            vertices.push_back(m_context->convertMetersToPixels(Vector2(poly->m_vertices[i].x, poly->m_vertices[i].y)));
        }
        return vertices;
    }

    void PolygonPhysicsShape::setVertices(const std::vector<Vector2> &vertices) {
        // Convert
        auto verts = new b2Vec2[vertices.size()];
        for (auto i = 0; i < vertices.size(); i++) {
            auto pos = m_context->convertPixelsToMeters(vertices[i]);
            verts[i] = {pos.X, pos.Y};
        }

        // Send
        ((b2PolygonShape *) m_shape)->Set(verts, vertices.size());

        // Delete
        delete[] verts;
    }

    void PolygonPhysicsShape::setAsBox(float width, float height) {
        // Convert
        auto w = m_context->convertPixelsToMeters(width);
        auto h = m_context->convertPixelsToMeters(height);

        // Send
        ((b2PolygonShape *) m_shape)->SetAsBox(w / 2.0f, h / 2.0f);
    }
}