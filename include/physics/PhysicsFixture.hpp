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

// Box2D declare
class b2Fixture;

namespace ngine::physics {
    class PhysicsBody;

    class NEAPI PhysicsFixture {
        friend class PhysicsBody;

        /**
         * The Box2D fixture.
         */
        b2Fixture *m_fixture;

        /**
         * The body we are attached to.
         */
        PhysicsBody *m_body;

        /**
         * Create a new fixture.
         */
        PhysicsFixture(b2Fixture *fixture);
    public:
    };
}

#endif //PHYSICSFIXTURE_HPP
