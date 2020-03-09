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

#ifndef PHYSICSSHAPE_HPP
#define PHYSICSSHAPE_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "PhysicsContext.hpp"

// Box2D declarations
class b2Shape;
class b2CircleShape;
class b2PolygonShape;

namespace ngine::physics {
    struct MassData {
        /**
         * The mass of the shape, usually in kilograms.
         */
        float Mass;

        /**
         * The position of the shape's centroid relative to the shape's origin.
         */
        Vector2 Center;

        /**
         * The rotational inertia of the shape about the local origin.
         */
        float Inertia;
    };

    /**
     * Root physics shape class.
     */
    class NEAPI PhysicsShape {
        friend class PhysicsBody;

    protected:
        /**
         * The Box2D shape.
         */
        b2Shape *m_shape = nullptr;

        /**
         * Physics context for unit conversions.
         */
        const PhysicsContext *m_context;

        /**
         * Whether or not the shape was allocated by us
         */
        bool m_allocatedByUs = false;

        /**
         * Create a physics shape.
         *
         * @param shape Box2D shape pointer.
         */
        PhysicsShape(const PhysicsContext *context, b2Shape *shape);

    public:
        virtual ~PhysicsShape();

        /**
         * Physics shape type.
         */
        enum class Type { Circle = 0, Edge, Polygon, Chain };

        /**
         * Get the shape's type.
         *
         * @return The shape type.
         */
        Type getType() const;

        int getChildCount() const;

        bool testPoint(const Transform2D &transform, const Vector2 &pos);

        MassData computeMass(float density) const;
    };

    /**
     * Defines a circle shape.
     */
    class NEAPI CirclePhysicsShape final : public PhysicsShape {
        friend class PhysicsFixture;

        /**
         * Create a circle shape reference.
         *
         * @param context The context.
         * @param circle The circle reference.
         */
        CirclePhysicsShape(const PhysicsContext *context,
                           b2CircleShape *circle);

    public:
        /**
         * Create a circle shape.
         *
         * @param context The context. Must be the same as the fixture you're
         * attaching this to.
         * @param radius Circle radius.
         * @param position Offset from body center.
         */
        CirclePhysicsShape(const PhysicsContext *context, float radius,
                           const Vector2 &position = {0, 0});

        /**
         * Get the circle radius.
         *
         * @return Circle radius.
         */
        float getRadius();

        /**
         * Set the circle radius.
         *
         * @param radius The new circle radius.
         */
        void setRadius(float radius);

        /**
         * Get the circle position (offset from body).
         *
         * @return Circle position.
         */
        Vector2 getPosition();

        /**
         * Set the circle position (offset from body).
         *
         * @param position New circle position.
         */
        void setPosition(const Vector2 &position);
    };

    /**
     * This describes a polygon shape.
     */
    class NEAPI PolygonPhysicsShape : public PhysicsShape {
        friend class PhysicsFixture;

        PolygonPhysicsShape(const PhysicsContext *context,
                            b2PolygonShape *polygon);

    public:
        /**
         * Create a polygon physics shape as a box.
         */
        PolygonPhysicsShape(const PhysicsContext *context, float width,
                            float height);

        /**
         * Create a polygon physics shape with vertices.
         */
        PolygonPhysicsShape(const PhysicsContext *context,
                            const std::vector<Vector2> &vertices);

        Vector2 getCentroid();

        void setCentroid(const Vector2 &centroid);

        std::vector<Vector2> getVertices();

        /**
         *
         * @warning It is not recommended to change the number of vertices.
         */
        void setVertices(const std::vector<Vector2> &vertices);

        /**
         * Make this a box positioned at its center.
         *
         * @param context Physics context.
         * @param width Width of box.
         * @param height Height of box.
         */
        void setAsBox(float width, float height);
    };
} // namespace ngine::physics

#endif // PHYSICSSHAPE_HPP
