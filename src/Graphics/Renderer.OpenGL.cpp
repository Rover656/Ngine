/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Renderer.h"

#include <utility>

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)

#include "OpenGL/OpenGL.h"

#endif

#include "GraphicsManager.h"

namespace NerdThings::Ngine::Graphics {
    // OpenGL Implementation

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)

#define CIRCLE_ERROR_RATE 0.5f

    void Renderer::__DrawLine(TVector2 a_, TVector2 b_, TColor c_, float thick_) {
        // Swap if start pos is past end pos
        if (a_.X > b_.X) {
            auto tmp = a_;
            a_ = b_;
            b_ = tmp;
        }

        auto dx = b_.X - a_.X;
        auto dy = b_.Y - a_.Y;

        float d = sqrtf(dx * dx + dy * dy);
        float angle = asinf(dy / d);

        OpenGL::GL::MatrixMode(OpenGL::MATRIX_MODELVIEW);
        OpenGL::GL::PushMatrix();

        OpenGL::GL::Translate({a_.X, a_.Y, 0.0f});
        OpenGL::GL::Rotate(angle, {0.0f, 0.0f, 1.0f});
        OpenGL::GL::Translate({0, (thick_ > 1.0f) ? -thick_ / 2.0f : -1.0f, 0.0f});

        OpenGL::GL::Color(c_);

        OpenGL::GL::TexCoord({0, 0});
        OpenGL::GL::Vertex({0, 0});

        OpenGL::GL::TexCoord({0, 1});
        OpenGL::GL::Vertex({0, thick_});

        OpenGL::GL::TexCoord({1, 1});
        OpenGL::GL::Vertex({d, thick_});

        OpenGL::GL::TexCoord({1, 0});
        OpenGL::GL::Vertex({d, 0});

        OpenGL::GL::PopMatrix();
    }

    void Renderer::BeginDrawing() {
        // Setup framebuffer
        GraphicsManager::SetupFramebuffer();
    }

    void Renderer::Clear(TColor color_) {
        // Set clear color
        OpenGL::GL::ClearColor(color_);

        // Clear
        OpenGL::GL::Clear();
    }

    void Renderer::DrawPixel(TVector2 position_, TColor color_) {
        DrawLine(position_, {position_.X + 1, position_.Y + 1}, color_);
    }

    void Renderer::DrawLine(TVector2 startPos_, TVector2 endPos_, TColor color_, float lineThickness_) {
        if (lineThickness_ < 0) lineThickness_ = -lineThickness_; // Cheeky liddle fix

        OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

        OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

        __DrawLine(startPos_, endPos_, color_, lineThickness_);

        OpenGL::GL::End();

        OpenGL::GL::StopUsingTexture();
    }

    void Renderer::DrawLineStrip(const std::vector<TVector2> &points_, TColor color_, float lineThickness_) {
        if (points_.size() >= 2) {
            if (OpenGL::GL::AtBufferLimit(points_.size() * 2)) OpenGL::GL::Draw();

            OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

            OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

            for (auto i = 0; i < points_.size() - 1; i++) {
                auto a = points_[i];
                auto b = points_[i + 1];

                __DrawLine(a, b, color_, lineThickness_);
            }

            OpenGL::GL::End();

            OpenGL::GL::StopUsingTexture();
        }
    }

    void Renderer::DrawCircle(TVector2 center_, float radius_, TColor color_) {
        DrawCircleSector(center_, radius_, 0, DegToRad(360), 36, color_);
    }

    void Renderer::DrawCircleLines(TVector2 center_, float radius_, TColor color_, float lineThickness_) {
        DrawCircleSectorLines(center_, radius_, 0, DegToRad(360), 36, color_, lineThickness_);
    }

    void Renderer::DrawCircleSector(TVector2 center_, float radius_, float startAngle_, float endAngle_, int segments_,
                                    TColor color_) {
        if (radius_ <= 0.0f) radius_ = 0.1f;

        // Fix angles if wrong way around
        if (endAngle_ < startAngle_) {
            // Swap values
            auto tmp = startAngle_;
            startAngle_ = endAngle_;
            endAngle_ = tmp;
        }

        if (segments_ < 4) {
            // Calculate max angle
            auto theta = acosf(2 * powf(1 - CIRCLE_ERROR_RATE / radius_, 2));
            segments_ = (endAngle_ - startAngle_) * ceilf(2 * PI / theta) / DegToRad(360);

            if (segments_ <= 4) segments_ = 4;
        }

        auto stepLen = (float) (endAngle_ - startAngle_) / (float) segments_;
        auto angle = startAngle_;

        if (OpenGL::GL::AtBufferLimit(4 * segments_ / 2)) OpenGL::GL::Draw();

        OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

        OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

        // 2 segments per quad
        for (auto i = 0; i < segments_ / 2; i++) {
            OpenGL::GL::Color(color_);

            OpenGL::GL::TexCoord({0, 0});
            OpenGL::GL::Vertex(center_);

            OpenGL::GL::TexCoord({0, 1});
            OpenGL::GL::Vertex({center_.X + sinf(angle) * radius_, center_.Y + cosf(angle) * radius_});

            OpenGL::GL::TexCoord({1, 1});
            OpenGL::GL::Vertex(
                    {center_.X + sinf(angle + stepLen) * radius_, center_.Y + cosf(angle + stepLen) * radius_});

            OpenGL::GL::TexCoord({1, 0});
            OpenGL::GL::Vertex(
                    {center_.X + sinf(angle + stepLen * 2) * radius_, center_.Y + cosf(angle + stepLen * 2) * radius_});

            angle += (stepLen * 2);
        }

        // If odd, add another segment
        if (segments_ % 2) {
            OpenGL::GL::Color(color_);

            OpenGL::GL::TexCoord({0, 0});
            OpenGL::GL::Vertex(center_);

            OpenGL::GL::TexCoord({0, 1});
            OpenGL::GL::Vertex({center_.X + sinf(angle) * radius_, center_.Y + cosf(angle) * radius_});

            OpenGL::GL::TexCoord({1, 1});
            OpenGL::GL::Vertex(
                    {center_.X + sinf(angle + stepLen) * radius_, center_.Y + cosf(angle + stepLen) * radius_});

            OpenGL::GL::TexCoord({1, 0});
            OpenGL::GL::Vertex(center_);
        }

        OpenGL::GL::End();

        OpenGL::GL::StopUsingTexture();
    }

    void
    Renderer::DrawCircleSectorLines(TVector2 center_, float radius_, float startAngle_, float endAngle_, int segments_,
                                    TColor color_, float lineThickness_) {
        if (radius_ <= 0.0f) radius_ = 0.1f;

        if (endAngle_ < startAngle_) {
            auto tmp = startAngle_;
            startAngle_ = endAngle_;
            endAngle_ = tmp;
        }

        if (segments_ < 4) {
            auto theta = acosf(2 * powf(1 - CIRCLE_ERROR_RATE / radius_, 2) - 1);
            segments_ = (endAngle_ - startAngle_) * ceilf(2 * PI / theta) / DegToRad(360);

            if (segments_ <= 0) segments_ = 4;
        }

        auto stepLen = (float) (endAngle_ - startAngle_) / (float) segments_;
        auto angle = startAngle_;

        // Cap lines
        auto showCapLines = true;
        auto limit = 4 * (segments_ + 2);

        float threeSixty = DegToRad(360);
        if (fmod((endAngle_ - startAngle_), threeSixty) == 0) {
            limit = 4 * segments_;
            showCapLines = false;
        }

        if (OpenGL::GL::AtBufferLimit(limit)) OpenGL::GL::Draw();

        OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

        OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

        if (showCapLines) {
            __DrawLine(center_, {center_.X + sinf(angle) * radius_, center_.Y + cosf(angle) * radius_}, color_,
                       lineThickness_);
        }

        for (auto i = 0; i < segments_; i++) {
            __DrawLine({center_.X + sinf(angle) * radius_, center_.Y + cosf(angle) * radius_},
                       {center_.X + sinf(angle + stepLen) * radius_, center_.Y + cosf(angle + stepLen) * radius_},
                       color_, lineThickness_);

            angle += stepLen;
        }

        if (showCapLines) {
            __DrawLine(center_, {center_.X + sinf(angle) * radius_, center_.Y + cosf(angle) * radius_}, color_,
                       lineThickness_);
        }

        OpenGL::GL::End();

        OpenGL::GL::StopUsingTexture();
    }

