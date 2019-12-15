/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "PhysicsShape.hpp"

#include <Box2D/Box2D.h>

namespace NerdThings::Ngine::Physics::Shapes {
    // Protected Constructors

    PhysicsShape::PhysicsShape() {
        _ManagedByMe = true;
    }

    PhysicsShape::PhysicsShape(PhysicsShape &&old) {
        _ManagedByMe = old._ManagedByMe;
        _Shape = old._Shape;
        old._Shape = nullptr;
    }

    PhysicsShape::PhysicsShape(b2Shape *shape_) {
        _ManagedByMe = false;
        _Shape = shape_;
    }

    PhysicsShape::PhysicsShape(const PhysicsShape &shape_) {
        _ManagedByMe = true;
    }

    // Destructor

    PhysicsShape::~PhysicsShape() {
        if (_ManagedByMe) delete _Shape;
    }

    // Public Methods

    BoundingBox PhysicsShape::ComputeAABB(const Transform &xf, int childIndex) {
        // Convert transform
        b2Transform transform;
        transform.p = {xf.Position.X, xf.Position.Y};
        transform.q.s = xf.Rotation.Sine;
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

    b2Shape *PhysicsShape::GetB2Shape() {
        return _Shape;
    }

    int PhysicsShape::GetChildCount() const {
        return _Shape->GetChildCount();
    }

    float PhysicsShape::GetRadius() const {
        return _Shape->m_radius;
    }

    void PhysicsShape::SetRadius(float radius_) {
        _Shape->m_radius = radius_;
    }

    bool PhysicsShape::TestPoint(const Transform &xf, const Vector2 &p) {
        // Convert transform
        b2Transform transform;
        transform.p = {xf.Position.X, xf.Position.Y};
        transform.q.s = xf.Rotation.Sine;
        transform.q.c = xf.Rotation.Cos;

        return _Shape->TestPoint(transform, {p.X, p.Y});
    }
}