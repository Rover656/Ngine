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

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Config.hpp"

#include "Math.hpp"

// TODO: Merge into math library.

namespace ngine {
    /*
     * A rectangle
     */
    struct NEAPI Rectangle {
        /*
         * Rectangle X position
         */
        float X;

        /*
         * Rectangle Y position
         */
        float Y;

        /*
         * Rectangle Width
         */
        float Width;

        /*
         * Rectangle Height
         */
        float Height;

        Rectangle() : X(0), Y(0), Width(0), Height(0) {}

        Rectangle(float x, float y, float width, float height)
            : X(x), Y(y), Width(width), Height(height) {}

        Rectangle(Vector2 position, float width, float height)
            : X(position.X), Y(position.Y), Width(width), Height(height) {}

        Rectangle(Vector2 position, Vector2 size)
            : X(position.X), Y(position.Y), Width(size.X), Height(size.Y) {}

        /*
         * Whether or not the rectangle contains the point
         */
        bool contains(Vector2 point) const;
    };
} // namespace ngine

#endif // RECTANGLE_HPP
