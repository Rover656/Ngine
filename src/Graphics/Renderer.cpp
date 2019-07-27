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

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL33)
#include "OpenGL/OpenGL.h"
#endif

#include "GraphicsManager.h"

namespace NerdThings::Ngine::Graphics {
    // OpenGL Implementation

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL33)

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
        OpenGL::GL::Begin(OpenGL::PRIMITIVE_LINES);

        OpenGL::GL::Color(color_);
        OpenGL::GL::Vertex(position_);
        OpenGL::GL::Vertex(TVector2(position_.X + 1, position_.Y + 1));

        OpenGL::GL::End();
    }

    void Renderer::DrawLine(TVector2 startPos_, TVector2 endPos_, TColor color_, float thickness_) {
        if (thickness_ < 0) thickness_ = -thickness_; // Cheeky liddle fix
        if (thickness_ == 1.0f) {
            // More basic, less math
            OpenGL::GL::Begin(OpenGL::PRIMITIVE_LINES);

            OpenGL::GL::Color(color_);
            OpenGL::GL::Vertex(startPos_);
            OpenGL::GL::Vertex(endPos_);

            OpenGL::GL::End();
        } else {
            // TODO: Thickness option
        }
    }

    void Renderer::DrawLineStrip(std::vector<TVector2> points_, TColor color_) {

    }

    void Renderer::DrawLineBezier(TVector2 startPos_, TVector2 endPos_, TColor color_, float thickness_) {

    }

    void Renderer::DrawCircle(TVector2 center_, float radius_, TColor color_) {

    }

    void Renderer::DrawCircleGradient(TVector2 center_, float radius_, TColor color1_, TColor color2_) {

    }

    void Renderer::DrawCircleLines(TVector2 center_, float radius_, TColor color_) {

    }

    void Renderer::DrawCircleSector(TVector2 center_, float radius_, int startAngle_, int endAngle_, int segments_,
                                    TColor color_) {

    }

    void Renderer::DrawCircleSectorLines(TVector2 center_, float radius_, int startAngle_, int endAngle_, int segments_,
                                         TColor color_) {

    }

    void Renderer::DrawRing(TVector2 center_, float innerRadius_, float outerRadius_, int startAngle_, int endAngle_,
                            int segments_, TColor color_) {

    }

    void
    Renderer::DrawRingLines(TVector2 center_, float innerRadius_, float outerRadius_, int startAngle_, int endAngle_,
                            int segments_, TColor color_) {

    }

    void Renderer::DrawRectangle(TVector2 position_, float width_, float height_, TColor color_, float rotation_,
                                 TVector2 origin_) {

    }

    void Renderer::DrawRectangle(TVector2 position_, TVector2 size_, TColor color_, float rotation_, TVector2 origin_) {

    }

    void Renderer::DrawRectangle(TRectangle rectangle_, TColor color_, float rotation_, TVector2 origin_) {

    }

    void
    Renderer::DrawRectangleGradientV(TVector2 position_, float width_, float height_, TColor color1_, TColor color2_) {

    }

    void Renderer::DrawRectangleGradientV(TVector2 position_, TVector2 size_, TColor color1_, TColor color2_) {

    }

    void Renderer::DrawRectangleGradientV(TRectangle rectangle_, TColor color1_, TColor color2_) {

    }

    void
    Renderer::DrawRectangleGradientH(TVector2 position_, float width_, float height_, TColor color1_, TColor color2_) {

    }

    void Renderer::DrawRectangleGradientH(TVector2 position_, TVector2 size_, TColor color1_, TColor color2_) {

    }

    void Renderer::DrawRectangleGradientH(TRectangle rectangle_, TColor color1_, TColor color2_) {

    }

    void
    Renderer::DrawRectangleGradientEx(TVector2 position_, float width_, float height_, TColor color1_, TColor color2_,
                                      TColor color3_, TColor color4_) {

    }

    void Renderer::DrawRectangleGradientEx(TVector2 position_, TVector2 size_, TColor color1_, TColor color2_,
                                           TColor color3_, TColor color4_) {

    }

    void Renderer::DrawRectangleGradientEx(TRectangle rectangle_, TColor color1_, TColor color2_, TColor color3_,
                                           TColor color4_) {

    }

    void
    Renderer::DrawRectangleLines(TVector2 position_, float width_, float height_, TColor color_, int lineThickness_) {

    }

    void Renderer::DrawRectangleLines(TVector2 position_, TVector2 size_, TColor color_, int lineThickness_) {

    }

    void Renderer::DrawRectangleLines(TRectangle rectangle_, TColor color_, int lineThickness_) {

    }

    void
    Renderer::DrawText(TFont font_, const std::string &string_, TVector2 position_, float fontSize_, float spacing_,
                       TColor color_) {

    }

    void Renderer::DrawTexture(TTexture2D texture_, TVector2 position_, TColor color_, float scale_, TVector2 origin_,
                               float rotation_) {
        DrawTexture(texture_, {0, 0, (float)texture_.Width, (float)texture_.Height}, position_, texture_.Width, texture_.Height, color_, origin_, rotation_);
    }

    void Renderer::DrawTexture(TTexture2D texture_, TVector2 position_, float width_, float height_, TColor color_,
                               TVector2 origin_, float rotation_) {
        DrawTexture(texture_, {0, 0, (float)texture_.Width, (float)texture_.Height}, position_, width_, height_, color_, origin_, rotation_);
    }

    void Renderer::DrawTexture(TTexture2D texture_, TRectangle sourceRectangle_, TVector2 position_, TColor color_,
                               TVector2 origin_, float rotation_) {
        DrawTexture(texture_, sourceRectangle_, position_, texture_.Width, texture_.Height, color_, origin_, rotation_);
    }

    void Renderer::DrawTexture(TTexture2D texture_, TRectangle sourceRectangle_, TVector2 position_, float width_,
                               float height_, TColor color_, TVector2 origin_, float rotation_) {
        DrawTexture(std::move(texture_), {position_, {width_, height_}}, sourceRectangle_, color_, origin_, rotation_);
    }

    void
    Renderer::DrawTexture(TTexture2D texture_, TRectangle destRectangle_, TRectangle sourceRectangle_, TColor color_,
                          TVector2 origin_, float rotation_) {
        // Raylib implementation
        if (texture_.IsValid()) {
            float width = (float)texture_.Width;
            float height = (float)texture_.Height;

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
            if (flipX) OpenGL::GL::TexCoord({(sourceRectangle_.X + sourceRectangle_.Width)/width, sourceRectangle_.Y/height});
            else OpenGL::GL::TexCoord({sourceRectangle_.X/width, sourceRectangle_.Y/height});
            OpenGL::GL::Vertex(TVector2(0.0f, 0.0f));

            // Bottom-right corner for texture and quad
            if (flipX) OpenGL::GL::TexCoord({(sourceRectangle_.X + sourceRectangle_.Width)/width, (sourceRectangle_.Y + sourceRectangle_.Height)/height});
            else OpenGL::GL::TexCoord({sourceRectangle_.X/width, (sourceRectangle_.Y + sourceRectangle_.Height)/height});
            OpenGL::GL::Vertex(TVector2(0.0f, destRectangle_.Height));

            // Top-right corner for texture and quad
            if (flipX) OpenGL::GL::TexCoord({sourceRectangle_.X/width, (sourceRectangle_.Y + sourceRectangle_.Height)/height});
            else OpenGL::GL::TexCoord({(sourceRectangle_.X + sourceRectangle_.Width)/width, (sourceRectangle_.Y + sourceRectangle_.Height)/height});
            OpenGL::GL::Vertex(TVector2(destRectangle_.Width, destRectangle_.Height));

            // Top-left corner for texture and quad
            if (flipX) OpenGL::GL::TexCoord({sourceRectangle_.X/width, sourceRectangle_.Y/height});
            else OpenGL::GL::TexCoord({(sourceRectangle_.X + sourceRectangle_.Width)/width, sourceRectangle_.Y/height});
            OpenGL::GL::Vertex(TVector2(destRectangle_.Width, 0.0f));

            OpenGL::GL::End();

            OpenGL::GL::PopMatrix();
        }
    }

    void Renderer::DrawTriangle(TVector2 v1_, TVector2 v2_, TVector2 v3_, TColor color_) {

    }

    void Renderer::DrawTriangleLines(TVector2 v1_, TVector2 v2_, TVector2 v3_, TColor color_) {

    }

    void Renderer::DrawTriangleFan(std::vector<TVector2> points_, TColor color_) {

    }

    void Renderer::DrawPoly(TVector2 center_, int sides_, float radius_, float rotation_, TColor color_) {

    }

    void Renderer::EndDrawing() {
        // Draw all batches
        OpenGL::GL::Draw();
    }

#endif
}
