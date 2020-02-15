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

#include "physics/PhysicsBody.hpp"

#include "physics/PhysicsWorld.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics {
    PhysicsBody::PhysicsBody(PhysicsWorld *world, b2Body *body) {
        // Save body and world
        m_body = body;
        m_world = world;

        // Save ourselves
        m_body->SetUserData(this);
    }

    PhysicsBody::~PhysicsBody() {
        // Remove from world
        m_body->GetWorld()->DestroyBody(m_body);
        m_body = nullptr;
    }

    PhysicsWorld *PhysicsBody::getWorld() {
        return m_world;
    }

    Vector2 PhysicsBody::getPosition() {
        auto p = m_body->GetPosition();
        return m_world->convertMetersToPixels(Vector2(p.x, p.y));
    }

    void PhysicsBody::setPosition(Vector2 position) {
        setTransform({position, getRotation()});
    }

    Angle PhysicsBody::getRotation() {
        return Angle(RadToDeg(m_body->GetAngle()));
    }

    void PhysicsBody::setRotation(Angle rotation) {
        setTransform({getPosition(), rotation});
    }

    Transform2D PhysicsBody::getTransform() {
        auto tr = m_body->GetTransform();
        auto rot = RadToDeg(tr.q.GetAngle());
        auto pos = m_world->convertMetersToPixels(Vector2(tr.p.x, tr.p.y));
        return Transform2D(pos, rot);
    }

    void PhysicsBody::setTransform(Transform2D transform) {
        auto pos = m_world->convertPixelsToMeters(transform.Position);
        m_body->SetTransform({pos.X, pos.Y}, DegToRad(transform.Rotation.getDegrees()));
    }

    float PhysicsBody::getMass() {
        return m_body->GetMass();
    }

    float PhysicsBody::getInertia() {
        // TODO: Verify unit conversion
        auto oneM = m_world->convertMetersToPixels(1);
        return m_body->GetInertia() * oneM * oneM;
    }

    void PhysicsBody::resetMassData() {
        m_body->ResetMassData();
    }

    PhysicsBodyType PhysicsBody::getType() {
        switch (m_body->GetType()) {
            case b2_staticBody:
                return PhysicsBodyType::Static;
            case b2_kinematicBody:
                return PhysicsBodyType::Kinematic;
            case b2_dynamicBody:
                return PhysicsBodyType::Dynamic;
        }
    }

    void PhysicsBody::setType(PhysicsBodyType type) {
        switch (type) {
            case PhysicsBodyType::Static:
                m_body->SetType(b2_staticBody);
                break;
            case PhysicsBodyType::Kinematic:
                m_body->SetType(b2_kinematicBody);
                break;
            case PhysicsBodyType::Dynamic:
                m_body->SetType(b2_dynamicBody);
                break;
        }
    }
}
