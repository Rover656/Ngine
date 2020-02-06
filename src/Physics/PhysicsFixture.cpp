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

#include "Physics/PhysicsFixture.hpp"

#include "Physics/Shapes/CircleShape.hpp"
#include "Physics/Shapes/PolygonShape.hpp"
#include "Physics/PhysicsWorld.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics {
    // Public Constructor

    PhysicsFixture::PhysicsFixture(b2Fixture *fixture_) {
        // Save fixture
        m_fixture = fixture_;

        // Save ourselves
        m_fixture->SetUserData(this);

        // Get shape
        switch(getType()) {
            case shapes::SHAPE_CIRCLE:
                m_shape = new shapes::CircleShape((b2CircleShape*)m_fixture->GetShape());
                break;
            case shapes::SHAPE_EDGE:
                break;
            case shapes::SHAPE_POLYGON:
                m_shape = new shapes::PolygonShape((b2PolygonShape*)m_fixture->GetShape());
                break;
            case shapes::SHAPE_CHAIN:
                break;
            case shapes::SHAPE_TYPECOUNT:
                break;
        }
    }

    // Destructor

    PhysicsFixture::~PhysicsFixture() {}

    // Public Methods

    void PhysicsFixture::destroy() {
        getBody()->destroyFixture(this);
    }

    b2Fixture *PhysicsFixture::getB2Fixture() {
        return m_fixture;
    }

    PhysicsBody *PhysicsFixture::getBody() {
        return (PhysicsBody *)m_fixture->GetBody()->GetUserData();
    }

    float PhysicsFixture::getDensity() const {
        return m_fixture->GetDensity();
    }

    float PhysicsFixture::getFriction() const {
        return m_fixture->GetFriction();
    }

    float PhysicsFixture::getRestitution() const {
        return m_fixture->GetRestitution();
    }

    shapes::PhysicsShape *PhysicsFixture::getShape() {
        return m_shape;
    }

    const shapes::PhysicsShape *PhysicsFixture::getShape() const {
        return m_shape;
    }

    shapes::PhysicsShapeType PhysicsFixture::getType() const {
        return (shapes::PhysicsShapeType)m_fixture->GetType();
    }

    bool PhysicsFixture::isSensor() {
        return m_fixture->IsSensor();
    }

    void PhysicsFixture::refilter() {
        m_fixture->Refilter();
    }

    void PhysicsFixture::setSensor(bool sensor_) {
        m_fixture->SetSensor(sensor_);
    }

    bool PhysicsFixture::testPoint(const Vector2 &p) {
        // Translate point (PPM)
        b2Vec2 pos(p.X / getBody()->getWorld()->PixelsPerMeter, p.Y / getBody()->getWorld()->PixelsPerMeter);
        return m_fixture->TestPoint(pos);
    }
}
