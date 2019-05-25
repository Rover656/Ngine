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

#include "../3rd-party/raylib/src/rlgl.h" // Used for custom rendering features

// TODO: Use rlgl to add custom vertex data support (i.e. custom shapes)

namespace NerdThings::Ngine::Graphics {
    // Private Fields

    std::vector<TRenderTarget*> Drawing::_TargetStack;

    // Public Methods

    void Drawing::BeginDrawing() {
        ::BeginDrawing();
    }

    void Drawing::Clear(const TColor color_) {
        auto c = color_.ToRaylibColor();
        ClearBackground(color_.ToRaylibColor());
    }

    void Drawing::DrawPixel(const TVector2 position_, const TColor color_) {
        DrawPixelV(position_.ToRaylibVec(),
                   color_.ToRaylibColor());
    }


    void Drawing::DrawLine(const TVector2 startPos_, const TVector2 endPos_, const TColor color_,
                           const float thickness_) {
        DrawLineEx(startPos_.ToRaylibVec(),
                   endPos_.ToRaylibVec(),
                   thickness_,
                   color_.ToRaylibColor());
    }

    void Drawing::DrawLineStrip(std::vector<TVector2> points_, const TColor color_) {
        //Build points array
        auto pts = new Vector2[points_.size()];
        for (unsigned int i = 0; i < points_.size(); i++) {
            pts[i] = points_[i].ToRaylibVec();
        }

        //Draw
        ::DrawLineStrip(pts,
                        points_.size(),
                        color_.ToRaylibColor());

        //Delete points array
        delete[] pts;
    }

    void Drawing::DrawLineBezier(const TVector2 startPos_, const TVector2 endPos_, const TColor color_,
                                 const float thickness_) {
        ::DrawLineBezier(startPos_.ToRaylibVec(),
                         endPos_.ToRaylibVec(),
                         thickness_,
                         color_.ToRaylibColor());
    }

    void Drawing::DrawCircle(const TVector2 center_, const float radius_, const TColor color_) {
        DrawCircleV(center_.ToRaylibVec(),
                    radius_,
                    color_.ToRaylibColor());
    }

    void Drawing::DrawCircleGradient(const TVector2 center_, const float radius_, const TColor color1_,
                                     const TColor color2_) {
        ::DrawCircleGradient(static_cast<int>(center_.X),
                             static_cast<int>(center_.Y),
                             radius_,
                             color1_.ToRaylibColor(),
                             color2_.ToRaylibColor());
    }

    void Drawing::DrawCircleLines(const TVector2 center_, const float radius_, const TColor color_) {
        ::DrawCircleLines(static_cast<int>(center_.X),
                          static_cast<int>(center_.Y),
                          radius_,
                          color_.ToRaylibColor());
    }

    void Drawing::DrawCircleSector(const TVector2 center_, const float radius_, const int startAngle_,
                                   const int endAngle_, const int segments_,
                                   const TColor color_) {
        ::DrawCircleSector(center_.ToRaylibVec(),
                           radius_,
                           startAngle_,
                           endAngle_,
                           segments_,
                           color_.ToRaylibColor());
    }

    void Drawing::DrawCircleSectorLines(const TVector2 center_, const float radius_, const int startAngle_,
                                        const int endAngle_, const int segments_,
                                        const TColor color_) {
        ::DrawCircleSectorLines(center_.ToRaylibVec(),
                                radius_,
                                startAngle_,
                                endAngle_,
                                segments_,
                                color_.ToRaylibColor());
    }

    void Drawing::DrawFPS(const TVector2 position_) {
        ::DrawFPS(static_cast<int>(position_.X),
                  static_cast<int>(position_.Y));
    }

    void Drawing::DrawRing(const TVector2 center_, const float innerRadius_, const float outerRadius_,
                           const int startAngle_, const int endAngle_,
                           const int segments_, const TColor color_) {
        ::DrawRing(center_.ToRaylibVec(),
                   innerRadius_,
                   outerRadius_,
                   startAngle_,
                   endAngle_,
                   segments_,
                   color_.ToRaylibColor());
    }

    void Drawing::DrawRingLines(const TVector2 center_, const float innerRadius_, const float outerRadius_,
                                const int startAngle_,
                                const int endAngle_, const int segments_, const TColor color_) {
        ::DrawRingLines(center_.ToRaylibVec(),
                        innerRadius_,
                        outerRadius_,
                        startAngle_,
                        endAngle_,
                        segments_,
                        color_.ToRaylibColor());
    }

