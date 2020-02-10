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

#include "physics/PhysicsSpace.hpp"

#include <chipmunk/chipmunk.h>
#include <chipmunk/chipmunk_unsafe.h>

namespace ngine::physics {
    PhysicsShape::PhysicsShape(cpShape *shape) {
        // Save shape
        m_shape = shape;

        // Save as userdata
        cpShapeSetUserData(m_shape, this);
    }

    PhysicsShape::~PhysicsShape() {
        // Delete shape
        if (m_shape != nullptr) {
            cpShapeFree(m_shape);
            m_shape = nullptr;
        }
    }

    PhysicsShapeCircle *PhysicsShape::CreateCircle(PhysicsBody *body, float radius, Vector2 offset) {
        // Create circle
        auto c = cpCircleShapeNew(body->m_body, radius, {(double) offset.X, (double) offset.Y});
        return new PhysicsShapeCircle(c);
    }

    PhysicsShapePolygon *PhysicsShape::CreatePolygon(PhysicsBody *body, std::vector<Vector2> vertices, float radius) {
        // TODO
        cpTransform t;

        // Convert
        std::vector<cpVect> data;
        for (auto v : vertices) {
            data.push_back({(double) v.X, (double) v.Y});
        }

        auto p = cpPolyShapeNew(body->m_body, data.size(), data.data(), t, radius);
        return new PhysicsShapePolygon(p);
    }

    PhysicsBody *PhysicsShape::getBody() {
        auto body = cpShapeGetBody(m_shape);
        if (body != nullptr)
            return (PhysicsBody *) cpBodyGetUserData(body);
        return nullptr;
    }

    PhysicsSpace *PhysicsShape::getSpace() {
        auto space = cpShapeGetSpace(m_shape);
        if (space != nullptr)
            return (PhysicsSpace *) cpSpaceGetUserData(space);
        return nullptr;
    }

    void PhysicsShape::setBody(PhysicsBody *body) {
        cpShapeSetBody(m_shape, body->m_body);
    }

    BoundingBox PhysicsShape::getBB() {
        auto box = cpShapeGetBB(m_shape);
        BoundingBox bb;
        bb.Max = {(float) box.r, (float) box.b};
        bb.Min = {(float) box.l, (float) box.t};
        return bb;
    }

    bool PhysicsShape::isSensor() {
        return cpShapeGetSensor(m_shape);
    }

    void PhysicsShape::setIsSensor(bool sensor) {
        cpShapeSetSensor(m_shape, sensor);
    }

    float PhysicsShape::getElasticity() {
        return cpShapeGetElasticity(m_shape);
    }

    void PhysicsShape::setElasticity(float value) {
        cpShapeSetElasticity(m_shape, value);
    }

    float PhysicsShape::getFriction() {
        return cpShapeGetFriction(m_shape);
    }

    void PhysicsShape::setFriction(float value) {
        cpShapeSetFriction(m_shape, value);
    }

    Vector2 PhysicsShape::getSurfaceVelocity() {
        auto v = cpShapeGetSurfaceVelocity(m_shape);
        return {(float) v.x, (float) v.y};
    }

    void PhysicsShape::setSurfaceVelocity(const Vector2 &vel) {
        cpShapeSetSurfaceVelocity(m_shape, {(double) vel.X, (double) vel.Y});
    }

    uintptr_t PhysicsShape::getCollisionType() {
        return cpShapeGetCollisionType(m_shape);
    }

    void PhysicsShape::setCollisionType(uintptr_t type) {
        cpShapeSetCollisionType(m_shape, type);
    }

    ShapeFilter PhysicsShape::getFilter() {
        auto f = cpShapeGetFilter(m_shape);
        return {
                f.group,
                f.categories,
                f.mask
        };
    }

    void PhysicsShape::setFilter(const ShapeFilter &filter) {
        cpShapeSetFilter(m_shape, {
                filter.Group,
                filter.Categories,
                filter.Mask
        });
    }

    void PhysicsShape::reindex() {
        auto space = cpShapeGetSpace(m_shape);
        if (space == nullptr) return;
        cpSpaceReindexShape(space, m_shape);
    }

    PhysicsShapeCircle::PhysicsShapeCircle(cpShape *shape)
            : PhysicsShape(shape) {}

    Vector2 PhysicsShapeCircle::getOffset() {
        auto o = cpCircleShapeGetOffset(m_shape);
        return {(float) o.x, (float) o.y};
    }

    void PhysicsShapeCircle::setOffset(const Vector2 &offset) {
        cpCircleShapeSetOffset(m_shape, {(double) offset.X, (double) offset.Y});
        // TODO: Update in space if static!
    }

    float PhysicsShapeCircle::getRadius() {
        return cpCircleShapeGetRadius(m_shape);
    }

    void PhysicsShapeCircle::setRadius(float radius) {
        cpCircleShapeSetRadius(m_shape, radius);
        // TODO: Update in space if static!
    }

    PhysicsShapeSegment::PhysicsShapeSegment(cpShape *shape)
            : PhysicsShape(shape) {}

    Vector2 PhysicsShapeSegment::getA() {
        return Vector2();
    }

    void PhysicsShapeSegment::setA(const Vector2 &a) {
        // TODO: Update in space if static!
    }

    Vector2 PhysicsShapeSegment::getB() {
        return Vector2();
    }

    void PhysicsShapeSegment::setB(const Vector2 &b) {
        // TODO: Update in space if static!
    }

    Vector2 PhysicsShapeSegment::getNormal() {
        return Vector2();
    }

    float PhysicsShapeSegment::getRadius() {
        return 0;
    }

    void PhysicsShapeSegment::setRadius(float radius) {
        // TODO: Update in space if static!
    }

    void PhysicsShapeSegment::setNeighbors(const Vector2 &prev, const Vector2 &next) {
        // TODO: Update in space if static!
    }

    PhysicsShapePolygon::PhysicsShapePolygon(cpShape *shape)
            : PhysicsShape(shape) {}
}
