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
    void ShapeRenderer::_drawLine(Renderer *renderer_, Vector2 v1_, Vector2 v2_, Color color_, float thickness_) {
        // Flip inputs if X is smaller
        bool flipped = false;
        if (v1_.X > v2_.X) {
            auto tmp = v1_;
            v1_ = v2_;
            v2_ = tmp;
            flipped = true;
        }

        // Get distance and angle
        auto dx = v2_.X - v1_.X;
        auto dy = v2_.Y - v1_.Y;
        float d = sqrtf(dx * dx + dy * dy);
        float angle = asinf(dy / d);

        auto t = RadToDeg(angle);

        // Build translation
        renderer_->pushMatrix();
        renderer_->translate({v1_.X, v1_.Y, 0});
        renderer_->rotate(RadToDeg(angle), {0, 0, 1});
        if (flipped)
            renderer_->translate({thickness_ / 2.0f, -thickness_ / 2.0f, 0});
        else renderer_->translate({-thickness_ / 2.0f, -thickness_ / 2.0f, 0});

        // Write to renderer
        renderer_->pushVertex({{0, 0, 0}, {0, 0}, color_});
        renderer_->pushVertex({{0, thickness_, 0}, {0, 1}, color_});
        renderer_->pushVertex({{d, thickness_, 0}, {1, 1}, color_});
        renderer_->pushVertex({{d, 0, 0}, {1, 0}, color_});

        // Pop matrix
        renderer_->popMatrix();
    }

    void graphics::ShapeRenderer::DrawLine(Renderer *renderer_, Vector2 v1_, Vector2 v2_, Color color_,
                                           float thickness_) {
        renderer_->setTexture(nullptr);
        renderer_->beginVertices(PrimitiveType::Quads);
        _drawLine(renderer_, v1_, v2_, color_, thickness_);
        renderer_->endVertices();
    }

    void
    graphics::ShapeRenderer::DrawCircle(Renderer *renderer_, Vector2 center_, float radius_, Color color_,
                                        bool outline_,
                                        float lineThickness_) {
        renderer_->setTexture(nullptr);
        if (outline_) {
            // Get two times PI
            auto twoPi = 2.0f * PI;
            float step = DegToRad(360.0f / LINES_PER_CIRCLE);

            // Render lines.
            renderer_->beginVertices(PrimitiveType::Quads);
            for (auto i = 0; i < LINES_PER_CIRCLE; i++) {
                _drawLine(renderer_,
                          {center_.X + sinf((float) i * step) * radius_, center_.Y + cosf((float) i * step) * radius_},
                          {center_.X + sinf((float) (i + 1) * step) * radius_,
                           center_.Y + cosf((float) (i + 1) * step) * radius_},
                          color_, lineThickness_);
            }
            renderer_->endVertices();
        } else {
            // Get two times PI
            float twoPi = 2.0f * PI;

            // Render using triangle fan primitive type (NEW!!!)
            renderer_->beginVertices(PrimitiveType::TriangleFan);
            renderer_->pushVertex({{center_.X, center_.Y, 0}, {0, 0}, color_});
            for (auto i = 1; i <= TRIANGLES_PER_CIRCLE + 1; i++) {
                renderer_->pushVertex({{
                                               center_.X + (radius_ * cosf(i * twoPi / TRIANGLES_PER_CIRCLE)),
                                               center_.Y + (radius_ * sinf(i * twoPi / TRIANGLES_PER_CIRCLE)), 0
                                       }, {0, 0}, color_});
            }
            renderer_->endVertices();
        }
    }

    void graphics::ShapeRenderer::DrawRectangle(Renderer *renderer_, Vector2 position_, float width_, float height_,
                                                Color color_, Angle rotation_, Vector2 origin_, bool outline_,
                                                float lineThickness_) {
        DrawRectangle(renderer_, {position_, width_, height_}, color_, rotation_, origin_, outline_, lineThickness_);
    }

    void graphics::ShapeRenderer::DrawRectangle(Renderer *renderer_, Rectangle rect_, Color color_, Angle rotation_,
                                                Vector2 origin_, bool outline_, float lineThickness_) {
        // Push a matrix and set up our transform/rotation
        renderer_->setTexture(nullptr);
        renderer_->pushMatrix();
        renderer_->translate({rect_.X, rect_.Y, 0});
        renderer_->rotate(rotation_, {0, 0, 1});
        renderer_->translate({-origin_.X, -origin_.Y, 0});

        if (outline_) {
            // Draw lines
            renderer_->beginVertices(PrimitiveType::Quads);
            _drawLine(renderer_, {0, 0}, {rect_.Width, 0}, color_, lineThickness_);
            _drawLine(renderer_, {rect_.Width, 0}, {rect_.Width, rect_.Height}, color_, lineThickness_);
            _drawLine(renderer_, {rect_.Width, rect_.Height}, {0, rect_.Height}, color_, lineThickness_);
            _drawLine(renderer_, {0, rect_.Height}, {0, 0}, color_, lineThickness_);
            renderer_->endVertices();
        } else {
            renderer_->beginVertices(PrimitiveType::Quads);
            renderer_->pushVertex({{0, 0, 0}, {0, 0}, color_});
            renderer_->pushVertex({{rect_.Width, 0, 0}, {1, 0}, color_});
            renderer_->pushVertex({{rect_.Width, rect_.Height, 0}, {1, 1}, color_});
            renderer_->pushVertex({{0, rect_.Height, 0}, {0, 1}, color_});
            renderer_->endVertices();
        }
        renderer_->popMatrix();
    }

    void graphics::ShapeRenderer::DrawTriangle(Renderer *renderer_, Vector2 v1_, Vector2 v2_, Vector2 v3_, Color color_,
                                               Angle rotation_, Vector2 origin_, bool outline_, float lineThickness_) {
        // Push matrix (for rotation)
        renderer_->setTexture(nullptr);
        renderer_->pushMatrix();
        renderer_->translate({v1_.X, v1_.Y, 0});
        renderer_->rotate(rotation_, {0, 0, 1});
        renderer_->translate({-origin_.X, -origin_.Y, 0});

        Vector2 a = {0, 0};
        auto b = v2_ - v1_;
        auto c = v3_ - v1_;

        if (outline_) {
            // Render with lines
            renderer_->beginVertices(PrimitiveType::Quads);
            _drawLine(renderer_, a, b, color_, lineThickness_);
            _drawLine(renderer_, b, c, color_, lineThickness_);
            _drawLine(renderer_, c, a, color_, lineThickness_);
            renderer_->endVertices();
        } else {
            // Render it.
            renderer_->beginVertices(PrimitiveType::Triangles);
            renderer_->pushVertex({{a.X, a.Y, 0}, {}, color_});
            renderer_->pushVertex({{b.X, b.Y, 0}, {}, color_});
            renderer_->pushVertex({{c.X, c.Y, 0}, {}, color_});
            renderer_->endVertices();
        }

        // Pop
        renderer_->popMatrix();
    }
}