    void Drawing::DrawRectangle(const TVector2 position_, const float width_, const float height_, const TColor color_,
                                const float rotation_,
                                const TVector2 origin_) {
        DrawRectangle({
                          position_,
                          width_,
                          height_
                      },
                      color_,
                      rotation_,
                      origin_);
    }

    void Drawing::DrawRectangle(const TVector2 position_, const TVector2 size_, const TColor color_,
                                const float rotation_, const TVector2 origin_) {
        DrawRectangle({
                          position_,
                          size_
                      },
                      color_,
                      rotation_,
                      origin_);
    }

    void Drawing::DrawRectangle(const TRectangle rectangle_, const TColor color_, const float rotation_,
                                const TVector2 origin_) {
        DrawRectanglePro(rectangle_.ToRaylibRect(),
                         origin_.ToRaylibVec(),
                         rotation_,
                         color_.ToRaylibColor());
    }

    void Drawing::DrawRectangleGradientV(const TVector2 position_, const float width_, const float height_,
                                         const TColor color1_,
                                         const TColor color2_) {
        DrawRectangleGradientV({
                                   position_,
                                   width_,
                                   height_
                               },
                               color1_,
                               color2_);
    }

    void Drawing::DrawRectangleGradientV(const TVector2 position_, const TVector2 size_, const TColor color1_,
                                         const TColor color2_) {
        DrawRectangleGradientV({
                                   position_,
                                   size_
                               },
                               color1_,
                               color2_);
    }

    void Drawing::DrawRectangleGradientV(const TRectangle rectangle_, const TColor color1_, const TColor color2_) {
        ::DrawRectangleGradientV(static_cast<int>(rectangle_.X),
                                 static_cast<int>(rectangle_.Y),
                                 static_cast<int>(rectangle_.Width),
                                 static_cast<int>(rectangle_.Height),
                                 color1_.ToRaylibColor(),
                                 color2_.ToRaylibColor());
    }

    void Drawing::DrawRectangleGradientH(const TVector2 position_, const float width_, const float height_,
                                         const TColor color1_,
                                         const TColor color2_) {
        DrawRectangleGradientH({
                                   position_,
                                   width_,
                                   height_
                               },
                               color1_,
                               color2_);
    }

    void Drawing::DrawRectangleGradientH(const TVector2 position_, const TVector2 size_, const TColor color1_,
                                         const TColor color2_) {
        DrawRectangleGradientH({
                                   position_,
                                   size_
                               },
                               color1_,
                               color2_);
    }

    void Drawing::DrawRectangleGradientH(const TRectangle rectangle_, const TColor color1_, const TColor color2_) {
        ::DrawRectangleGradientH(static_cast<int>(rectangle_.X),
                                 static_cast<int>(rectangle_.Y),
                                 static_cast<int>(rectangle_.Width),
                                 static_cast<int>(rectangle_.Height),
                                 color1_.ToRaylibColor(),
                                 color2_.ToRaylibColor());
    }

    void Drawing::DrawRectangleGradientEx(const TVector2 position_, const float width_, const float height_,
                                          const TColor color1_,
                                          const TColor color2_, const TColor color3_, const TColor color4_) {
        DrawRectangleGradientEx({
                                    position_,
                                    width_,
                                    height_
                                },
                                color1_,
                                color2_,
                                color3_,
                                color4_);
    }

    void Drawing::DrawRectangleGradientEx(const TVector2 position_, const TVector2 size_, const TColor color1_,
                                          const TColor color2_,
                                          const TColor color3_, const TColor color4_) {
        DrawRectangleGradientEx({
                                    position_,
                                    size_
                                },
                                color1_,
                                color2_,
                                color3_,
                                color4_);
    }

    void Drawing::DrawRectangleGradientEx(const TRectangle rectangle_, const TColor color1_, const TColor color2_,
                                          const TColor color3_,
                                          const TColor color4_) {
        ::DrawRectangleGradientEx(rectangle_.ToRaylibRect(),
                                  color1_.ToRaylibColor(),
                                  color2_.ToRaylibColor(),
                                  color3_.ToRaylibColor(),
                                  color4_.ToRaylibColor());
    }

    void Drawing::DrawRectangleLines(const TVector2 position_, const float width_, const float height_,
                                     const TColor color_,
                                     const int lineThickness_) {
        DrawRectangleLines({
                               position_,
                               width_,
                               height_
                           },
                           color_,
                           lineThickness_);
    }

