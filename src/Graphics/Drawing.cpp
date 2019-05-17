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

#include "Drawing.h"

namespace Ngine::Graphics {
    // Public Methods

    void Drawing::BeginCamera2D(TCamera2D *camera_) {
        BeginMode2D(camera_->ToRaylibCam());
    }

    void Drawing::BeginDrawing() {
        ::BeginDrawing();
    }

    void Drawing::Clear(TColor color_) {
        auto c = color_.ToRaylibColor();
        ClearBackground(color_.ToRaylibColor());
    }

    void Drawing::DrawPixel(TVector2 position_, TColor color_) {
        DrawPixelV(position_.ToRaylibVec(),
                   color_.ToRaylibColor());
    }


    void Drawing::DrawLine(TVector2 startPos_, TVector2 endPos_, TColor color_, float thickness_) {
        DrawLineEx(startPos_.ToRaylibVec(),
                   endPos_.ToRaylibVec(),
                   thickness_,
                   color_.ToRaylibColor());
    }

    void Drawing::DrawLineStrip(std::vector<Types::TVector2> points_, Types::TColor color_) {
        //Build points array
        auto pts = new Vector2[points_.size()];
        for (auto i = 0; i < points_.size(); i++) {
            pts[i] = points_[i].ToRaylibVec();
        }

        //Draw
        ::DrawLineStrip(pts,
                        points_.size(),
                        color_.ToRaylibColor());

        //Delete points array
        delete[] pts;
    }

    void Drawing::DrawLineBezier(TVector2 startPos_, TVector2 endPos_, TColor color_, float thickness_) {
        ::DrawLineBezier(startPos_.ToRaylibVec(),
                         endPos_.ToRaylibVec(),
                         thickness_,
                         color_.ToRaylibColor());
    }

    void Drawing::DrawCircle(TVector2 center_, float radius_, TColor color_) {
        DrawCircleV(center_.ToRaylibVec(),
                    radius_,
                    color_.ToRaylibColor());
    }

    void Drawing::DrawCircleGradient(TVector2 center_, float radius_, TColor color1_, TColor color2_) {
        ::DrawCircleGradient(static_cast<int>(center_.X),
                             static_cast<int>(center_.Y),
                             radius_,
                             color1_.ToRaylibColor(),
                             color2_.ToRaylibColor());
    }

    void Drawing::DrawCircleLines(TVector2 center_, float radius_, TColor color_) {
        ::DrawCircleLines(static_cast<int>(center_.X),
                          static_cast<int>(center_.Y),
                          radius_,
                          color_.ToRaylibColor());
    }

    void Drawing::DrawCircleSector(TVector2 center_, float radius_, int startAngle_, int endAngle_, int segments_,
                                   TColor color_) {
        ::DrawCircleSector(center_.ToRaylibVec(),
                           radius_,
                           startAngle_,
                           endAngle_,
                           segments_,
                           color_.ToRaylibColor());
    }

    void Drawing::DrawCircleSectorLines(TVector2 center_, float radius_, int startAngle_, int endAngle_, int segments_,
                                        TColor color_) {
        ::DrawCircleSectorLines(center_.ToRaylibVec(),
                                radius_,
                                startAngle_,
                                endAngle_,
                                segments_,
                                color_.ToRaylibColor());
    }

    void Drawing::DrawFPS(TVector2 position_) {
        ::DrawFPS(static_cast<int>(position_.X),
                  static_cast<int>(position_.Y));
    }

    void Drawing::DrawRing(TVector2 center_, float innerRadius_, float outerRadius_, int startAngle_, int endAngle_,
                           int segments_, TColor color_) {
        ::DrawRing(center_.ToRaylibVec(),
                   innerRadius_,
                   outerRadius_,
                   startAngle_,
                   endAngle_,
                   segments_,
                   color_.ToRaylibColor());
    }

    void Drawing::DrawRingLines(TVector2 center_, float innerRadius_, float outerRadius_, int startAngle_,
                                int endAngle_, int segments_, TColor color_) {
        ::DrawRingLines(center_.ToRaylibVec(),
                        innerRadius_,
                        outerRadius_,
                        startAngle_,
                        endAngle_,
                        segments_,
                        color_.ToRaylibColor());
    }

    void Drawing::DrawRectangle(TVector2 position_, float width_, float height_, TColor color_, float rotation_,
                                TVector2 origin_) {
        DrawRectangle({position_, width_, height_},
                      color_,
                      rotation_,
                      origin_);
    }

