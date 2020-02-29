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

#ifndef PHYSICSDEBUGDRAW_HPP
#define PHYSICSDEBUGDRAW_HPP

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "ngine/Config.hpp"

#include "ngine/graphics/Renderer.hpp"
#include "ngine/physics/PhysicsWorld.hpp"

#include <box2d/box2d.h>

namespace ngine::physics {
    class PhysicsDebugDraw : public b2Draw {
        graphics::Renderer *m_renderer = nullptr;
        PhysicsWorld *m_world = nullptr;
        const PhysicsContext *m_context = nullptr;
    public:
        PhysicsDebugDraw(PhysicsWorld *world);

        void setRenderer(graphics::Renderer *renderer);

        void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;

        void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;

        void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override;

        void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override;

        void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override;

        void DrawTransform(const b2Transform &xf) override;

        void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override;
    };
}

#endif // DOXYGEN_SHOULD_SKIP_THIS

#endif // PHYSICSDEBUGDRAW_HPP
