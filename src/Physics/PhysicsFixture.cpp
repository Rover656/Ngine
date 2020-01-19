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

namespace NerdThings::Ngine::Physics {
    // Public Constructor

    PhysicsFixture::PhysicsFixture(b2Fixture *fixture_) {
        // Save fixture
        _Fixture = fixture_;

        // Save ourselves
        _Fixture->SetUserData(this);

        // Get shape
        switch(GetType()) {
            case Shapes::SHAPE_CIRCLE:
                _Shape = new Shapes::CircleShape((b2CircleShape*)_Fixture->GetShape());
                break;
            case Shapes::SHAPE_EDGE:
                break;
            case Shapes::SHAPE_POLYGON:
                _Shape = new Shapes::PolygonShape((b2PolygonShape*)_Fixture->GetShape());
                break;
            case Shapes::SHAPE_CHAIN:
                break;
            case Shapes::SHAPE_TYPECOUNT:
                break;
        }
    }

    // Destructor

    PhysicsFixture::~PhysicsFixture() {}

    // Public Methods

    void PhysicsFixture::Destroy() {
        GetBody()->DestroyFixture(this);
    }

    b2Fixture *PhysicsFixture::GetB2Fixture() {
        return _Fixture;
    }

    PhysicsBody *PhysicsFixture::GetBody() {
        return (PhysicsBody *)_Fixture->GetBody()->GetUserData();
    }

    float PhysicsFixture::GetDensity() const {
        return _Fixture->GetDensity();
    }

    float PhysicsFixture::GetFriction() const {
        return _Fixture->GetFriction();
    }

    float PhysicsFixture::GetRestitution() const {
        return _Fixture->GetRestitution();
    }

    Shapes::PhysicsShape *PhysicsFixture::GetShape() {
        return _Shape;
    }

    const Shapes::PhysicsShape *PhysicsFixture::GetShape() const {
        return _Shape;
    }

    Shapes::PhysicsShapeType PhysicsFixture::GetType() const {
        return (Shapes::PhysicsShapeType)_Fixture->GetType();
    }

    bool PhysicsFixture::IsSensor() {
        return _Fixture->IsSensor();
    }

    void PhysicsFixture::Refilter() {
        _Fixture->Refilter();
    }

    void PhysicsFixture::SetSensor(bool sensor_) {
        _Fixture->SetSensor(sensor_);
    }

    bool PhysicsFixture::TestPoint(const Vector2 &p) {
        // Translate point (PPM)
        b2Vec2 pos(p.X / GetBody()->GetWorld()->PixelsPerMeter, p.Y / GetBody()->GetWorld()->PixelsPerMeter);
        return _Fixture->TestPoint(pos);
    }
}
