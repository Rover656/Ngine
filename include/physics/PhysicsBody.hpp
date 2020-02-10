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

#ifndef PHYSICSBODY_HPP
#define PHYSICSBODY_HPP

#include "../Config.hpp"

#include "../graphics/Renderer.hpp"
#include "../Math.hpp"
#include "PhysicsShape.hpp"

// Forward declare Box2D
class cpBody;

namespace ngine::physics {
    // Forward declare
    class PhysicsShape;
    class PhysicsSpace;

    /**
      * The body type.
      */
    enum class PhysicsBodyType {
        Static = 0,
        Kinematic,
        Dynamic
    };

    /**
     * A physics body.
     */
    class NEAPI PhysicsBody final {
        friend class PhysicsShape;
        friend class PhysicsSpace;

        /**
         * The chipmunk body.
         */
        cpBody *m_body;

        PhysicsBody(cpBody *body);
    public:
        ~PhysicsBody();

        static PhysicsBody *CreateDynamicBody(float mass = 0, float moment = 0);
        static PhysicsBody *CreateKinematicBody();
        static PhysicsBody *CreateStaticBody();

        PhysicsBodyType getType() const;

        void setType(PhysicsBodyType type);

        PhysicsSpace *getSpace();

        void addShape(PhysicsShape *shape);

        /**
         *
         * @param shape
         * @param free Whether or not the shape should be freed or left.
         */
        void removeShape(PhysicsShape *shape, bool free = false);

        float getMass();

        void setMass(float mass);

        float getMoment();

        void setMoment(float moment);

        Vector2 getCenterOfGravity();

        void setCenterOfGravity(const Vector2 &center);

        Vector2 getPosition();

        void setPosition(const Vector2 &position);

        Vector2 getVelocity();

        void setVelocity(const Vector2 &vel);

        Vector2 getForce();

        void setForce(const Vector2 &force);

        Angle getRotation();

        void setRotation(Angle angle);

        float getAngularVelocity();

        void setAngularVelocity(float vel);

        float getTorque();

        void setTorque(float torque);

        /**
         * @note Called if a static body is moved.
         */
        void reindexShapes();
    };
}

#endif //PHYSICSBODY_HPP
