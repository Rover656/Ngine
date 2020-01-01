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

#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "Shape.hpp"

#define MAX_POLY_VERTS 8

struct b2Shape;
struct b2PolygonShape;

namespace NerdThings::Ngine::Physics {
    /**
     * A Polygon shape.
     *
     * Powered by Box2D Math.
     */
    struct NEAPI Polygon : public Shape {
        static b2PolygonShape createPolygonShape(std::vector<Vector2> verts_);
        static b2PolygonShape createPolygonShape(std::vector<Vector2> verts_, std::vector<Vector2> normals_);
    public:
        Vector2 Vertices[MAX_POLY_VERTS];
        Vector2 Normals[MAX_POLY_VERTS];
        Vector2 Centroid;
        int Count;

        Polygon(std::vector<Vector2> verts_);
        Polygon(const b2PolygonShape &shape_);

        void Set(std::vector<Vector2> verts_);
        void SetAsBox(float hx, float hy);
    };
}

#endif //POLYGON_HPP
