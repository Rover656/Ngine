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

#ifndef SHAPERENDERER_HPP
#define SHAPERENDERER_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "../Rectangle.hpp"
#include "Color.hpp"
#include "Renderer.hpp"

namespace ngine::graphics {
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
         * @param renderer Renderer.
         * @param v1 From.
         * @param v2 To.
         * @param color Line color.
         * @param thickness Line thickness.
         */
        static void _drawLine(Renderer *renderer, Vector2 v1, Vector2 v2, Color color, float thickness = 1);

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
         * @param renderer The game renderer.
         * @param v1 Starting line position.
         * @param v2 Ending line position.
         * @param color Line color.
         * @param thickness The line thickness in pixels.
         */
        static void DrawLine(Renderer *renderer, Vector2 v1, Vector2 v2, Color color, float thickness = 1);

        /**
         * Draw a circle
         *
         * @param renderer The game renderer.
         * @param center The center point of the circle.
         * @param radius The circle radius.
         * @param color The circle color.
         * @param outline Whether or not this should be an outline.
         * @param lineThickness The thickness of the line (if drawing an outline).
         */
        static void DrawCircle(Renderer *renderer, Vector2 center, float radius, Color color, bool outline = false,
                               float lineThickness = 1);

        /**
         * Draw a rectangle.
         *
         * @param renderer The game renderer.
         * @param position The position (top-left) of the rectangle.
         * @param width The rectangle width.
         * @param height The rectangle height.
         * @param color The rectangle color.
         * @param rotation The rectangle rotation.
         * @param origin The origin of the rectangle (where it rotates from).
         * @param outline Whether or not to draw an outline.
         * @param lineThickness The line thickness for the outline.
         */
        static void
        DrawRectangle(Renderer *renderer, Vector2 position, float width, float height, Color color, Angle rotation = 0,
                      Vector2 origin = Vector2::Zero, bool outline = false, float lineThickness = 1);

        /**
         * Draw a rectangle.
         *
         * @param renderer The game renderer.
         * @param rect The rectangle to draw.
         * @param color The rectangle color.
         * @param rotation The rectangle rotation.
         * @param origin The origin of the rectangle (where it rotates from).
         * @param outline Whether or not to draw an outline.
         * @param lineThickness The line thickness for the outline.
         */
        static void DrawRectangle(Renderer *renderer, Rectangle rect, Color color, Angle rotation = 0,
                                  Vector2 origin = Vector2::Zero, bool outline = false, float lineThickness = 1);

        /**
         * Draw a rectangle.
         *
         * @param renderer The game renderer.
         * @param v1 The first point (the peak).
         * @param v2 The second point (the right base).
         * @param v3 The third point (the left base).
         * @param color The color of the triangle.
         * @param outline Whether or not to draw an outline.
         * @param lineThickness The line thickness for the outline.
         */
        static void
        DrawTriangle(Renderer *renderer, Vector2 v1, Vector2 v2, Vector2 v3, Color color, Angle rotation = 0,
                     Vector2 origin = Vector2::Zero, bool outline = false, float lineThickness = 1);

        // TODO: Draw polygon once shape classes are final
        // TODO: Drawing circle sectors etc.
    };
}

#endif //SHAPERENDERER_HPP