    void Drawing::DrawRectangle(TVector2 position_, TVector2 size_, TColor color_, float rotation_, TVector2 origin_) {
        DrawRectangle({position_, size_},
                      color_,
                      rotation_,
                      origin_);
    }

    void Drawing::DrawRectangle(TRectangle rectangle_, TColor color_, float rotation_, TVector2 origin_) {
        DrawRectanglePro(rectangle_.ToRaylibRect(),
                         origin_.ToRaylibVec(),
                         rotation_,
                         color_.ToRaylibColor());
    }

    void Drawing::DrawRectangleGradientV(TVector2 position_, float width_, float height_, TColor color1_,
                                         TColor color2_) {
        DrawRectangleGradientV({position_, width_, height_},
                               color1_,
                               color2_);
    }

    void Drawing::DrawRectangleGradientV(TVector2 position_, TVector2 size_, TColor color1_, TColor color2_) {
        DrawRectangleGradientV({position_, size_},
                               color1_,
                               color2_);
    }

    void Drawing::DrawRectangleGradientV(TRectangle rectangle_, TColor color1_, TColor color2_) {
        ::DrawRectangleGradientV(static_cast<int>(rectangle_.X),
                                 static_cast<int>(rectangle_.Y),
                                 static_cast<int>(rectangle_.Width),
                                 static_cast<int>(rectangle_.Height),
                                 color1_.ToRaylibColor(),
                                 color2_.ToRaylibColor());
    }

    void Drawing::DrawRectangleGradientH(TVector2 position_, float width_, float height_, TColor color1_,
                                         TColor color2_) {
        DrawRectangleGradientH({position_, width_, height_},
                               color1_,
                               color2_);
    }

    void Drawing::DrawRectangleGradientH(TVector2 position_, TVector2 size_, TColor color1_, TColor color2_) {
        DrawRectangleGradientH({position_, position_},
                               color1_,
                               color2_);
    }

    void Drawing::DrawRectangleGradientH(TRectangle rectangle_, TColor color1_, TColor color2_) {
        ::DrawRectangleGradientH(static_cast<int>(rectangle_.X),
                                 static_cast<int>(rectangle_.Y),
                                 static_cast<int>(rectangle_.Width),
                                 static_cast<int>(rectangle_.Height),
                                 color1_.ToRaylibColor(),
                                 color2_.ToRaylibColor());
    }

    void Drawing::DrawRectangleGradientEx(TVector2 position_, float width_, float height_, TColor color1_,
                                          TColor color2_, TColor color3_, TColor color4_) {
        DrawRectangleGradientEx({position_, width_, height_},
                                color1_,
                                color2_,
                                color3_,
                                color4_);
    }

    void Drawing::DrawRectangleGradientEx(TVector2 position_, TVector2 size_, TColor color1_, TColor color2_,
                                          TColor color3_, TColor color4_) {
        DrawRectangleGradientEx({position_, size_},
                                color1_,
                                color2_,
                                color3_,
                                color4_);
    }

    void Drawing::DrawRectangleGradientEx(TRectangle rectangle_, TColor color1_, TColor color2_, TColor color3_,
                                          TColor color4_) {
        ::DrawRectangleGradientEx(rectangle_.ToRaylibRect(),
                                  color1_.ToRaylibColor(),
                                  color2_.ToRaylibColor(),
                                  color3_.ToRaylibColor(),
                                  color4_.ToRaylibColor());
    }

    void Drawing::DrawRectangleLines(TVector2 position_, float width_, float height_, TColor color_,
                                     int lineThickness_) {
        DrawRectangleLines({position_, width_, height_}, color_, lineThickness_);
    }

    void Drawing::DrawRectangleLines(TVector2 position_, TVector2 size_, TColor color_, int lineThickness_) {
        DrawRectangleLines({position_, size_}, color_, lineThickness_);
    }

    void Drawing::DrawRectangleLines(TRectangle rectangle_, TColor color_, int lineThickness_) {
        ::DrawRectangleLinesEx(rectangle_.ToRaylibRect(),
                               lineThickness_,
                               color_.ToRaylibColor());
    }

    void Drawing::DrawRectangleRounded(TVector2 position_, float width_, float height_, float roundness_, int segments_,
                                       TColor color_) {
        DrawRectangleRounded({position_, width_, height_}, roundness_, segments_, color_);
    }

    void Drawing::DrawRectangleRounded(TVector2 position_, TVector2 size_, float roundness_, int segments_,
                                       TColor color_) {
        DrawRectangleRounded({position_, size_}, roundness_, segments_, color_);
    }

