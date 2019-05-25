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

#ifndef DRAWING_H
#define DRAWING_H

#include "../ngine.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * Connect to the raylib renderer directly
     */
    class NEAPI Drawing {
    private:
        // Private fields

        /*
         * The render target stack, used for pushing and popping
         */
        static std::vector<TRenderTarget*> _TargetStack;

    public:
        // TODO: Shader support
        // TODO: Depth support through batching?

        // Public Methods

        /*
         * Begin a drawing loop
         */
        static void BeginDrawing();

        /*
         * Clear the screen with a color
         */
        static void Clear(TColor color_);

        /*
         * Draw a pixel
         */
        static void DrawPixel(TVector2 position_, TColor color_);

        /*
         * Draw line
         */
        static void DrawLine(TVector2 startPos_, TVector2 endPos_, TColor color_,
                             float thickness_ = 1);

        /*
         * Draw a polygon outline from a point array
         */
        // TODO: Thickness for this too?
        static void DrawLineStrip(std::vector<TVector2> points_, TColor color_);

        /*
         * Draw a line using cubic-bezier curves
         */
        static void DrawLineBezier(TVector2 startPos_, TVector2 endPos_, TColor color_,
                                   float thickness_ = 1);

        /*
         * Draw a color-filled circle
         */
        static void DrawCircle(TVector2 center_, float radius_, TColor color_);

        /*
         * Draw a gradient-filled circle
         */
        static void DrawCircleGradient(TVector2 center_, float radius_, TColor color1_,
                                       TColor color2_);

        /*
         * Draw a circle outline
         */
        static void DrawCircleLines(TVector2 center_, float radius_, TColor color_);

        /*
         * Draw a sector of a circle
         */
        static void DrawCircleSector(TVector2 center_, float radius_, int startAngle_, int endAngle_,
                                     int segments_, TColor color_);

        /*
         * Draw a circle sector outline
         */
        static void DrawCircleSectorLines(TVector2 center_, float radius_, int startAngle_, int endAngle_,
                                          int segments_, TColor color_);

        /*
         * Draw the current FPS (Should be used for debug only...)
         */
        static void DrawFPS(TVector2 position_);

        /*
         * Draw a ring
         */
        static void DrawRing(TVector2 center_, float innerRadius_, float outerRadius_, int startAngle_,
                             int endAngle_, int segments_, TColor color_);

        /*
         * Draw a ring outline
         */
        static void DrawRingLines(TVector2 center_, float innerRadius_, float outerRadius_, int startAngle_,
                                  int endAngle_, int segments_, TColor color_);

        /*
         * Draw a rectangle
         */
        static void DrawRectangle(TVector2 position_, float width_, float height_, TColor color_,
                                  float rotation_ = 0, TVector2 origin_ = TVector2());

        /*
         * Draw a rectangle
         */
        static void DrawRectangle(TVector2 position_, TVector2 size_, TColor color_,
                                  float rotation_ = 0, TVector2 origin_ = TVector2());

        /*
         * Draw a rectangle with additional parameters
         */
        static void DrawRectangle(TRectangle rectangle_, TColor color_, float rotation_ = 0,
                                  TVector2 origin_ = TVector2());

        /*
         * Draw a vertical-gradient-filled rectangle
         */
        static void DrawRectangleGradientV(TVector2 position_, float width_, float height_,
                                           TColor color1_, TColor color2_);

        /*
         * Draw a vertical-gradient-filled rectangle
         */
        static void DrawRectangleGradientV(TVector2 position_, TVector2 size_, TColor color1_,
                                           TColor color2_);

        /*
         * Draw a vertical-gradient-filled rectangle
         */
        static void DrawRectangleGradientV(TRectangle rectangle_, TColor color1_,
                                           TColor color2_);

        /*
         * Draw a horizonal-gradient-filled rectangle
         */
        static void DrawRectangleGradientH(TVector2 position_, float width_, float height_,
                                           TColor color1_, TColor color2_);

        /*
         * Draw a horizonal-gradient-filled rectangle
         */
        static void DrawRectangleGradientH(TVector2 position_, TVector2 size_, TColor color1_,
                                           TColor color2_);

        /*
         * Draw a horizonal-gradient-filled rectangle
         */
        static void DrawRectangleGradientH(TRectangle rectangle_, TColor color1_,
                                           TColor color2_);

        /*
         * Draw a rectangle with a color per vertex
         */
        static void DrawRectangleGradientEx(TVector2 position_, float width_, float height_,
                                            TColor color1_, TColor color2_, TColor color3_,
                                            TColor color4_);

        /*
         * Draw a rectangle with a color per vertex
         */
        static void DrawRectangleGradientEx(TVector2 position_, TVector2 size_, TColor color1_,
                                            TColor color2_, TColor color3_, TColor color4_);

        /*
         * Draw a rectangle with a color per vertex
         */
        static void DrawRectangleGradientEx(TRectangle rectangle_, TColor color1_,
                                            TColor color2_, TColor color3_, TColor color4_);

        /*
         * Draw rectangle outline
         */
        static void DrawRectangleLines(TVector2 position_, float width_, float height_, TColor color_,
                                       int lineThickness_ = 1);

        /*
         * Draw rectangle outline
         */
        static void DrawRectangleLines(TVector2 position_, TVector2 size_, TColor color_,
                                       int lineThickness_ = 1);

        /*
         * Draw rectangle outline
         */
        static void DrawRectangleLines(TRectangle rectangle_, TColor color_, int lineThickness_ = 1);

        /*
         * Draw a rounded rectangle
         */
        static void DrawRectangleRounded(TVector2 position_, float width_, float height_, float roundness_,
                                         int segments_, TColor color_);

        /*
         * Draw a rounded rectangle
         */
        static void DrawRectangleRounded(TVector2 position_, TVector2 size_, float roundness_,
                                         int segments_, TColor color_);

        /*
         * Draw a rounded rectangle
         */
        static void DrawRectangleRounded(TRectangle rectangle_, float roundness_, int segments_,
                                         TColor color_);

        /*
         * Draw a rounded rectangle outline
         */
        static void DrawRectangleRoundedLines(TVector2 position_, float width_, float height_,
                                              float roundness_, int segments_, int lineThickness_,
                                              TColor color_);

        /*
         * Draw a rounded rectangle outline
         */
        static void DrawRectangleRoundedLines(TVector2 position_, TVector2 size_, float roundness_,
                                              int segments_, int lineThickness_, TColor color_);

        /*
         * Draw a rounded rectangle outline
         */
        static void DrawRectangleRoundedLines(TRectangle rectangle_, float roundness_, int segments_,
                                              int lineThickness_, TColor color_);

        /*
         * Draw text
         */
        static void DrawText(const TFont &font_, const std::string &string_, TVector2 position_,
                             float fontSize_, float spacing_, TColor color_);

        /*
         * Draw text with rectangle constraint
         */
        static void DrawTextRect(const TFont &font_, const std::string &string_,
                                 TRectangle rectangle_, float fontSize_, float spacing_,
                                 TColor color_, bool wordWrap_ = true);

        /*
         * Draw text with rectangle constraint and select support
         */
        static void DrawTextRectEx(const TFont &font_, const std::string &string_,
                                   TRectangle rectangle_, float fontSize_, float spacing_,
                                   TColor color_, int selectStart_, int selectLength_,
                                   TColor selectText_, TColor selectBack_, bool wordWrap_ = true);

        /*
         * Draw a texture
         */
        static void DrawTexture(const TTexture2D &texture_, TVector2 position_, TColor color_,
                                float scale_ = 1, TVector2 origin = TVector2(), float rotation_ = 0);

        /*
         * Draw a texture with specified dimensions
         */
        static void DrawTexture(const TTexture2D &texture_, TVector2 position_, float width_,
                                float height_, TColor color_, TVector2 origin_ = TVector2(),
                                float rotation_ = 0);

        /*
         * Draw a part of a texture
         */
        static void DrawTexture(const TTexture2D &texture_, TRectangle sourceRectangle_,
                                TVector2 position_, TColor color_,
                                TVector2 origin_ = TVector2(), float rotation_ = 0);

        /*
         * Draw a part of a texture with specified dimensions
         */
        static void DrawTexture(const TTexture2D &texture_, TRectangle sourceRectangle_,
                                TVector2 position_, float width_, float height_, TColor color_,
                                TVector2 origin_ = TVector2(), float rotation_ = 0);

        /*
         * Draw a texture with pro parameters
         */
        static void DrawTexture(const TTexture2D &texture_, TRectangle destRectangle_,
                                TRectangle sourceRectangle_, TColor color_,
                                TVector2 origin_ = TVector2(), float rotation_ = 0);

        /*
         * Draw a triangle
         */
        static void DrawTriangle(TVector2 v1_, TVector2 v2_, TVector2 v3_,
                                 TColor color_);

        /*
         * Draw a triangle outline
         */
        // TODO: Can we add line thickness into raylib for this???
        static void DrawTriangleLines(TVector2 v1_, TVector2 v2_, TVector2 v3_,
                                      TColor color_);

        /*
         * Draw a triangle fan from a point array
         */
        static void DrawTriangleFan(std::vector<TVector2> points_, TColor color_);

        /*
         * Draw a polygon
         */
        static void DrawPoly(TVector2 center_, int sides_, float radius_, float rotation_,
                             TColor color_);
        /*
         * End a drawing loop.
         */
        static void EndDrawing();

        /*
         * Pop a target off the stack (if present)
         * If not present, returns empty target
         */
        static TRenderTarget *PopTarget(bool &popped_);

        /*
         * Push a target onto the stack
         */
        static void PushTarget(TRenderTarget *target_);

        /*
         * Replace a target on the stack
         */
        static void ReplaceTarget(TRenderTarget *old_, TRenderTarget *new_);
    };
}

#endif //DRAWING_H
