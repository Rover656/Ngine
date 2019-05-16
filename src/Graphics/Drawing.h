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

#include "../Core/BaseScene.h"
#include "../Core/Scene2D.h"
#include "../ngine.h"

namespace Ngine {
    namespace Graphics {
        /*
         * Connect to the raylib renderer directly
         */
        class NEAPI Drawing {
            // Private Methods

            /*
             * Unset the texture
             */
            static void UnsetTexture();

        public:
            // Public Methods

            /*
             * Enter 2D camera mode
             */
            static void BeginCamera2D(Types::TCamera2D* camera_);

            /*
             * Begin a drawing loop
             */
            static void BeginDrawing();

            /*
             * Clear the screen with a color
             */
            static void Clear(Types::TColor color_);

            /*
             * Draw a pixel
             */
            static void DrawPixel(Types::TVector2 position_, Types::TColor color_);

            /*
             * Draw line
             */
            static void DrawLine(Types::TVector2 startPos_, Types::TVector2 endPos_, Types::TColor color_, float thickness_ = 1);

            /*
             * Draw a polygon outline from a point array
             */
            // TODO: Thickness for this too?
            static void DrawLineStrip(std::vector<Types::TVector2> points_, Types::TColor color_);

            /*
             * Draw a line using cubic-bezier curves
             */
            static void DrawLineBezier(Types::TVector2 startPos_, Types::TVector2 endPos_, Types::TColor color_, float thickness_ = 1);

            /*
             * Draw a color-filled circle
             */
            static void DrawCircle(Types::TVector2 center_, float radius_, Types::TColor color_);

            /*
             * Draw a gradient-filled circle
             */
            static void DrawCircleGradient(Types::TVector2 center_, float radius_, Types::TColor color1_, Types::TColor color2_);

            /*
             * Draw a circle outline
             */
            static void DrawCircleLines(Types::TVector2 center_, float radius_, Types::TColor color_);

            /*
             * Draw a sector of a circle
             */
            static void DrawCircleSector(Types::TVector2 center_, float radius_, int startAngle_, int endAngle_, int segments_, Types::TColor color_);

            /*
             * Draw a circle sector outline
             */
            static void DrawCircleSectorLines(Types::TVector2 center_, float radius_, int startAngle_, int endAngle_, int segments_, Types::TColor color_);

            /*
             * Draw a ring
             */
            static void DrawRing(Types::TVector2 center_, float innerRadius_, float outerRadius_, int startAngle_, int endAngle_, int segments_, Types::TColor color_);

            /*
             * Draw a ring outline
             */
            static void DrawRingLines(Types::TVector2 center_, float innerRadius_, float outerRadius_, int startAngle_, int endAngle_, int segments_, Types::TColor color_);

            /*
             * Draw a rectangle
             */
            static void DrawRectangle(Types::TVector2 position_, float width_, float height_, Types::TColor color_, float rotation_ = 0, Types::TVector2 origin_ = Types::TVector2());

            /*
             * Draw a rectangle
             */
            static void DrawRectangle(Types::TVector2 position_, Types::TVector2 size_, Types::TColor color_, float rotation_ = 0, Types::TVector2 origin_ = Types::TVector2());

            /*
             * Draw a rectangle with additional parameters
             */
            static void DrawRectangle(Types::TRectangle rectangle_, Types::TColor color_, float rotation_ = 0, Types::TVector2 origin_ = Types::TVector2());

            /*
             * Draw a vertical-gradient-filled rectangle
             */
            static void DrawRectangleGradientV(Types::TVector2 position_, float width_, float height_, Types::TColor color1_, Types::TColor color2_);

            /*
             * Draw a vertical-gradient-filled rectangle
             */
            static void DrawRectangleGradientV(Types::TVector2 position_, Types::TVector2 size_, Types::TColor color1_, Types::TColor color2_);

            /*
             * Draw a vertical-gradient-filled rectangle
             */
            static void DrawRectangleGradientV(Types::TRectangle rectangle_, Types::TColor color1_, Types::TColor color2_);

