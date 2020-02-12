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

#include "physics/shapes/PhysicsShape.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics::shapes {
    // Protected Constructors

    PhysicsShape::PhysicsShape() {
        _ManagedByMe = true;
    }

    PhysicsShape::PhysicsShape(PhysicsShape &&old) {
        _ManagedByMe = old._ManagedByMe;
        _Shape = old._Shape;
        old._Shape = nullptr;
    }

    PhysicsShape::PhysicsShape(b2Shape *shape) {
        _ManagedByMe = false;
        _Shape = shape;
    }

    PhysicsShape::PhysicsShape(const PhysicsShape &shape) {
        _ManagedByMe = true;
    }

    // Destructor

    PhysicsShape::~PhysicsShape() {
        if (_ManagedByMe) delete _Shape;
    }

    // Public Methods

    BoundingBox PhysicsShape::computeAABB(const Transform2D &xf, int childIndex) {
        // Convert transform
        b2Transform transform;
        transform.p = {xf.Position.X, xf.Position.Y};
        transform.q.s = xf.Rotation.Sin;
        transform.q.c = xf.Rotation.Cos;

        // Get AABB
        b2AABB aabb;
        _Shape->ComputeAABB(&aabb, transform, childIndex);

        // Build bounding box
        BoundingBox b;
        b.Min = {aabb.lowerBound.x, aabb.lowerBound.y};
        b.Max = {aabb.upperBound.x, aabb.upperBound.y};
        return b;
    }

    b2Shape *PhysicsShape::getB2Shape() {
        return _Shape;
    }

    int PhysicsShape::getChildCount() const {
        return _Shape->GetChildCount();
    }

    float PhysicsShape::getRadius() const {
        return _Shape->m_radius;
    }

    void PhysicsShape::setRadius(float radius) {
        _Shape->m_radius = radius;
    }

    bool PhysicsShape::testPoint(const Transform2D &xf, const Vector2 &p) {
        // Convert transform
        b2Transform transform;
        transform.p = {xf.Position.X, xf.Position.Y};
        transform.q.s = xf.Rotation.Sin;
        transform.q.c = xf.Rotation.Cos;

        return _Shape->TestPoint(transform, {p.X, p.Y});
    }
}