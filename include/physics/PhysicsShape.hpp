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

#ifndef PHYSICSFIXTURE_HPP
#define PHYSICSFIXTURE_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "BoundingBox.hpp"

// Chipmunk forward declare
class cpShape;

namespace ngine::physics {
    class PhysicsBody;
    class PhysicsSpace;

    class PhysicsShapeCircle;
    class PhysicsShapeSegment;
    class PhysicsShapePolygon;

    struct ShapeFilter {
        /**
        * Two objects with the same non-zero group value do not collide.
        * This is generally used to group objects in a composite object together to disable self collisions.
        */
        uintptr_t Group;

        /**
         * A bitmask of user definable categories that this object belongs to.
         * The category/mask combinations of both objects in a collision must agree for a collision to occur.
         */
        unsigned int Categories;

        /**
         * A bitmask of user definable category types that this object object collides with.
         * The category/mask combinations of both objects in a collision must agree for a collision to occur.
         */
        unsigned int Mask;
    };

    class NEAPI PhysicsShape {
        friend class PhysicsBody;
        friend class PhysicsShapeCircle;
        friend class PhysicsShapeSegment;
        friend class PhysicsShapePolygon;

        /**
         * Chipmunk shape
         */
        cpShape *m_shape = nullptr;

        explicit PhysicsShape(cpShape *shape);
    public:
        virtual ~PhysicsShape();

        static PhysicsShapeCircle *CreateCircle(PhysicsBody *body, float radius, Vector2 offset);

        // TODO: Transform support
        static PhysicsShapePolygon *CreatePolygon(PhysicsBody *body, std::vector<Vector2> vertices, float radius);

        PhysicsBody *getBody();

        PhysicsSpace *getSpace();

        void setBody(PhysicsBody *body);

        BoundingBox getBB();

        bool isSensor();

        void setIsSensor(bool sensor);

        float getElasticity();

        void setElasticity(float value);

        float getFriction();

        void setFriction(float value);

        Vector2 getSurfaceVelocity();

        void setSurfaceVelocity(const Vector2 &vel);

        uintptr_t getCollisionType();

        void setCollisionType(uintptr_t type);

        ShapeFilter getFilter();

        void setFilter(const ShapeFilter &filter);

        /**
         * @note Should be called if the shape has been modified and is attached to a static body.
         */
        void reindex();
    };

    class NEAPI PhysicsShapeCircle final : public PhysicsShape {
        friend class PhysicsShape;
        PhysicsShapeCircle(cpShape *shape);
    public:
        Vector2 getOffset();

        /**
         * @warning This is not recommended as this would result in unrealistic behaviour
         * @param offset
         */
        void setOffset(const Vector2 &offset);

        float getRadius();

        /**
         * @warning This is not recommended as this would result in unrealistic behaviour
         * @param radius
         */
        void setRadius(float radius);
    };

    class NEAPI PhysicsShapeSegment final : public PhysicsShape {
        friend class PhysicsShape;
        PhysicsShapeSegment(cpShape *shape);
    public:
        Vector2 getA();

        /**
         * @warning This is not recommended as this would result in unrealistic behaviour
         * @param a
         */
        void setA(const Vector2 &a);

        Vector2 getB();

        /**
         * @warning This is not recommended as this would result in unrealistic behaviour
         * @param b
         */
        void setB(const Vector2 &b);

        Vector2 getNormal();

        float getRadius();

        /**
         * @warning This is not recommended as this would result in unrealistic behaviour
         * @param radius
         */
        void setRadius(float radius);

        void setNeighbors(const Vector2 &prev, const Vector2 &next);
    };

    class NEAPI PhysicsShapePolygon final : public PhysicsShape {
        friend class PhysicsShape;
        PhysicsShapePolygon(cpShape *shape);
    public:
    };
}

#endif //PHYSICSSHAPE_HPP