    void Drawing::DrawRectangleLines(const TVector2 position_, const TVector2 size_, const TColor color_,
                                     const int lineThickness_) {
        DrawRectangleLines({
                               position_,
                               size_
                           },
                           color_,
                           lineThickness_);
    }

    void Drawing::DrawRectangleLines(const TRectangle rectangle_, const TColor color_, const int lineThickness_) {
        ::DrawRectangleLinesEx(rectangle_.ToRaylibRect(),
                               lineThickness_,
                               color_.ToRaylibColor());
    }

    void Drawing::DrawRectangleRounded(const TVector2 position_, const float width_, const float height_,
                                       const float roundness_, const int segments_,
                                       const TColor color_) {
        DrawRectangleRounded({
                                 position_,
                                 width_,
                                 height_
                             },
                             roundness_,
                             segments_,
                             color_);
    }

    void Drawing::DrawRectangleRounded(const TVector2 position_, const TVector2 size_, const float roundness_,
                                       const int segments_,
                                       const TColor color_) {
        DrawRectangleRounded({
                                 position_,
                                 size_
                             },
                             roundness_,
                             segments_,
                             color_);
    }

    void Drawing::DrawRectangleRounded(const TRectangle rectangle_, const float roundness_, const int segments_,
                                       const TColor color_) {
        ::DrawRectangleRounded(rectangle_.ToRaylibRect(),
                               roundness_,
                               segments_,
                               color_.ToRaylibColor());
    }

    void Drawing::DrawRectangleRoundedLines(const TVector2 position_, const float width_, const float height_,
                                            const float roundness_,
                                            const int segments_, const int lineThickness_, const TColor color_) {
        DrawRectangleRoundedLines({
                                      position_,
                                      width_,
                                      height_
                                  },
                                  roundness_,
                                  segments_,
                                  lineThickness_,
                                  color_);
    }

    void Drawing::DrawRectangleRoundedLines(const TVector2 position_, const TVector2 size_, const float roundness_,
                                            const int segments_,
                                            const int lineThickness_, const TColor color_) {
        DrawRectangleRoundedLines({
                                      position_,
                                      size_
                                  },
                                  roundness_,
                                  segments_,
                                  lineThickness_,
                                  color_);
    }

    void Drawing::DrawRectangleRoundedLines(const TRectangle rectangle_, const float roundness_, const int segments_,
                                            const int lineThickness_,
                                            const TColor color_) {
        ::DrawRectangleRoundedLines(rectangle_.ToRaylibRect(),
                                    roundness_,
                                    segments_,
                                    lineThickness_,
                                    color_.ToRaylibColor());
    }

    void Drawing::DrawText(const TFont &font_, const std::string &string_, const TVector2 position_,
                           const float fontSize_,
                           const float spacing_, const TColor color_) {
        DrawTextEx(font_.ToRaylibFont(),
                   string_.c_str(),
                   position_.ToRaylibVec(),
                   fontSize_,
                   spacing_,
                   color_.ToRaylibColor());
    }

    void Drawing::DrawTextRect(const TFont &font_, const std::string &string_, const TRectangle rectangle_,
                               const float fontSize_, const float spacing_, const TColor color_, const bool wordWrap_) {
        DrawTextRec(font_.ToRaylibFont(),
                    string_.c_str(),
                    rectangle_.ToRaylibRect(),
                    fontSize_,
                    spacing_,
                    wordWrap_,
                    color_.ToRaylibColor());
    }