//    void Renderer::DrawRing(TVector2 center_, float innerRadius_, float outerRadius_, int startAngle_, int endAngle_,
//                            int segments_, TColor color_) {
//
//    }
//
//    void
//    Renderer::DrawRingLines(TVector2 center_, float innerRadius_, float outerRadius_, int startAngle_, int endAngle_,
//                            int segments_, TColor color_) {
//
//    }

    void Renderer::DrawRectangle(TVector2 position_, float width_, float height_, TColor color_, float rotation_,
                                 TVector2 origin_) {
        DrawRectangle(position_, {width_, height_}, color_, rotation_);
    }

    void Renderer::DrawRectangle(TVector2 position_, TVector2 size_, TColor color_, float rotation_, TVector2 origin_) {
        DrawRectangle({position_.X, position_.Y, size_.X, size_.Y}, color_, rotation_, origin_);
    }

    void Renderer::DrawRectangle(TRectangle rectangle_, TColor color_, float rotation_, TVector2 origin_) {
        if (OpenGL::GL::AtBufferLimit(4)) OpenGL::GL::Draw();

        OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

        OpenGL::GL::MatrixMode(OpenGL::MATRIX_MODELVIEW);
        OpenGL::GL::PushMatrix();

        OpenGL::GL::Translate({rectangle_.X, rectangle_.Y, 0});
        OpenGL::GL::Rotate(rotation_, {0, 0, 1.0f});
        OpenGL::GL::Translate({-origin_.X, -origin_.Y, 0});

        OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

        OpenGL::GL::Color(color_);

        OpenGL::GL::Vertex({0, 0});
        OpenGL::GL::Vertex({0, rectangle_.Height});
        OpenGL::GL::Vertex({rectangle_.Width, rectangle_.Height});
        OpenGL::GL::Vertex({rectangle_.Width, 0});

        OpenGL::GL::End();

        OpenGL::GL::PopMatrix();

        OpenGL::GL::StopUsingTexture();
    }

    void
    Renderer::DrawRectangleGradientV(TVector2 position_, float width_, float height_, TColor color1_, TColor color2_,
                                     float rotation_, TVector2 origin_) {
        DrawRectangleGradientV(position_, {width_, height_}, color1_, color2_, rotation_, origin_);
    }

    void Renderer::DrawRectangleGradientV(TVector2 position_, TVector2 size_, TColor color1_, TColor color2_,
                                          float rotation_, TVector2 origin_) {
        DrawRectangleGradientV({position_, size_}, color1_, color2_, rotation_, origin_);
    }

    void Renderer::DrawRectangleGradientV(TRectangle rectangle_, TColor color1_, TColor color2_, float rotation_,
                                          TVector2 origin_) {
        DrawRectangleGradientEx(rectangle_, color1_, color2_, color2_, color1_, rotation_, origin_);
    }

    void
    Renderer::DrawRectangleGradientH(TVector2 position_, float width_, float height_, TColor color1_, TColor color2_,
                                     float rotation_, TVector2 origin_) {
        DrawRectangleGradientH(position_, {width_, height_}, color1_, color2_, rotation_, origin_);
    }

    void Renderer::DrawRectangleGradientH(TVector2 position_, TVector2 size_, TColor color1_, TColor color2_,
                                          float rotation_, TVector2 origin_) {
        DrawRectangleGradientH({position_, size_}, color1_, color2_, rotation_, origin_);
    }

    void Renderer::DrawRectangleGradientH(TRectangle rectangle_, TColor color1_, TColor color2_, float rotation_,
                                          TVector2 origin_) {
        DrawRectangleGradientEx(rectangle_, color1_, color1_, color2_, color2_, rotation_, origin_);
    }

    void
    Renderer::DrawRectangleGradientEx(TVector2 position_, float width_, float height_, TColor color1_, TColor color2_,
                                      TColor color3_, TColor color4_, float rotation_, TVector2 origin_) {
        DrawRectangleGradientEx(position_, {width_, height_}, color1_, color2_, color3_, color4_, rotation_, origin_);
    }

    void Renderer::DrawRectangleGradientEx(TVector2 position_, TVector2 size_, TColor color1_, TColor color2_,
                                           TColor color3_, TColor color4_, float rotation_, TVector2 origin_) {
        DrawRectangleGradientEx({position_, size_}, color1_, color2_, color3_, color4_, rotation_, origin_);
    }

    void Renderer::DrawRectangleGradientEx(TRectangle rectangle_, TColor color1_, TColor color2_, TColor color3_,
                                           TColor color4_, float rotation_, TVector2 origin_) {
        if (OpenGL::GL::AtBufferLimit(4)) OpenGL::GL::Draw();

        OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

        OpenGL::GL::PushMatrix();

        OpenGL::GL::Translate({rectangle_.X, rectangle_.Y, 0});
        OpenGL::GL::Rotate(rotation_, {0, 0, 1.0f});
        OpenGL::GL::Translate({-origin_.X, -origin_.Y, 0});

        OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

        OpenGL::GL::Color(color1_);
        OpenGL::GL::TexCoord({0, 0});
        OpenGL::GL::Vertex({0, 0});

        OpenGL::GL::Color(color2_);
        OpenGL::GL::TexCoord({0, 1});
        OpenGL::GL::Vertex({0, rectangle_.Height});

        OpenGL::GL::Color(color3_);
        OpenGL::GL::TexCoord({1, 1});
        OpenGL::GL::Vertex({rectangle_.Width, rectangle_.Height});

        OpenGL::GL::Color(color4_);
        OpenGL::GL::TexCoord({1, 0});
        OpenGL::GL::Vertex({rectangle_.Width, 0});

        OpenGL::GL::End();

        OpenGL::GL::PopMatrix();

        OpenGL::GL::StopUsingTexture();
    }

    void
    Renderer::DrawRectangleLines(TVector2 position_, float width_, float height_, TColor color_, int lineThickness_) {
        DrawRectangleLines(position_, {width_, height_}, color_, lineThickness_);
    }

    void Renderer::DrawRectangleLines(TVector2 position_, TVector2 size_, TColor color_, int lineThickness_) {
        DrawRectangleLines({position_, size_}, color_, lineThickness_);
    }

    void Renderer::DrawRectangleLines(TRectangle rectangle_, TColor color_, int lineThickness_) {
        if (OpenGL::GL::AtBufferLimit(4 * 4)) OpenGL::GL::Draw();

        OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

        OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

        __DrawLine({rectangle_.X, rectangle_.Y}, {rectangle_.X + rectangle_.Width, rectangle_.Y}, color_,
                   lineThickness_);
        __DrawLine({rectangle_.X + rectangle_.Width, rectangle_.Y},
                   {rectangle_.X + rectangle_.Width, rectangle_.Y + rectangle_.Height}, color_, lineThickness_);
        __DrawLine({rectangle_.X + rectangle_.Width, rectangle_.Y + rectangle_.Height},
                   {rectangle_.X, rectangle_.Y + rectangle_.Height}, color_, lineThickness_);
        __DrawLine({rectangle_.X, rectangle_.Y + rectangle_.Height}, {rectangle_.X, rectangle_.Y}, color_,
                   lineThickness_);

        OpenGL::GL::End();

        OpenGL::GL::StopUsingTexture();
    }

    void
    Renderer::DrawText(TFont font_, const std::string &string_, TVector2 position_, float fontSize_, float spacing_,
                       TColor color_) {
        // TODO: Text support
    }

    void
    Renderer::DrawTexture(const TTexture2D &texture_, TVector2 position_, TColor color_, float scale_, TVector2 origin_,
                          float rotation_) {
        DrawTexture(texture_, {0, 0, (float) texture_.Width, (float) texture_.Height}, position_, texture_.Width,
                    texture_.Height, color_, origin_, rotation_);
    }

    void
    Renderer::DrawTexture(const TTexture2D &texture_, TVector2 position_, float width_, float height_, TColor color_,
                          TVector2 origin_, float rotation_) {
        DrawTexture(texture_, {0, 0, (float) texture_.Width, (float) texture_.Height}, position_, width_, height_,
                    color_, origin_, rotation_);
    }

    void
    Renderer::DrawTexture(const TTexture2D &texture_, TRectangle sourceRectangle_, TVector2 position_, TColor color_,
                          TVector2 origin_, float rotation_) {
        DrawTexture(texture_, sourceRectangle_, position_, sourceRectangle_.Width, sourceRectangle_.Height, color_,
                    origin_, rotation_);
    }

    void
    Renderer::DrawTexture(const TTexture2D &texture_, TRectangle sourceRectangle_, TVector2 position_, float width_,
                          float height_, TColor color_, TVector2 origin_, float rotation_) {
        DrawTexture(texture_, {position_, {width_, height_}}, sourceRectangle_, color_, origin_, rotation_);
    }

    void
    Renderer::DrawTexture(const TTexture2D &texture_, TRectangle destRectangle_, TRectangle sourceRectangle_,
                          TColor color_,
                          TVector2 origin_, float rotation_) {
        // Raylib implementation
        if (texture_.IsValid()) {
            float width = (float) texture_.Width;
            float height = (float) texture_.Height;

            bool flipX = false;

            if (sourceRectangle_.Width < 0) {
                flipX = true;
                sourceRectangle_.Width *= -1;
            }

            if (sourceRectangle_.Height < 0) {
                sourceRectangle_.Y -= sourceRectangle_.Height;
            }

            OpenGL::GL::UseTexture(texture_.InternalTexture);

            OpenGL::GL::MatrixMode(OpenGL::MATRIX_MODELVIEW);
            OpenGL::GL::PushMatrix();

            OpenGL::GL::Translate({destRectangle_.X, destRectangle_.Y, 0.0f});
            OpenGL::GL::Rotate(rotation_, {0.0f, 0.0f, 1.0f});
            OpenGL::GL::Translate({-origin_.X, -origin_.Y, 0.0f});

            OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

            OpenGL::GL::Color(color_);

            // Bottom-left corner for texture and quad
            if (flipX)
                OpenGL::GL::TexCoord(
                        {(sourceRectangle_.X + sourceRectangle_.Width) / width, sourceRectangle_.Y / height});
            else OpenGL::GL::TexCoord({sourceRectangle_.X / width, sourceRectangle_.Y / height});
            OpenGL::GL::Vertex(TVector2(0.0f, 0.0f));

            // Bottom-right corner for texture and quad
            if (flipX)
                OpenGL::GL::TexCoord({(sourceRectangle_.X + sourceRectangle_.Width) / width,
                                      (sourceRectangle_.Y + sourceRectangle_.Height) / height});
            else
                OpenGL::GL::TexCoord(
                        {sourceRectangle_.X / width, (sourceRectangle_.Y + sourceRectangle_.Height) / height});
            OpenGL::GL::Vertex(TVector2(0.0f, destRectangle_.Height));

            // Top-right corner for texture and quad
            if (flipX)
                OpenGL::GL::TexCoord(
                        {sourceRectangle_.X / width, (sourceRectangle_.Y + sourceRectangle_.Height) / height});
            else
                OpenGL::GL::TexCoord({(sourceRectangle_.X + sourceRectangle_.Width) / width,
                                      (sourceRectangle_.Y + sourceRectangle_.Height) / height});
            OpenGL::GL::Vertex(TVector2(destRectangle_.Width, destRectangle_.Height));

            // Top-left corner for texture and quad
            if (flipX) OpenGL::GL::TexCoord({sourceRectangle_.X / width, sourceRectangle_.Y / height});
            else
                OpenGL::GL::TexCoord(
                        {(sourceRectangle_.X + sourceRectangle_.Width) / width, sourceRectangle_.Y / height});
            OpenGL::GL::Vertex(TVector2(destRectangle_.Width, 0.0f));

            OpenGL::GL::End();

            OpenGL::GL::PopMatrix();

            OpenGL::GL::StopUsingTexture();
        }
    }

    void Renderer::DrawTriangle(TVector2 v1_, TVector2 v2_, TVector2 v3_, TColor color_) {
        // Check for buffer space
        if (OpenGL::GL::AtBufferLimit(4)) OpenGL::GL::Draw();

        // Draw
        OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

        OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

        OpenGL::GL::Color(color_);

        OpenGL::GL::TexCoord({0, 0});
        OpenGL::GL::Vertex(v1_);

        OpenGL::GL::TexCoord({0, 1});
        OpenGL::GL::Vertex(v2_);

        OpenGL::GL::TexCoord({1, 1});
        OpenGL::GL::Vertex(v2_);

        OpenGL::GL::TexCoord({1, 0});
        OpenGL::GL::Vertex(v3_);

        OpenGL::GL::End();

        OpenGL::GL::StopUsingTexture();
    }

    void Renderer::DrawTriangleLines(TVector2 v1_, TVector2 v2_, TVector2 v3_, TColor color_, float lineThickness_) {
        // Check for buffer space
        if (OpenGL::GL::AtBufferLimit(12)) OpenGL::GL::Draw();

        // Draw
        OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

        OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

        __DrawLine(v1_, v2_, color_, lineThickness_);
        __DrawLine(v2_, v3_, color_, lineThickness_);
        __DrawLine(v3_, v1_, color_, lineThickness_);

        OpenGL::GL::End();

        OpenGL::GL::StopUsingTexture();
    }

    void Renderer::DrawTriangleFan(const std::vector<TVector2> &points_, TColor color_) {
        if (points_.size() >= 3) {
            if (OpenGL::GL::AtBufferLimit((points_.size() - 2) * 4)) OpenGL::GL::Draw();

            OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

            OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

            OpenGL::GL::Color(color_);

            for (auto i = 0; i < points_.size() - 1; i++) {
                OpenGL::GL::TexCoord({0, 0});
                OpenGL::GL::Vertex({points_[0].X, points_[0].Y});

                OpenGL::GL::TexCoord({0, 1});
                OpenGL::GL::Vertex({points_[i].X, points_[i].Y});

                OpenGL::GL::TexCoord({1, 1});
                OpenGL::GL::Vertex({points_[i + 1].X, points_[i + 1].Y});

                OpenGL::GL::TexCoord({1, 0});
                OpenGL::GL::Vertex({points_[i + 1].X, points_[i + 1].Y});
            }

            OpenGL::GL::End();

            OpenGL::GL::StopUsingTexture();
        }
    }

    void Renderer::DrawPoly(TVector2 center_, int sides_, float radius_, float rotation_, TColor color_) {
        if (sides_ < 3) sides_ = 3;

        if (OpenGL::GL::AtBufferLimit(4 * (360 / sides_))) OpenGL::GL::Draw();

        OpenGL::GL::PushMatrix();

        OpenGL::GL::Translate({center_.X, center_.Y, 0});
        OpenGL::GL::Rotate(rotation_, {});

        OpenGL::GL::UseTexture(OpenGL::GL::DefaultTexture);

        OpenGL::GL::Begin(OpenGL::PRIMITIVE_QUADS);

        OpenGL::GL::Color(color_);

        for (auto i = 0; i < 360; i += 360 / sides_) {
            OpenGL::GL::TexCoord({0, 0});
            OpenGL::GL::Vertex({0, 0});

            OpenGL::GL::TexCoord({0, 1});
            OpenGL::GL::Vertex({sinf(i) * radius_, cosf(i) * radius_});

            OpenGL::GL::TexCoord({1, 1});
            OpenGL::GL::Vertex({sinf(i) * radius_, cosf(i) * radius_});

            OpenGL::GL::TexCoord({1, 0});
            OpenGL::GL::Vertex({sinf(i + 360.0f / sides_) * radius_, cosf(i + 360.0f / sides_) * radius_});
        }

        OpenGL::GL::End();

        OpenGL::GL::StopUsingTexture();

        OpenGL::GL::PopMatrix();
    }

    void Renderer::EndDrawing() {
        // Draw all batches
        OpenGL::GL::Draw();
    }

#endif
}