    void Drawing::DrawRectangleRounded(TRectangle rectangle_, float roundness_, int segments_, TColor color_) {
        ::DrawRectangleRounded(rectangle_.ToRaylibRect(),
                               roundness_,
                               segments_,
                               color_.ToRaylibColor());
    }

    void Drawing::DrawRectangleRoundedLines(TVector2 position_, float width_, float height_, float roundness_,
                                            int segments_, int lineThickness_, TColor color_) {
        DrawRectangleRoundedLines({position_, width_, height_},
                                  roundness_,
                                  segments_,
                                  lineThickness_,
                                  color_);
    }

    void Drawing::DrawRectangleRoundedLines(TVector2 position_, TVector2 size_, float roundness_, int segments_,
                                            int lineThickness_, TColor color_) {
        DrawRectangleRoundedLines({position_, size_},
                                  roundness_,
                                  segments_,
                                  lineThickness_,
                                  color_);
    }

    void Drawing::DrawRectangleRoundedLines(TRectangle rectangle_, float roundness_, int segments_, int lineThickness_,
                                            TColor color_) {
        ::DrawRectangleRoundedLines(rectangle_.ToRaylibRect(),
                                    roundness_,
                                    segments_,
                                    lineThickness_,
                                    color_.ToRaylibColor());
    }

    void Drawing::DrawText(const Types::TFont &font_, std::string string_, Types::TVector2 position_, float fontSize_,
                           float spacing_, Types::TColor color_) {
        DrawTextEx(font_.ToRaylibFont(),
                   string_.c_str(),
                   position_.ToRaylibVec(),
                   fontSize_,
                   spacing_,
                   color_.ToRaylibColor());
    }

    void Drawing::DrawTextRect(const Types::TFont &font_, std::string string_, Types::TRectangle rectangle_,
                               float fontSize_, float spacing_, Types::TColor color_, bool wordWrap_) {
        DrawTextRec(font_.ToRaylibFont(),
                    string_.c_str(),
                    rectangle_.ToRaylibRect(),
                    fontSize_,
                    spacing_,
                    wordWrap_,
                    color_.ToRaylibColor());
    }

    void Drawing::DrawTextRectEx(const Types::TFont &font_, std::string string_, Types::TRectangle rectangle_,
                                 float fontSize_, float spacing_, Types::TColor color_, const int selectStart_,
                                 const int selectLength_, Types::TColor selectText_, Types::TColor selectBack_,
                                 bool wordWrap_) {
        DrawTextRecEx(font_.ToRaylibFont(),
                      string_.c_str(),
                      rectangle_.ToRaylibRect(),
                      fontSize_,
                      spacing_,
                      wordWrap_,
                      color_.ToRaylibColor(),
                      selectStart_,
                      selectLength_,
                      selectText_.ToRaylibColor(),
                      selectBack_.ToRaylibColor());
    }

    void Drawing::DrawTriangle(TVector2 v1_, TVector2 v2_, TVector2 v3_, TColor color_) {
        ::DrawTriangle(v1_.ToRaylibVec(),
                       v2_.ToRaylibVec(),
                       v3_.ToRaylibVec(),
                       color_.ToRaylibColor());
    }

    void Drawing::DrawTriangleLines(TVector2 v1_, TVector2 v2_, TVector2 v3_, TColor color_) {
        ::DrawTriangleLines(v1_.ToRaylibVec(),
                            v2_.ToRaylibVec(),
                            v3_.ToRaylibVec(),
                            color_.ToRaylibColor());
    }

    void Drawing::DrawTriangleFan(std::vector<Types::TVector2> points_, Types::TColor color_) {
        //Build points array
        auto pts = new Vector2[points_.size()];
        for (auto i = 0; i < points_.size(); i++) {
            pts[i] = points_[i].ToRaylibVec();
        }

        //Draw
        ::DrawTriangleFan(pts,
                          points_.size(),
                          color_.ToRaylibColor());

        //Delete points array
        delete[] pts;
    }

    void Drawing::DrawPoly(TVector2 center_, int sides_, float radius_, float rotation_, TColor color_) {
        ::DrawPoly(center_.ToRaylibVec(),
                   sides_,
                   radius_,
                   rotation_,
                   color_.ToRaylibColor());
    }

    void Drawing::EndCamera2D() {
        EndMode2D();
    }

    void Drawing::EndDrawing() {
        ::EndDrawing();
    }
}
