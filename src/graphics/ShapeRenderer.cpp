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

#include "graphics/ShapeRenderer.hpp"

#include <cmath>

namespace ngine::graphics {
    void ShapeRenderer::_drawLine(Renderer *renderer, Vector2 v1, Vector2 v2, Color color, float thickness) {
        // Flip inputs if X is smaller
        bool flipped = false;
        if (v1.X > v2.X) {
            auto tmp = v1;
            v1 = v2;
            v2 = tmp;
            flipped = true;
        }

        // Get distance and angle
        auto dx = v2.X - v1.X;
        auto dy = v2.Y - v1.Y;
        float d = sqrtf(dx * dx + dy * dy);
        float angle = asinf(dy / d);

        auto t = RadToDeg(angle);

        // Build translation
        renderer->pushMatrix();
        renderer->translate({v1.X, v1.Y, 0});
        renderer->rotate(RadToDeg(angle), {0, 0, 1});
        if (flipped)
            renderer->translate({thickness / 2.0f, -thickness / 2.0f, 0});
        else renderer->translate({-thickness / 2.0f, -thickness / 2.0f, 0});

        // Write to renderer
        renderer->pushVertex({{0, 0, 0}, {0, 0}, color});
        renderer->pushVertex({{0, thickness, 0}, {0, 1}, color});
        renderer->pushVertex({{d, thickness, 0}, {1, 1}, color});
        renderer->pushVertex({{d, 0, 0}, {1, 0}, color});

        // Pop matrix
        renderer->popMatrix();
    }

    void graphics::ShapeRenderer::DrawLine(Renderer *renderer, Vector2 v1, Vector2 v2, Color color,
                                           float thickness) {
        renderer->setTexture(nullptr);
        renderer->beginVertices(PrimitiveType::Quads);
        _drawLine(renderer, v1, v2, color, thickness);
        renderer->endVertices();
    }

    void
    graphics::ShapeRenderer::DrawCircle(Renderer *renderer, Vector2 center, float radius, Color color,
                                        bool outline,
                                        float lineThickness) {
        renderer->setTexture(nullptr);
        if (outline) {
            // Get two times PI
            auto twoPi = 2.0f * PI;
            float step = DegToRad(360.0f / LINES_PER_CIRCLE);

            // Render lines.
            renderer->beginVertices(PrimitiveType::Quads);
            for (auto i = 0; i < LINES_PER_CIRCLE; i++) {
                _drawLine(renderer,
                          {center.X + sinf((float) i * step) * radius, center.Y + cosf((float) i * step) * radius},
                          {center.X + sinf((float) (i + 1) * step) * radius,
                           center.Y + cosf((float) (i + 1) * step) * radius},
                          color, lineThickness);
            }
            renderer->endVertices();
        } else {
            // Get two times PI
            float twoPi = 2.0f * PI;

            // Render using triangle fan primitive type (NEW!!!)
            renderer->beginVertices(PrimitiveType::TriangleFan);
            renderer->pushVertex({{center.X, center.Y, 0}, {0, 0}, color});
            for (auto i = 1; i <= TRIANGLES_PER_CIRCLE + 1; i++) {
                renderer->pushVertex({{
                                              center.X + (radius * cosf(i * twoPi / TRIANGLES_PER_CIRCLE)),
                                              center.Y + (radius * sinf(i * twoPi / TRIANGLES_PER_CIRCLE)), 0
                                       }, {0, 0}, color});
            }
            renderer->endVertices();
        }
    }

    void graphics::ShapeRenderer::DrawRectangle(Renderer *renderer, Vector2 position, float width, float height,
                                                Color color, Angle rotation, Vector2 origin, bool outline,
                                                float lineThickness) {
        DrawRectangle(renderer, {position, width, height}, color, rotation, origin, outline, lineThickness);
    }

    void graphics::ShapeRenderer::DrawRectangle(Renderer *renderer, Rectangle rect, Color color, Angle rotation,
                                                Vector2 origin, bool outline, float lineThickness) {
        // Push a matrix and set up our transform/rotation
        renderer->setTexture(nullptr);
        renderer->pushMatrix();
        renderer->translate({rect.X, rect.Y, 0});
        renderer->rotate(rotation, {0, 0, 1});
        renderer->translate({-origin.X, -origin.Y, 0});

        if (outline) {
            // Draw lines
            renderer->beginVertices(PrimitiveType::Quads);
            _drawLine(renderer, {0, 0}, {rect.Width, 0}, color, lineThickness);
            _drawLine(renderer, {rect.Width, 0}, {rect.Width, rect.Height}, color, lineThickness);
            _drawLine(renderer, {rect.Width, rect.Height}, {0, rect.Height}, color, lineThickness);
            _drawLine(renderer, {0, rect.Height}, {0, 0}, color, lineThickness);
            renderer->endVertices();
        } else {
            renderer->beginVertices(PrimitiveType::Quads);
            renderer->pushVertex({{0, 0, 0}, {0, 0}, color});
            renderer->pushVertex({{rect.Width, 0, 0}, {1, 0}, color});
            renderer->pushVertex({{rect.Width, rect.Height, 0}, {1, 1}, color});
            renderer->pushVertex({{0, rect.Height, 0}, {0, 1}, color});
            renderer->endVertices();
        }
        renderer->popMatrix();
    }

    void graphics::ShapeRenderer::DrawTriangle(Renderer *renderer, Vector2 v1, Vector2 v2, Vector2 v3, Color color,
                                               Angle rotation, Vector2 origin, bool outline, float lineThickness) {
        // Push matrix (for rotation)
        renderer->setTexture(nullptr);
        renderer->pushMatrix();
        renderer->translate({v1.X, v1.Y, 0});
        renderer->rotate(rotation, {0, 0, 1});
        renderer->translate({-origin.X, -origin.Y, 0});

        Vector2 a = {0, 0};
        auto b = v2 - v1;
        auto c = v3 - v1;

        if (outline) {
            // Render with lines
            renderer->beginVertices(PrimitiveType::Quads);
            _drawLine(renderer, a, b, color, lineThickness);
            _drawLine(renderer, b, c, color, lineThickness);
            _drawLine(renderer, c, a, color, lineThickness);
            renderer->endVertices();
        } else {
            // Render it.
            renderer->beginVertices(PrimitiveType::Triangles);
            renderer->pushVertex({{a.X, a.Y, 0}, {}, color});
            renderer->pushVertex({{b.X, b.Y, 0}, {}, color});
            renderer->pushVertex({{c.X, c.Y, 0}, {}, color});
            renderer->endVertices();
        }

        // Pop
        renderer->popMatrix();
    }
}
