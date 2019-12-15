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

#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "CollisionShape.hpp"

#define MAX_POLY_VERTS 8

namespace NerdThings::Ngine::Physics {
    /*
     * Polygon shape
     */
    struct NEAPI Polygon : public ICollisionShape {
    public:
        Vector2 Vertices[MAX_POLY_VERTS];
        Vector2 Normals[MAX_POLY_VERTS];
        Vector2 Centroid;
        int Count;

        Polygon(std::vector<Vector2> verts_) {}

        void SetAsBox(float hx, float hy);
    };
}

#endif //POLYGON_HPP
