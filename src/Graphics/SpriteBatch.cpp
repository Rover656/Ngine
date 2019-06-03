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

#include "SpriteBatch.h"

namespace NerdThings::Ngine::Graphics {
    // Public Constructor(s)

    SpriteBatch::SpriteBatch() = default;

    // Public Methods

    void SpriteBatch::Begin() {
        if (_Drawing)
            throw std::runtime_error("Cannot start new batch before ending the last one.");
        _Drawing = true;
    }


    void SpriteBatch::DrawCircle(Physics::TCircle circle_, TColor color_, int depth_) {
        DrawCircle(circle_.Center, circle_.Radius, color_, depth_);
    }

    void SpriteBatch::DrawCircle(Math::TVector2 center_, float radius_, TColor color_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_CIRCLE;
        bi.Floats.push_back(center_.X);
        bi.Floats.push_back(center_.Y);
        bi.Floats.push_back(radius_);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawCircleGradient(Physics::TCircle circle_, TColor color1_, TColor color2_, int depth_) {
        DrawCircleGradient(circle_.Center, circle_.Radius, color1_, color2_, depth_);
    }

    void SpriteBatch::DrawCircleGradient(Math::TVector2 center_, float radius_, TColor color1_, TColor color2_,
                                         int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_CIRCLE_GRADIENT;
        bi.Floats.push_back(center_.X);
        bi.Floats.push_back(center_.Y);
        bi.Floats.push_back(radius_);
        bi.Colors.push_back(color1_);
        bi.Colors.push_back(color2_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawCircleLines(Physics::TCircle circle_, TColor color_, int depth_) {
        DrawCircleLines(circle_.Center, circle_.Radius, color_, depth_);
    }

    void SpriteBatch::DrawCircleLines(Math::TVector2 center_, float radius_, TColor color_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_CIRCLE_LINES;
        bi.Floats.push_back(center_.X);
        bi.Floats.push_back(center_.Y);
        bi.Floats.push_back(radius_);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawFPS(float x_, float y_, int depth_) {
        DrawFPS({x_, y_});
    }

    void SpriteBatch::DrawFPS(Math::TVector2 position_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_FPS;
        bi.Floats.push_back(position_.X);
        bi.Floats.push_back(position_.Y);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawLine(Math::TVector2 pos1_, Math::TVector2 pos2_, TColor col_, float thickness_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_LINE;
        bi.Floats.push_back(pos1_.X);
        bi.Floats.push_back(pos1_.Y);
        bi.Floats.push_back(pos2_.X);
        bi.Floats.push_back(pos2_.Y);
        bi.Colors.push_back(col_);
        bi.Floats.push_back(thickness_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawLineBezier(Math::TVector2 pos1_, Math::TVector2 pos2_, TColor color_, float thickness_,
                                     int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_LINE_BEZIER;
        bi.Floats.push_back(pos1_.X);
        bi.Floats.push_back(pos1_.Y);
        bi.Floats.push_back(pos2_.X);
        bi.Floats.push_back(pos2_.Y);
        bi.Colors.push_back(color_);
        bi.Floats.push_back(thickness_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawLineStrip(std::vector<Math::TVector2> points_, TColor color_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_LINE_STRIP;
        for (auto p : points_) {
            bi.Floats.push_back(p.X);
            bi.Floats.push_back(p.Y);
        }
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawPixel(float x_, float y_, TColor color_, int depth_) {
        DrawPixel({x_, y_}, color_, depth_);
    }

    void SpriteBatch::DrawPixel(Math::TVector2 position_, TColor color_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_PIXEL;
        bi.Floats.push_back(position_.X);
        bi.Floats.push_back(position_.Y);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawPoly(Math::TVector2 center_, int sides_, float radius_, float rotation_, TColor color_,
                               int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_POLY;
        bi.Floats.push_back(center_.X);
        bi.Floats.push_back(center_.Y);
        bi.Ints.push_back(sides_);
        bi.Floats.push_back(radius_);
        bi.Floats.push_back(rotation_);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawRectangle(Math::TVector2 position_, Math::TVector2 size_, TColor color_, float rotation_,
                                    Math::TVector2 origin_, int depth_) {
        DrawRectangle({position_, size_}, color_, rotation_, origin_, depth_);
    }

    void SpriteBatch::DrawRectangle(Math::TRectangle rectangle_, TColor color_, float rotation_, Math::TVector2 origin_,
                                    int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_RECTANGLE;
        bi.Floats.push_back(rectangle_.X);
        bi.Floats.push_back(rectangle_.Y);
        bi.Floats.push_back(rectangle_.Width);
        bi.Floats.push_back(rectangle_.Height);
        bi.Colors.push_back(color_);
        bi.Floats.push_back(rotation_);
        bi.Floats.push_back(origin_.X);
        bi.Floats.push_back(origin_.Y);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawRectangleGradientV(Math::TRectangle rectangle_, TColor color1_, TColor color2_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_RECTANGLE_GRADIENT_V;
        bi.Floats.push_back(rectangle_.X);
        bi.Floats.push_back(rectangle_.Y);
        bi.Floats.push_back(rectangle_.Width);
        bi.Floats.push_back(rectangle_.Height);
        bi.Colors.push_back(color1_);
        bi.Colors.push_back(color2_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawRectangleGradientH(Math::TRectangle rectangle_, TColor color1_, TColor color2_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_RECTANGLE_GRADIENT_H;
        bi.Floats.push_back(rectangle_.X);
        bi.Floats.push_back(rectangle_.Y);
        bi.Floats.push_back(rectangle_.Width);
        bi.Floats.push_back(rectangle_.Height);
        bi.Colors.push_back(color1_);
        bi.Colors.push_back(color2_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawRectangleGradientEx(Math::TRectangle rectangle_, TColor color1_, TColor color2_,
                                              TColor color3_, TColor color4_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_RECTANGLE_GRADIENT_EX;
        bi.Floats.push_back(rectangle_.X);
        bi.Floats.push_back(rectangle_.Y);
        bi.Floats.push_back(rectangle_.Width);
        bi.Floats.push_back(rectangle_.Height);
        bi.Colors.push_back(color1_);
        bi.Colors.push_back(color2_);
        bi.Colors.push_back(color3_);
        bi.Colors.push_back(color4_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawRectangleLines(Math::TVector2 position_, Math::TVector2 size_, TColor color_,
                                         int lineThickness_, int depth_) {
        DrawRectangleLines({position_, size_}, color_, lineThickness_, depth_);
    }

    void SpriteBatch::DrawRectangleLines(Math::TRectangle rectangle_, TColor color_, int lineThickness_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_RECTANGLE_LINES;
        bi.Floats.push_back(rectangle_.X);
        bi.Floats.push_back(rectangle_.Y);
        bi.Floats.push_back(rectangle_.Width);
        bi.Floats.push_back(rectangle_.Height);
        bi.Colors.push_back(color_);
        bi.Ints.push_back(lineThickness_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawRectangleRounded(Math::TVector2 position_, Math::TVector2 size_, float roundness_,
                                           int segments_, TColor color_, int depth_) {
        DrawRectangleRounded({position_, size_}, roundness_, segments_, color_, depth_);
    }

    void SpriteBatch::DrawRectangleRounded(Math::TRectangle rectangle_, float roundness_, int segments_, TColor color_,
                                           int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_RECTANGLE_ROUNDED;
        bi.Floats.push_back(rectangle_.X);
        bi.Floats.push_back(rectangle_.Y);
        bi.Floats.push_back(rectangle_.Width);
        bi.Floats.push_back(rectangle_.Height);
        bi.Floats.push_back(roundness_);
        bi.Ints.push_back(segments_);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawRectangleRoundedLines(Math::TVector2 position_, Math::TVector2 size_, float roundness_,
                                                int segments_, int lineThickness_, TColor color_, int depth_) {
        DrawRectangleRoundedLines({position_, size_}, roundness_, segments_, lineThickness_, color_, depth_);
    }

    void SpriteBatch::DrawRectangleRoundedLines(Math::TRectangle rectangle_, float roundness_, int segments_,
                                                int lineThickness_, TColor color_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_RECTANGLE_ROUNDED_LINES;
        bi.Floats.push_back(rectangle_.X);
        bi.Floats.push_back(rectangle_.Y);
        bi.Floats.push_back(rectangle_.Width);
        bi.Floats.push_back(rectangle_.Height);
        bi.Floats.push_back(roundness_);
        bi.Ints.push_back(segments_);
        bi.Ints.push_back(lineThickness_);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawRing(Math::TVector2 center_, float inRadius_, float outRadius_, int startAngle_,
                               int endAngle_, int segments_, TColor color_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_RING;
        bi.Floats.push_back(center_.X);
        bi.Floats.push_back(center_.Y);
        bi.Floats.push_back(inRadius_);
        bi.Floats.push_back(outRadius_);
        bi.Ints.push_back(startAngle_);
        bi.Ints.push_back(endAngle_);
        bi.Ints.push_back(segments_);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawRingLines(Math::TVector2 center_, float inRadius_, float outRadius_, int startAngle_,
                                    int endAngle_, int segments_, TColor color_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_RING_LINES;
        bi.Floats.push_back(center_.X);
        bi.Floats.push_back(center_.Y);
        bi.Floats.push_back(inRadius_);
        bi.Floats.push_back(outRadius_);
        bi.Ints.push_back(startAngle_);
        bi.Ints.push_back(endAngle_);
        bi.Ints.push_back(segments_);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawText(TFont *font_, const std::string &string_, Math::TVector2 position_, float fontSize_,
                               float spacing_, TColor color_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_TEXT;
        bi.Font = font_;
        bi.String = string_;
        bi.Floats.push_back(position_.X);
        bi.Floats.push_back(position_.Y);
        bi.Floats.push_back(fontSize_);
        bi.Floats.push_back(spacing_);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawTextRect(TFont *font_, const std::string &string_, Math::TRectangle rectangle_,
                                   float fontSize_, float spacing_, TColor color_, bool wordWrap_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_TEXT_RECT;
        bi.Font = font_;
        bi.String = string_;
        bi.Floats.push_back(rectangle_.X);
        bi.Floats.push_back(rectangle_.Y);
        bi.Floats.push_back(rectangle_.Width);
        bi.Floats.push_back(rectangle_.Height);
        bi.Floats.push_back(fontSize_);
        bi.Floats.push_back(spacing_);
        bi.Colors.push_back(color_);
        bi.Bools.push_back(wordWrap_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawTextRectEx(TFont *font_, const std::string &string_, Math::TRectangle rectangle_,
                                     float fontSize_, float spacing_, TColor color_, int selectStart_,
                                     int selectLength_, TColor selectText_,
                                     TColor selectBack_, bool wordWrap_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_TEXT_RECT_EX;
        bi.Font = font_;
        bi.String = string_;
        bi.Floats.push_back(rectangle_.X);
        bi.Floats.push_back(rectangle_.Y);
        bi.Floats.push_back(rectangle_.Width);
        bi.Floats.push_back(rectangle_.Height);
        bi.Floats.push_back(fontSize_);
        bi.Floats.push_back(spacing_);
        bi.Colors.push_back(color_);
        bi.Ints.push_back(selectStart_);
        bi.Ints.push_back(selectLength_);
        bi.Colors.push_back(selectText_);
        bi.Colors.push_back(selectBack_);
        bi.Bools.push_back(wordWrap_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawTexture(TTexture2D *texture_, Math::TVector2 position_, TColor color_, float scale_,
                                  float rotation_, Math::TVector2 origin_, int depth_) {
        DrawTexture(texture_, {position_, texture_->Width * scale_, texture_->Height * scale_},
                    {0, 0, static_cast<float>(texture_->Width), static_cast<float>(texture_->Height)},
                    color_, rotation_, origin_, depth_);
    }

    void SpriteBatch::DrawTexture(TTexture2D *texture_, Math::TRectangle destinationRectangle_,
                                  Math::TRectangle sourceRectangle_, TColor color_, float rotation_,
                                  Math::TVector2 origin_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_TEXTURE;
        bi.Texture = texture_;
        bi.Floats.push_back(destinationRectangle_.X);
        bi.Floats.push_back(destinationRectangle_.Y);
        bi.Floats.push_back(destinationRectangle_.Width);
        bi.Floats.push_back(destinationRectangle_.Height);
        bi.Floats.push_back(sourceRectangle_.X);
        bi.Floats.push_back(sourceRectangle_.Y);
        bi.Floats.push_back(sourceRectangle_.Width);
        bi.Floats.push_back(sourceRectangle_.Height);
        bi.Colors.push_back(color_);
        bi.Floats.push_back(rotation_);
        bi.Floats.push_back(origin_.X);
        bi.Floats.push_back(origin_.Y);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawTriangle(Math::TVector2 v1_, Math::TVector2 v2_, Math::TVector2 v3_, TColor color_,
                                   int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_TRIANGLE;
        bi.Floats.push_back(v1_.X);
        bi.Floats.push_back(v1_.Y);
        bi.Floats.push_back(v2_.X);
        bi.Floats.push_back(v2_.Y);
        bi.Floats.push_back(v3_.X);
        bi.Floats.push_back(v3_.Y);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawTriangleLines(Math::TVector2 v1_, Math::TVector2 v2_, Math::TVector2 v3_, TColor color_,
                                        int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_TRIANGLE_LINES;
        bi.Floats.push_back(v1_.X);
        bi.Floats.push_back(v1_.Y);
        bi.Floats.push_back(v2_.X);
        bi.Floats.push_back(v2_.Y);
        bi.Floats.push_back(v3_.X);
        bi.Floats.push_back(v3_.Y);
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::DrawTriangleFan(std::vector<Math::TVector2> points_, TColor color_, int depth_) {
        auto bi = SpriteBatchItem();
        bi.Type = ITEM_TRIANGLE_FAN;
        for (auto p : points_) {
            bi.Floats.push_back(p.X);
            bi.Floats.push_back(p.Y);
        }
        bi.Colors.push_back(color_);
        if (_Batch.find(depth_) != _Batch.end())
            _Batch.insert({depth_, {}});
        _Batch[depth_].push_back(bi);
    }

    void SpriteBatch::End() {
        if (!_Drawing)
            throw std::runtime_error("Cannot end a batch before one has begun.");

        // Draw batch (Maps are sorted automagically)
        for (auto mapPairs : _Batch) {
            auto items = mapPairs.second;

            for (auto bi : items) {
                switch (bi.Type) {
                    case ITEM_CIRCLE:
                        ::DrawCircle(static_cast<int>(bi.Floats[0]),
                                     static_cast<int>(bi.Floats[1]),
                                     bi.Floats[2],
                                     bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_CIRCLE_GRADIENT:
                        ::DrawCircleGradient(static_cast<int>(bi.Floats[0]),
                                             static_cast<int>(bi.Floats[1]),
                                             bi.Floats[2],
                                             bi.Colors[0].ToRaylibColor(),
                                             bi.Colors[1].ToRaylibColor());
                        break;
                    case ITEM_CIRCLE_LINES:
                        ::DrawCircleLines(static_cast<int>(bi.Floats[0]),
                                          static_cast<int>(bi.Floats[1]),
                                          bi.Floats[2],
                                          bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_FPS:
                        ::DrawFPS(static_cast<int>(bi.Floats[0]), static_cast<int>(bi.Floats[1]));
                        break;
                    case ITEM_LINE:
                        ::DrawLineEx({bi.Floats[0], bi.Floats[1]},
                                     {bi.Floats[2], bi.Floats[3]},
                                     bi.Floats[4],
                                     bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_LINE_BEZIER:
                        ::DrawLineBezier({bi.Floats[0], bi.Floats[1]},
                                         {bi.Floats[2], bi.Floats[3]},
                                         bi.Floats[4],
                                         bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_LINE_STRIP: {
                        //Build points array
                        auto pts = new Vector2[bi.Floats.size() / 2];
                        for (unsigned int i = 0; i < bi.Floats.size(); i += 2) {
                            pts[i] = Math::TVector2(bi.Floats[i], bi.Floats[i + 1]).ToRaylibVec();
                        }

                        //Draw
                        ::DrawLineStrip(pts,
                                        bi.Floats.size() / 2,
                                        bi.Colors[0].ToRaylibColor());

                        //Delete points array
                        delete[] pts;
                        break;
                    }
                    case ITEM_PIXEL:
                        ::DrawPixelV({bi.Floats[0], bi.Floats[1]}, bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_POLY:
                        ::DrawPoly({bi.Floats[0], bi.Floats[1]},
                                   bi.Ints[0],
                                   bi.Floats[2],
                                   bi.Floats[3],
                                   bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_RECTANGLE:
                        ::DrawRectangleV({bi.Floats[0], bi.Floats[1]},
                                         {bi.Floats[2], bi.Floats[3]},
                                         bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_RECTANGLE_GRADIENT_V:
                        ::DrawRectangleGradientEx({bi.Floats[0], bi.Floats[1], bi.Floats[2], bi.Floats[3]},
                                                  bi.Colors[0].ToRaylibColor(),
                                                  bi.Colors[1].ToRaylibColor(),
                                                  bi.Colors[1].ToRaylibColor(),
                                                  bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_RECTANGLE_GRADIENT_H:
                        ::DrawRectangleGradientEx({bi.Floats[0], bi.Floats[1], bi.Floats[2], bi.Floats[3]},
                                                  bi.Colors[0].ToRaylibColor(),
                                                  bi.Colors[0].ToRaylibColor(),
                                                  bi.Colors[1].ToRaylibColor(),
                                                  bi.Colors[1].ToRaylibColor());
                        break;
                    case ITEM_RECTANGLE_GRADIENT_EX:
                        ::DrawRectangleGradientEx({bi.Floats[0], bi.Floats[1], bi.Floats[2], bi.Floats[3]},
                                                  bi.Colors[0].ToRaylibColor(),
                                                  bi.Colors[1].ToRaylibColor(),
                                                  bi.Colors[2].ToRaylibColor(),
                                                  bi.Colors[3].ToRaylibColor());
                        break;
                    case ITEM_RECTANGLE_LINES:
                        ::DrawRectangleLinesEx({bi.Floats[0], bi.Floats[1], bi.Floats[2], bi.Floats[3]},
                                               bi.Ints[0],
                                               bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_RECTANGLE_ROUNDED:
                        ::DrawRectangleRounded({bi.Floats[0], bi.Floats[1], bi.Floats[2], bi.Floats[3]},
                                               bi.Floats[4],
                                               bi.Ints[0],
                                               bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_RECTANGLE_ROUNDED_LINES:
                        ::DrawRectangleRoundedLines({bi.Floats[0], bi.Floats[1], bi.Floats[2], bi.Floats[3]},
                                                    bi.Floats[4],
                                                    bi.Ints[0],
                                                    bi.Ints[1],
                                                    bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_RING:
                        ::DrawRing({bi.Floats[0], bi.Floats[1]},
                                   bi.Floats[2],
                                   bi.Floats[3],
                                   bi.Ints[0],
                                   bi.Ints[1],
                                   bi.Ints[2],
                                   bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_RING_LINES:
                        ::DrawRingLines({bi.Floats[0], bi.Floats[1]},
                                        bi.Floats[2],
                                        bi.Floats[3],
                                        bi.Ints[0],
                                        bi.Ints[1],
                                        bi.Ints[2],
                                        bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_TEXT:
                        ::DrawTextEx(bi.Font->ToRaylibFont(),
                                     bi.String.c_str(),
                                     {bi.Floats[0], bi.Floats[1]},
                                     bi.Floats[2],
                                     bi.Floats[3],
                                     bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_TEXT_RECT:
                        ::DrawTextRecEx(bi.Font->ToRaylibFont(),
                                        bi.String.c_str(),
                                        {bi.Floats[0], bi.Floats[1], bi.Floats[2], bi.Floats[3]},
                                        bi.Floats[4],
                                        bi.Floats[5],
                                        bi.Bools[0],
                                        bi.Colors[0].ToRaylibColor(),
                                        bi.Ints[0],
                                        bi.Ints[1],
                                        bi.Colors[1].ToRaylibColor(),
                                        bi.Colors[2].ToRaylibColor());
                        break;
                    case ITEM_TEXTURE:
                        ::DrawTexturePro(bi.Texture->ToRaylibTex(),
                                         {bi.Floats[0], bi.Floats[1], bi.Floats[2], bi.Floats[3]},
                                         {bi.Floats[4], bi.Floats[5], bi.Floats[6], bi.Floats[7]},
                                         {bi.Floats[8], bi.Floats[9]},
                                         bi.Floats[10],
                                         bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_TRIANGLE:
                        ::DrawTriangle({bi.Floats[0], bi.Floats[1]},
                                       {bi.Floats[2], bi.Floats[3]},
                                       {bi.Floats[4], bi.Floats[5]},
                                       bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_TRIANGLE_LINES:
                        ::DrawTriangleLines({bi.Floats[0], bi.Floats[1]},
                                            {bi.Floats[2], bi.Floats[3]},
                                            {bi.Floats[4], bi.Floats[5]},
                                            bi.Colors[0].ToRaylibColor());
                        break;
                    case ITEM_TRIANGLE_FAN:
                        //Build points array
                        auto pts = new Vector2[bi.Floats.size() / 2];
                        for (unsigned int i = 0; i < bi.Floats.size(); i += 2) {
                            pts[i] = Math::TVector2(bi.Floats[i], bi.Floats[i + 1]).ToRaylibVec();
                        }

                        //Draw
                        ::DrawTriangleFan(pts,
                                          bi.Floats.size() / 2,
                                          bi.Colors[0].ToRaylibColor());

                        //Delete points array
                        delete[] pts;
                        break;
                }
            }
        }

        // Clear and reset
        _Batch.clear();
        _Drawing = false;
    }
}
