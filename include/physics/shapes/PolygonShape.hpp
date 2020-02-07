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

#ifndef POLYGONSHAPE_HPP
#define POLYGONSHAPE_HPP

#include "../../Config.hpp"

#include "PhysicsShape.hpp"

#define POLYGON_MAX_VERTS 8

class b2PolygonShape;

namespace ngine::physics::shapes {
    class NEAPI PolygonShape : public PhysicsShape {
    public:
        // Public Constructors

        /*
         * Create a polygon shape
         */
        PolygonShape();

        /*
         * Create a polygon shape from a Box2D polygon.
         */
        explicit PolygonShape(b2PolygonShape *polygonShape);

        /*
         * Move constructor
         */
        PolygonShape(PolygonShape &&old);

        /*
         * Copy constructor
         */
        PolygonShape(const PolygonShape &polygon);

        // Public Methods

        /*
         * Create a polygon with vertices to represent an axis-aligned box centered on the local origin.
         */
        static PolygonShape CreateAsBox(float hx, float hy);

        void debugDraw(graphics::Renderer *renderer, float ppm, Vector2 pos, float angle) const override;

        Vector2 getCentroid() const;

        std::vector<Vector2> getNormals() const;

        int getVertexCount() const;

        std::vector<Vector2> getVertices() const;

        /*
         * Build vertices to represent an axis-aligned box centered on the local origin.
         */
        void setAsBox(float hx, float hy);

        /*
         * Build vertices to represent an oriented box.
         */
        void setAsBox(float hx, float hy, const Vector2 &center, float rotation);

        /*
         * Set polygon centroid.
         */
        void setCentroid(Vector2 centroid);

        /*
         * Set polygon normals. Must have the same number of normals as vertices.
         */
        void setNormals(std::vector<Vector2> norms);

        /*
         * Set polygon vertices.
         */
        void setVertices(std::vector<Vector2> verts);
    };
}

#endif //POLYGONSHAPE_HPP
