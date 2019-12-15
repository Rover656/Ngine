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

#ifndef POLYGON_H
#define POLYGON_H

#include "../Ngine.h"

#include "../Vector2.h"
#include "CollisionShape.h"

#define MAX_POLY_VERTS 8

namespace NerdThings::Ngine::Physics {
    /*
     * Polygon shape
     */
    struct NEAPI Polygon : public ICollisionShape {
    private:
        // Private Methods

        bool _isCompatible(ICollisionShape *shape_) override;

        bool _runCollisionCheck(ICollisionShape *shape_) override;

    public:
        // Public Fields

        /*
         * The number of vertices
         */
        unsigned int VertexCount;

        /*
         * Polygon vertices
         */
        Vector2 Vertices[MAX_POLY_VERTS];

        /*
         * Polygon normals
         */
        Vector2 Normals[MAX_POLY_VERTS];

        // Public Constructor(s)

        /*
         * Create an empty polygon
         */
        Polygon()
                : VertexCount(0) {}

        /*
         * Create a polygon from vertices
         */
        Polygon(const std::vector<Vector2> &vertices_) {
            if (vertices_.size() > MAX_POLY_VERTS)
                throw std::runtime_error("Too many vertices");

            VertexCount = vertices_.size();

            for (unsigned int i = 0; i < VertexCount; i++) {
                Vertices[i] = vertices_[i];
            }

            GenerateNormals();
        }

        // Public Methods

#ifdef INCLUDE_BOX2D
        b2PolygonShape ToB2Shape();
#endif

        /*
         * Generate normals
         */
        void GenerateNormals();
    };
}

#endif //POLYGON_H
