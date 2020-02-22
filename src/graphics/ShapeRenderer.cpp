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

        renderer->rotate(t, {0, 0, 1});

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

    void graphics::ShapeRenderer::drawLine(Renderer *renderer, Vector2 v1, Vector2 v2, Color color,
                                           float thickness) {
        renderer->setTexture(nullptr);
        renderer->beginVertices(PrimitiveType::Quads);

        // Flip Ys if needed
        _drawLine(renderer, {v1.X, v1.Y}, {v2.X, v2.Y}, color, thickness);

        renderer->endVertices();
    }

    void
    graphics::ShapeRenderer::drawCircle(Renderer *renderer, Vector2 center, float radius, Color color,
                                        bool outline,
                                        float lineThickness) {
        renderer->setTexture(nullptr);
        if (outline) {
            // Get two times PI
            auto twoPi = 2.0f * PI;
            float step = DegToRad(360.0f / LINES_PER_CIRCLE);

            // Render lines.
            renderer->beginVertices(PrimitiveType::Quads);  // TODO: lines don't line up correctly?
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
            renderer->beginVertices(PrimitiveType::TriangleFan); // TODO: theres a strange bug, see renderdoc.
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

    void graphics::ShapeRenderer::drawRectangle(Renderer *renderer, Vector2 position, float width, float height,
                                                Color color, Angle rotation, Vector2 origin, bool outline,
                                                float lineThickness) {
        drawRectangle(renderer, {position, width, height}, color, rotation, origin, outline, lineThickness);
    }

    void graphics::ShapeRenderer::drawRectangle(Renderer *renderer, Rectangle rect, Color color, Angle rotation,
                                                Vector2 origin, bool outline, float lineThickness) {
        // Get origin in pixel units
        auto pixelOrigin = Vector2(origin.X * rect.Width, origin.Y * rect.Height);

        // Push a matrix and set up our transform/rotation
        renderer->setTexture(nullptr);
        renderer->pushMatrix();

        // Move to destination
        renderer->translate({rect.X, rect.Y, 0});

        // Apply rotation (this fixes issues with different Y stuffs)
        if (renderer->getCoordinateSystem() == CoordinateSystem::GUI)
            renderer->rotate(rotation, {0, 0, 1});
        else renderer->rotate(rotation, {0, 0, -1});

        // Fix origin
        renderer->translate({-pixelOrigin.X, -pixelOrigin.Y, 0});

        // Get correct dimensions
        float x1, y1, x2, y2;

        // Set common values
        x1 = 0;
        x2 = rect.Width;

        if (renderer->getCoordinateSystem() != CoordinateSystem::GUI) {
            // Start in top-left
            y1 = rect.Height;
            y2 = 0;
        } else {
            y1 = 0;
            y2 = rect.Height;
        }

        if (outline) {
            // Draw lines
            renderer->beginVertices(PrimitiveType::Quads);
            _drawLine(renderer, {x1, y1}, {x2, y1}, color, lineThickness);
            _drawLine(renderer, {x2, y1}, {x2, y2}, color, lineThickness);
            _drawLine(renderer, {x2, y2}, {x1, y2}, color, lineThickness);
            _drawLine(renderer, {x1, y2}, {x1, y1}, color, lineThickness);
            renderer->endVertices();
        } else {
            renderer->beginVertices(PrimitiveType::Quads);
            renderer->pushVertex({{x1, y1, 0}, {0, 0}, color});
            renderer->pushVertex({{x2, y1, 0}, {1, 0}, color});
            renderer->pushVertex({{x2, y2, 0}, {1, 1}, color});
            renderer->pushVertex({{x1, y2, 0}, {0, 1}, color});
            renderer->endVertices();
        }
        renderer->popMatrix();
    }

    void graphics::ShapeRenderer::drawTriangle(Renderer *renderer, Vector2 v1, Vector2 v2, Vector2 v3, Color color,
                                               Angle rotation, Vector2 origin, bool outline, float lineThickness) {
        // Push matrix (for rotation)
        renderer->setTexture(nullptr);
        renderer->pushMatrix();
        renderer->translate({-origin.X, -origin.Y, 0});
        renderer->rotate(rotation, {0, 0, 1});
        renderer->translate({v1.X, v1.Y, 0});

        Vector2 a = {0, 0};
        auto b = v2 - v1;
        auto c = v3 - v1;

        // Correct dimensions
        float x1, y1, x2, y2, x3, y3;

        // Common values
        x1 = v1.X;
        x2 = v2.X;
        x3 = v3.X;
        y1 = v1.Y;
        y2 = v2.Y;
        y3 = v3.Y;

        // TODO: Do we make this coordinate system aware?

        if (outline) {
            // Render with lines
            renderer->beginVertices(PrimitiveType::Quads);
            _drawLine(renderer, {x1, y1}, {x2, y2}, color, lineThickness);
            _drawLine(renderer, {x2, y2}, {x3, y3}, color, lineThickness);
            _drawLine(renderer, {x3, y3}, {x1, y1}, color, lineThickness);
            renderer->endVertices();
        } else {
            // Render it.
            renderer->beginVertices(PrimitiveType::Triangles);
            renderer->pushVertex({{x1, y1, 0}, {}, color});
            renderer->pushVertex({{x2, y2, 0}, {}, color});
            renderer->pushVertex({{x3, y3, 0}, {}, color});
            renderer->endVertices();
        }

        // Pop
        renderer->popMatrix();
    }

    void
    ShapeRenderer::drawPolygon(Renderer *renderer, Vector2 position, const std::vector<Vector2> &vertices, Color color,
                               Angle rotation, Vector2 origin, bool outline, float lineThickness) {
        renderer->setTexture(nullptr);

        // Push matrix
        renderer->pushMatrix();

        // Move into position (not top left yet!!!)
        renderer->translate({position.X, position.Y, 0});

        // Apply rotation (this fixes issues with different Y stuffs)
        if (renderer->getCoordinateSystem() == CoordinateSystem::GUI)
            renderer->rotate(rotation, {0, 0, 1});
        else renderer->rotate(rotation, {0, 0, -1});

        // Fix origin (Flipping Y for bottom-origin renders)
        if (renderer->getCoordinateSystem() == CoordinateSystem::GUI)
            renderer->translate({-origin.X, -origin.Y, 0});
        else renderer->translate({-origin.X, origin.Y, 0});

        // TODO: Deal with different rotations etc.

        if (outline) {
            // TODO: Check vertex count!!
            renderer->beginVertices(PrimitiveType::Quads);

            for (auto i = -1; i < vertices.size() - 1; i++) {
                // Get vertices
                Vector2 v1;
                Vector2 v2;
                if (i == -1) {
                    v1 = vertices[vertices.size() - 1];
                    v2 = vertices[0];
                } else {
                    v1 = vertices[i];
                    v2 = vertices[i + 1];
                }

                // Convert
                float x1, y1, x2, y2;

                // Common
                x1 = v1.X;
                x2 = v2.X;

                // Get Y
                if (renderer->getCoordinateSystem() == CoordinateSystem::GUI) {
                    y1 = v1.Y;
                    y2 = v2.Y;
                } else {
                    y1 = -v1.Y;
                    y2 = -v2.Y;
                }

                // Draw
                _drawLine(renderer, {x1, y1}, {x2, y2}, color, lineThickness);
            }

            renderer->endVertices();
        } else {
            // TODO: Check vertex count!!
            renderer->beginVertices(PrimitiveType::TriangleFan);

            // Push center
            if (renderer->getCoordinateSystem() == CoordinateSystem::GUI) {
                renderer->pushVertex({{vertices[0].X, vertices[0].Y, 0}, {}, color});
            } else {
                renderer->pushVertex({{vertices[0].X, vertices[0].Y, 0}, {}, color});
            }

            for (auto i = 1; i < vertices.size() - 1; i++) {
                // Get vertices
                auto v1 = vertices[i];
                auto v2 = vertices[i + 1];;

                // Push
                renderer->pushVertex({{v1.X, v1.Y, 0}, {}, color});
                renderer->pushVertex({{v2.X, v2.Y, 0}, {}, color});
            }
            renderer->endVertices();
        }

        // Pop
        renderer->popMatrix();
    }
}
