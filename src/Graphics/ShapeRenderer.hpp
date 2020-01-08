/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#ifndef SHAPERENDERER_HPP
#define SHAPERENDERER_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "Color.hpp"
#include "Renderer.hpp"

namespace NerdThings::Ngine::Graphics {
    /**
     * The shape renderer is a static API that allows shapes do be drawn. Kinda figures.
     *
     * @note This *is* the API that will replace the shape drawing in the old renderer. Best to start using it as soon as to be forward compatible.
     * @warning The old renderer does not support rotation on some shapes. Please check old renderer documentation to see what you could do. All limitations there apply here.
     */
    class NEAPI ShapeRenderer {
        /**
         * Add vertices for a line to the current batch.
         *
         * @param renderer_ Renderer.
         * @param v1_ From.
         * @param v2_ To.
         * @param color_ Line color.
         * @param thickness_ Line thickness.
         */
        static void _drawLine(Renderer *renderer_, Vector2 v1_, Vector2 v2_, Color color_, float thickness_ = 1);

    public:
        /**
         * The number of triangles used to produce a circle.
         *
         * @todo Settle on a value for this (OR add CMake config for it).
         */
        const static int TRIANGLES_PER_CIRCLE = 20;

        /**
         * The number of lines (therefore quads) used to produce a circle outline.
         *
         * @todo Settle on a value for this (OR add CMake config for it).
         */
        const static int LINES_PER_CIRCLE = 25;

        /**
         * Draw a line.
         *
         * @param renderer_ The game renderer.
         * @param v1_ Starting line position.
         * @param v2_ Ending line position.
         * @param color_ Line color.
         * @param thickness_ The line thickness in pixels.
         */
        static void
        DrawLine(Renderer *renderer_, Vector2 v1_, Vector2 v2_, Color color_, float thickness_ = 1);

        /**
         * Draw a circle
         *
         * @param renderer_ The game renderer.
         * @param center_ The center point of the circle.
         * @param radius_ The circle radius.
         * @param color_ The circle color.
         * @param outline_ Whether or not this should be an outline.
         * @param lineThickness_ The thickness of the line (if drawing an outline).
         */
        static void DrawCircle(Renderer *renderer_, Vector2 center_, float radius_, Color color_, bool outline_ = false,
                               float lineThickness_ = 1);

        /**
         * Draw a rectangle.
         *
         * @param renderer_ The game renderer.
         * @param position_ The position (top-left) of the rectangle.
         * @param width_ The rectangle width.
         * @param height_ The rectangle height.
         * @param color_ The rectangle color.
         * @param rotation_ The rectangle rotation.
         * @param origin_ The origin of the rectangle (where it rotates from).
         * @param outline_ Whether or not to draw an outline.
         * @param lineThickness_ The line thickness for the outline.
         */
        static void DrawRectangle(Renderer *renderer_, Vector2 position_, float width_, float height_, Color color_,
                                  Angle rotation_ = 0, Vector2 origin_ = Vector2::Zero, bool outline_ = false,
                                  float lineThickness_ = 1);

        /**
         * Draw a rectangle.
         *
         * @param renderer_ The game renderer.
         * @param rect_ The rectangle to draw.
         * @param color_ The rectangle color.
         * @param rotation_ The rectangle rotation.
         * @param origin_ The origin of the rectangle (where it rotates from).
         * @param outline_ Whether or not to draw an outline.
         * @param lineThickness_ The line thickness for the outline.
         */
        static void DrawRectangle(Renderer *renderer_, Rectangle rect_, Color color_, Angle rotation_ = 0,
                                  Vector2 origin_ = Vector2::Zero, bool outline_ = false, float lineThickness_ = 1);

        /**
         * Draw a rectangle.
         *
         * @param renderer_ The game renderer.
         * @param v1_ The first point (the peak).
         * @param v2_ The second point (the right base).
         * @param v3_ The third point (the left base).
         * @param color_ The color of the triangle.
         * @param outline_ Whether or not to draw an outline.
         * @param lineThickness_ The line thickness for the outline.
         */
        static void DrawTriangle(Renderer *renderer_, Vector2 v1_, Vector2 v2_, Vector2 v3_, Color color_,
                                 Angle rotation_ = 0, Vector2 origin_ = Vector2::Zero, bool outline_ = false,
                                 float lineThickness_ = 1);

        // TODO: Draw polygon once shape classes are final
        // TODO: Drawing circle sectors etc.
    };
}

#endif //SHAPERENDERER_HPP