            /*
             * Draw a horizonal-gradient-filled rectangle
             */
            static void DrawRectangleGradientH(Types::TVector2 position_, float width_, float height_, Types::TColor color1_, Types::TColor color2_);

            /*
             * Draw a horizonal-gradient-filled rectangle
             */
            static void DrawRectangleGradientH(Types::TVector2 position_, Types::TVector2 size_, Types::TColor color1_, Types::TColor color2_);

            /*
             * Draw a horizonal-gradient-filled rectangle
             */
            static void DrawRectangleGradientH(Types::TRectangle rectangle_, Types::TColor color1_, Types::TColor color2_);

            /*
             * Draw a rectangle with a color per vertex
             */
            static void DrawRectangleGradientEx(Types::TVector2 position_, float width_, float height_, Types::TColor color1_, Types::TColor color2_, Types::TColor color3_, Types::TColor color4_);

            /*
             * Draw a rectangle with a color per vertex
             */
            static void DrawRectangleGradientEx(Types::TVector2 position_, Types::TVector2 size_, Types::TColor color1_, Types::TColor color2_, Types::TColor color3_, Types::TColor color4_);

            /*
             * Draw a rectangle with a color per vertex
             */
            static void DrawRectangleGradientEx(Types::TRectangle rectangle_, Types::TColor color1_, Types::TColor color2_, Types::TColor color3_, Types::TColor color4_);

            /*
             * Draw rectangle outline
             */
            static void DrawRectangleLines(Types::TVector2 position_, float width_, float height_, Types::TColor color_, int lineThickness_ = 1);

            /*
             * Draw rectangle outline
             */
            static void DrawRectangleLines(Types::TVector2 position_, Types::TVector2 size_, Types::TColor color_, int lineThickness_ = 1);

            /*
             * Draw rectangle outline
             */
            static void DrawRectangleLines(Types::TRectangle rectangle_, Types::TColor color_, int lineThickness_ = 1);

            /*
             * Draw a rounded rectangle
             */
            static void DrawRectangleRounded(Types::TVector2 position_, float width_, float height_, float roundness_, int segments_, Types::TColor color_);

            /*
             * Draw a rounded rectangle
             */
            static void DrawRectangleRounded(Types::TVector2 position_, Types::TVector2 size_, float roundness_, int segments_, Types::TColor color_);

            /*
             * Draw a rounded rectangle
             */
            static void DrawRectangleRounded(Types::TRectangle rectangle_, float roundness_, int segments_, Types::TColor color_);

            /*
             * Draw a rounded rectangle outline
             */
            static void DrawRectangleRoundedLines(Types::TVector2 position_, float width_, float height_, float roundness_, int segments_, int lineThickness_, Types::TColor color_);

            /*
             * Draw a rounded rectangle outline
             */
            static void DrawRectangleRoundedLines(Types::TVector2 position_, Types::TVector2 size_, float roundness_, int segments_, int lineThickness_, Types::TColor color_);

            /*
             * Draw a rounded rectangle outline
             */
            static void DrawRectangleRoundedLines(Types::TRectangle rectangle_, float roundness_, int segments_, int lineThickness_, Types::TColor color_);

            /*
             * Draw a triangle
             */
            static void DrawTriangle(Types::TVector2 v1_, Types::TVector2 v2_, Types::TVector2 v3_, Types::TColor color_);

            /*
             * Draw a triangle outline
             */
            // TODO: Can we add line thickness into raylib for this???
            static void DrawTriangleLines(Types::TVector2 v1_, Types::TVector2 v2_, Types::TVector2 v3_, Types::TColor color_);

            /*
             * Draw a triangle fan from a point array
             */
            static void DrawTriangleFan(std::vector<Types::TVector2> points_, Types::TColor color_);

            /*
             * Draw a polygon
             */
            static void DrawPoly(Types::TVector2 center_, int sides_, float radius_, float rotation_, Types::TColor color_);

            // TODO: Textures and text drawing

            /*
             * Finish using 2D camera
             */
            static void EndCamera2D();

            /*
             * End a drawing loop.
             */
            static void EndDrawing();
        };
    }
}

#endif //DRAWING_H