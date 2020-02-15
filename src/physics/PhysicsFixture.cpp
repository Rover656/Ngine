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

#include "physics/PhysicsFixture.hpp"

#include <Box2D/Box2D.h>

namespace ngine::physics {
    PhysicsFixture::PhysicsFixture(b2Fixture *fixture) {
        // Save fixture
        m_fixture = fixture;

        // Get body
        m_body = (PhysicsBody *) m_fixture->GetBody()->GetUserData();

        // Save ourselves
        m_fixture->SetUserData(this);
    }
}