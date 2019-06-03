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

#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include "../ngine.h"

#include "../Math/Rectangle.h"
#include "../Math/Vector2.h"
#include "../Physics/BoundingBox.h"
#include "../Physics/Circle.h"
#include "../Physics/Polygon.h"
#include "Color.h"
#include "Font.h"
#include "RenderTarget.h"
#include "Texture2D.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * Sprite batch item type
     */
    enum SpriteBatchItemType {
        ITEM_PIXEL,
        ITEM_LINE,
        ITEM_LINE_STRIP,
        ITEM_LINE_BEZIER,
        ITEM_CIRCLE,
        ITEM_CIRCLE_GRADIENT,
        ITEM_CIRCLE_LINES,
        ITEM_CIRCLE_SECTOR,
        ITEM_CIRCLE_SECTOR_LINES,
        ITEM_FPS,
        ITEM_RING,
        ITEM_RING_LINES,
        ITEM_RECTANGLE,
        ITEM_RECTANGLE_GRADIENT_V,
        ITEM_RECTANGLE_GRADIENT_H,
        ITEM_RECTANGLE_GRADIENT_EX,
        ITEM_RECTANGLE_LINES,
        ITEM_RECTANGLE_ROUNDED,
        ITEM_RECTANGLE_ROUNDED_LINES,
        ITEM_TEXT,
        ITEM_TEXT_RECT,
        ITEM_TEXT_RECT_EX,
        ITEM_TEXTURE,
        ITEM_TRIANGLE,
        ITEM_TRIANGLE_LINES,
        ITEM_TRIANGLE_FAN,
        ITEM_POLY
    };

    /*
     * Sprite batch item
     */
    struct SpriteBatchItem {
        // Public Fields

        /*
         * Bool values
         */
        std::vector<bool> Bools;

        /*
         * Colors
         */
        std::vector<TColor> Colors;

        /*
         * Float values
         */
        std::vector<float> Floats;

        /*
         * Font
         */
        TFont *Font;

        /*
         * Int values
         */
        std::vector<int> Ints;

        /*
         * String
         */
        std::string String;

        /*
         * Texture
         */
        TTexture2D *Texture;

        /*
         * Item type
         */
        SpriteBatchItemType Type;
    };

    /*
     * Sprite batcher
     */
    class NEAPI SpriteBatch {
        // Private Fields

        /*
         * The current batch
         */
        std::map<int, std::vector<SpriteBatchItem>> _Batch;

        /*
         * Whether or not we are already drawing
         */
        bool _Drawing = false;
    public:

        // Public Constructor(s)

        /*
         * Create a new sprite batch
         */
        SpriteBatch();

        // Destructor

        virtual ~SpriteBatch() = default;

        // Public Methods

        /*
         * Begin drawing
         */
        void Begin();

        /*
         * Draw circle
         */
        void DrawCircle(Physics::TCircle circle_, TColor color_, int depth_ = 0);

        /*
         * Draw circle
         */
        void DrawCircle(Math::TVector2 center_, float radius_, TColor color_, int depth_ = 0);

        /*
         * Draw circle gradient
         */
        void DrawCircleGradient(Physics::TCircle circle_, TColor color1_, TColor color2_, int depth_ = 0);

        /*
         * Draw circle gradient
         */
        void DrawCircleGradient(Math::TVector2 center_, float radius_, TColor color1_, TColor color2_, int depth_ = 0);

        /*
         * Draw circle outline
         */
        void DrawCircleLines(Physics::TCircle circle_, TColor color_, int depth_ = 0);

        /*
         * Draw circle outline
         */
        void DrawCircleLines(Math::TVector2 center_, float radius_, TColor color_, int depth_ = 0);

        /*
         * Draw game fps
         */
        void DrawFPS(float x_, float y_, int depth_ = 0);

        /*
         * Draw game fps
         */
        void DrawFPS(Math::TVector2 position_, int depth_ = 0);

        /*
         * Draw line
         */
        void DrawLine(Math::TVector2 pos1_, Math::TVector2 pos2_, TColor col_, float thickness_ = 1, int depth_ = 0);

        /*
         * Draw line bezier
         */
        void DrawLineBezier(Math::TVector2 pos1_, Math::TVector2 pos2_, TColor color_, float thickness_ = 1,
            int depth_ = 0);

        /*
         * Draw line strip
         */
        void DrawLineStrip(std::vector<Math::TVector2> points_, TColor color_, int depth_ = 0);

        /*
         * Draw a pixel to the screen
         */
        void DrawPixel(float x_, float y_, TColor color_, int depth_ = 0);

        /*
         * Draw a pixel to the screen
         */
        void DrawPixel(Math::TVector2 position_, TColor color_, int depth_ = 0);

        /*
         * Draw polygon
         */
        void DrawPoly(Math::TVector2 center_, int sides_, float radius_, float rotation_, TColor color_,
                      int depth_ = 0);

        /*
         * Draw rectangle
         */
        void DrawRectangle(Math::TVector2 position_, Math::TVector2 size_, TColor color_, float rotation_ = 0,
                           Math::TVector2 origin_ = Math::TVector2::Zero, int depth_ = 0);

        /*
         * Draw rectangle
         */
        void DrawRectangle(Math::TRectangle rectangle_, TColor color_, float rotation_ = 0,
                           Math::TVector2 origin_ = Math::TVector2::Zero, int depth_ = 0);

        /*
         * Draw rectangle with vertical gradient
         */
        void DrawRectangleGradientV(Math::TRectangle rectangle_, TColor color1_, TColor color2_, int depth_ = 0);

        /*
         * Draw rectangle with horizontal gradient
         */
        void DrawRectangleGradientH(Math::TRectangle rectangle_, TColor color1_, TColor color2_, int depth_ = 0);

        /*
         * Draw rectangle with custom gradient
         */
        void DrawRectangleGradientEx(Math::TRectangle rectangle_, TColor color1_, TColor color2_, TColor color3_,
                                     TColor color4_, int depth_ = 0);

        /*
         * Draw rectangle outline
         */
        void DrawRectangleLines(Math::TVector2 position_, Math::TVector2 size_, TColor color_, int lineThickness_ = 1,
                                int depth_ = 0);

        /*
         * Draw rectangle outline
         */
        void DrawRectangleLines(Math::TRectangle rectangle_, TColor color_, int lineThickness_ = 1, int depth_ = 0);

        /*
         * Draw rounded rectangle
         */
        void DrawRectangleRounded(Math::TVector2 position_, Math::TVector2 size_, float roundness_, int segments_,
                                  TColor color_, int depth_ = 0);

        /*
         * Draw rounded rectangle
         */
        void DrawRectangleRounded(Math::TRectangle rectangle_, float roundness_, int segments_, TColor color_,
                                  int depth_ = 0);

        /*
         * Draw rounded rectangle outline
         */
        void DrawRectangleRoundedLines(Math::TVector2 position_, Math::TVector2 size_, float roundness_, int segments_,
                                       int lineThickness_, TColor color_, int depth_ = 0);

        /*
         * Draw rounded rectangle outline
         */
        void DrawRectangleRoundedLines(Math::TRectangle rectangle_, float roundness_, int segments_, int lineThickness_,
                                       TColor color_, int depth_ = 0);

        /*
         * Draw ring
         */
        void DrawRing(Math::TVector2 center_, float inRadius_, float outRadius_, int startAngle_, int endAngle_,
                      int segments_, TColor color_, int depth_ = 0);

        /*
         * Draw ring outline
         */
        void DrawRingLines(Math::TVector2 center_, float inRadius_, float outRadius_, int startAngle_, int endAngle_,
                           int segments_, TColor color_, int depth_ = 0);

        /*
         * Draw text
         */
        void DrawText(TFont *font_, const std::string &string_, Math::TVector2 position_, float fontSize_,
                      float spacing_, TColor color_, int depth_ = 0);

        /*
         * Draw text in rectangle
         */
        void DrawTextRect(TFont *font_, const std::string &string_, Math::TRectangle rectangle_, float fontSize_,
                          float spacing_, TColor color_, bool wordWrap_ = true, int depth_ = 0);

        /*
         * Draw text in rectangle with editing options
         */
        void DrawTextRectEx(TFont *font_, const std::string &string_, Math::TRectangle rectangle_, float fontSize_,
                            float spacing_, TColor color_, int selectStart_, int selectLength_, TColor selectText_,
                            TColor selectBack_, bool wordWrap_ = true, int depth_ = 0);

        /*
         * Draw texture
         */
        void DrawTexture(TTexture2D *texture_, Math::TVector2 position_, TColor color_, float scale_ = 1,
                         float rotation_ = 0, Math::TVector2 origin_ = Math::TVector2::Zero, int depth_ = 0);

        /*
         * Draw texture
         */
        void DrawTexture(TTexture2D *texture_, Math::TRectangle destinationRectangle_, Math::TRectangle sourceRectangle_,
                         TColor color_, float rotation_ = 0, Math::TVector2 origin_ = Math::TVector2::Zero,
                         int depth_ = 0);

        /*
         * Draw triangle
         */
        void DrawTriangle(Math::TVector2 v1_, Math::TVector2 v2_, Math::TVector2 v3_, TColor color_, int depth_ = 0);

        /*
         * Draw triangle outline
         */
        void DrawTriangleLines(Math::TVector2 v1_, Math::TVector2 v2_, Math::TVector2 v3_, TColor color_,
                               int depth_ = 0);

        /*
         * Draw triangle fan
         */
        void DrawTriangleFan(std::vector<Math::TVector2> points_, TColor color_, int depth_ = 0);


        /*
         * End drawing
         */
        void End();
    };
}

#endif //SPRITEBATCH_H
