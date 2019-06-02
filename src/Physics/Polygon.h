/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef POLYGON_H
#define POLYGON_H

#include "../ngine.h"

#include "../Math/Vector2.h"
#include "CollisionShape.h"

#define MAX_POLY_VERTS 8

namespace NerdThings::Ngine::Physics {
    struct NEAPI TPolygon : public ICollisionShape {
    private:
        // Private Methods

        bool IsCompatible(ICollisionShape *shape_) override;

        bool RunCollisionCheck(ICollisionShape *shape_) override;
    public:
        // Public Fields

        /*
         * The number of vertices
         */
        unsigned int VertexCount;

        /*
         * Polygon vertices
         */
        Math::TVector2 Vertices[MAX_POLY_VERTS];

        /*
         * Polygon normals
         */
        Math::TVector2 Normals[MAX_POLY_VERTS];

        // Public Constructor(s)

        /*
         * Create an empty polygon
         */
        TPolygon()
            : VertexCount(0) {}

        /*
         * Create a polygon from vertices
         */
        TPolygon(const std::vector<Math::TVector2> &vertices_) {
            if (vertices_.size() > MAX_POLY_VERTS)
                throw std::runtime_error("Too many vertices");

            VertexCount = vertices_.size();

            for (auto i = 0; i < VertexCount; i++) {
                Vertices[i] = vertices_[i];
            }

            GenerateNormals();
        }

        // Public Methods

        /*
         * Generate normals
         */
        void GenerateNormals();
    };
}

#endif //POLYGON_H
