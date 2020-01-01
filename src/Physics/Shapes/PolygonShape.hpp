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

#ifndef POLYGONSHAPE_HPP
#define POLYGONSHAPE_HPP

#include "../../Config.hpp"

#include "PhysicsShape.hpp"

#define POLYGON_MAX_VERTS 8

class b2PolygonShape;

namespace NerdThings::Ngine::Physics::Shapes {
    class PolygonShape : public PhysicsShape {
    public:
        // Public Constructors

        /*
         * Create a polygon shape
         */
        PolygonShape();

        /*
         * Create a polygon shape from a Box2D polygon.
         */
        explicit PolygonShape(b2PolygonShape *polygonShape_);

        /*
         * Move constructor
         */
        PolygonShape(PolygonShape &&old);

        /*
         * Copy constructor
         */
        PolygonShape(const PolygonShape &polygon_);

        // Public Methods

        /*
         * Create a polygon with vertices to represent an axis-aligned box centered on the local origin.
         */
        static PolygonShape CreateAsBox(float hx, float hy);

        void DebugDraw(float ppm_, Vector2 pos_, float angle_) const override;

        Vector2 GetCentroid() const;

        std::vector<Vector2> GetNormals() const;

        int GetVertexCount() const;

        std::vector<Vector2> GetVertices() const;
        /*
         * Build vertices to represent an axis-aligned box centered on the local origin.
         */
        void SetAsBox(float hx, float hy);

        /*
         * Build vertices to represent an oriented box.
         */
        void SetAsBox(float hx, float hy, const Vector2 &center_, float rotation_);

        /*
         * Set polygon centroid.
         */
        void SetCentroid(Vector2 centroid_);

        /*
         * Set polygon normals. Must have the same number of normals as vertices.
         */
        void SetNormals(std::vector<Vector2> verts_);

        /*
         * Set polygon vertices.
         */
        void SetVertices(std::vector<Vector2> verts_);
    };
}

#endif //POLYGONSHAPE_HPP