    void Drawing::DrawTextRectEx(const TFont &font_, const std::string &string_, const TRectangle rectangle_,
                                 const float fontSize_, const float spacing_, const TColor color_,
                                 const int selectStart_,
                                 const int selectLength_, const TColor selectText_, const TColor selectBack_,
                                 const bool wordWrap_) {
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

    void Drawing::DrawTexture(const TTexture2D &texture_, const TVector2 position_, const TColor color_,
                              const float scale_,
                              const TVector2 origin_, const float rotation_) {
        DrawTexture(texture_,
                    {
                        position_.X,
                        position_.Y,
                        static_cast<float>(texture_.Width) * scale_,
                        static_cast<float>(texture_.Height) * scale_
                    },
                    {
                        0,
                        0,
                        static_cast<float>(texture_.Width),
                        static_cast<float>(texture_.Height)
                    },
                    color_,
                    origin_,
                    rotation_);
    }

    void Drawing::DrawTexture(const TTexture2D &texture_, const TVector2 position_, const float width_,
                              const float height_,
                              const TColor color_, const TVector2 origin_, const float rotation_) {
        DrawTexture(texture_,
                    {
                        position_.X,
                        position_.Y,
                        width_,
                        height_
                    },
                    {
                        0,
                        0,
                        static_cast<float>(texture_.Width),
                        static_cast<float>(texture_.Height)
                    },
                    color_,
                    origin_,
                    rotation_);
    }

    void Drawing::DrawTexture(const TTexture2D &texture_, const TRectangle sourceRectangle_,
                              const TVector2 position_, const TColor color_, const TVector2 origin_,
                              const float rotation_) {
        DrawTexture(texture_,
                    {
                        position_.X,
                        position_.Y,
                        sourceRectangle_.Width,
                        sourceRectangle_.Height
                    },
                    sourceRectangle_,
                    color_,
                    origin_,
                    rotation_);
    }

    void Drawing::DrawTexture(const TTexture2D &texture_, const TRectangle sourceRectangle_,
                              const TVector2 position_, const float width_, const float height_, const TColor color_,
                              const TVector2 origin_, const float rotation_) {
        DrawTexture(texture_,
                    {
                        position_.X,
                        position_.Y,
                        width_,
                        height_
                    },
                    sourceRectangle_,
                    color_,
                    origin_,
                    rotation_);
    }

    void Drawing::DrawTexture(const TTexture2D &texture_, const TRectangle destRectangle_,
                              const TRectangle sourceRectangle_, const TColor color_, const TVector2 origin_,
                              const float rotation_) {
        DrawTexturePro(texture_.ToRaylibTex(),
                       sourceRectangle_.ToRaylibRect(),
                       destRectangle_.ToRaylibRect(),
                       origin_.ToRaylibVec(),
                       rotation_,
                       color_.ToRaylibColor());
    }

    void Drawing::DrawTriangle(const TVector2 v1_, const TVector2 v2_, const TVector2 v3_, const TColor color_) {
        ::DrawTriangle(v1_.ToRaylibVec(),
                       v2_.ToRaylibVec(),
                       v3_.ToRaylibVec(),
                       color_.ToRaylibColor());
    }

    void Drawing::DrawTriangleLines(const TVector2 v1_, const TVector2 v2_, const TVector2 v3_, const TColor color_) {
        ::DrawTriangleLines(v1_.ToRaylibVec(),
                            v2_.ToRaylibVec(),
                            v3_.ToRaylibVec(),
                            color_.ToRaylibColor());
    }

    void Drawing::DrawTriangleFan(std::vector<TVector2> points_, const TColor color_) {
        //Build points array
        auto pts = new Vector2[points_.size()];
        for (unsigned int i = 0; i < points_.size(); i++) {
            pts[i] = points_[i].ToRaylibVec();
        }

        //Draw
        ::DrawTriangleFan(pts,
                          points_.size(),
                          color_.ToRaylibColor());

        //Delete points array
        delete[] pts;
    }

    void Drawing::DrawPoly(const TVector2 center_, const int sides_, const float radius_, const float rotation_,
                           const TColor color_) {
        ::DrawPoly(center_.ToRaylibVec(),
                   sides_,
                   radius_,
                   rotation_,
                   color_.ToRaylibColor());
    }

    void Drawing::EndDrawing() {
        ::EndDrawing();
    }

    TRenderTarget *Drawing::PopTarget(bool &popped_) {
        if (!_TargetStack.empty()) {
            // Get target
            auto pop = _TargetStack.back();

            // Remove target
            _TargetStack.pop_back();

            // Start using another if it is available
            if (!_TargetStack.empty()) {
                BeginTextureMode(_TargetStack.back()->ToRaylibTarget());
            }

            EndTextureMode();

            popped_ = true;
            return pop;
        }

        popped_ = false;
        return nullptr;
    }

    void Drawing::PushTarget(TRenderTarget *target_) {
        // Add to target stack
        _TargetStack.emplace_back(target_);

        // Use the target
        BeginTextureMode(target_->ToRaylibTarget());
    }

    void Drawing::ReplaceTarget(TRenderTarget *old_, TRenderTarget *new_) {
        const auto oldPos = std::find(_TargetStack.begin(), _TargetStack.end(), old_) - _TargetStack.begin();

        // If this is the currently active target, replace it too
        if (oldPos == _TargetStack.size() - 1) {
            EndTextureMode();
            BeginTextureMode(new_->ToRaylibTarget());
        }

        // Send to stack
        _TargetStack[oldPos] = new_;
    }

}
