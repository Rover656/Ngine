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

#include "Polygon.hpp"

#include "BoundingBox.hpp"
#include "Circle.hpp"

namespace NerdThings::Ngine::Physics {
    void Polygon::SetAsBox(float hx, float hy) {
        Count = 4;
        Vertices[0] = {-hx, -hy};
        Vertices[1] = {hx, -hy};
        Vertices[2] = {hx, hy};
        Vertices[3] = {-hx, hy};
        Normals[0] = {0, -1};
        Normals[1] = {1, 0};
        Normals[2] = {0, 1};
        Normals[3] = {-1, 0};
        Centroid = {0, 0};
    }
}
