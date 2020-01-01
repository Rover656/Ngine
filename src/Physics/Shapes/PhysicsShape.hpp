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

#ifndef PHYSICSSHAPE_HPP
#define PHYSICSSHAPE_HPP

#include "../../Config.hpp"

#include "../../Math.hpp"
#include "../BoundingBox.hpp"

// Forward declare box2d
class b2Shape;

namespace NerdThings::Ngine::Physics::Shapes {
    enum PhysicsShapeType {
        SHAPE_CIRCLE = 0,
        SHAPE_EDGE = 1,
        SHAPE_POLYGON = 2,
        SHAPE_CHAIN = 3,
        SHAPE_TYPECOUNT = 4
    };

    class NEAPI PhysicsShape {
        // Private Fields

        /*
         * Whether or not I manage the lifecycle of the shape.
         */
        bool _ManagedByMe = false;

    protected:
        // Protected Fields

        /*
         * The shape
         */
        b2Shape *_Shape;

        // Protected Constructors

        /*
         * Create new Physics Shape
         */
        PhysicsShape();

        /*
         * Create a physics shape from a Box2D shape created by Box2D.
         */
        explicit PhysicsShape(b2Shape *shape_);

        /*
         * Move constructor
         */
        PhysicsShape(PhysicsShape &&old);

        /*
         * Copy constructor
         */
        PhysicsShape(const PhysicsShape &shape_);

    public:
        // Destructor

        virtual ~PhysicsShape();

        // Public Methods

        BoundingBox ComputeAABB(const Transform2D &xf, int childIndex = 0);

        // TODO: void ComputeMass()

        virtual void DebugDraw(float ppm_, Vector2 pos_, float angle_) const = 0;

        /*
         * Get the Box2D shape.
         */
        b2Shape *GetB2Shape();

        /*
         * Get the number of child primitives
         */
        int GetChildCount() const;

        /*
         * Get shape radius
         */
        float GetRadius() const;

        /*
         * Set shape radius.
         */
        void SetRadius(float radius_);

        /*
         * Test a point for containment in this shape.
         */
        bool TestPoint(const Transform2D &xf, const Vector2 &p);

        // TODO: Raycasting
    };
}

#endif //PHYSICSSHAPE_HPP
