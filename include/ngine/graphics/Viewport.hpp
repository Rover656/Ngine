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

#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "../Config.hpp"

#include "../Math.hpp"

namespace ngine::graphics {
    /**
     * Defines a coordinate system.
     */
    enum class CoordinateSystem {
        /**
         * Screen coordinates go from (0, 0) to (screenWidth, screenHeight).
         * With (0, 0) in the bottom left.
         *
         * All draw calls with origin (0, 0) will assume position as bottom
         * left.
         */
        Screen,

        /**
         * GUI coordinates go from (0, 0) to (screenWidth, screenHeight). With
         * (0, 0) in the top left.
         *
         * All draw calls with origin (0, 0) will assume position as top left.
         */
        GUI,
        //        World3D
    };

    class NEAPI Viewport {
        float m_x = -1;
        float m_y = -1;
        float m_width = -1;
        float m_height = -1;

        Matrix m_screenProjection;
        Matrix m_guiProjection;
        Matrix m_world3DProjection;

    public:
        Viewport(float x = 0, float y = 0, float width = 0, float height = 0);

        void update(float x, float y, float width, float height);

        Matrix getProjection(CoordinateSystem system) const;

        float getX() const;
        float getY() const;
        float getWidth() const;
        float getHeight() const;

        bool operator==(const Viewport &b);
        bool operator!=(const Viewport &b);

        // Convenience methods
        //        Vector2 screenToGUI(Vector2 screen);
        //        Vector3 screenToWorld3D(Vector2 screen);
        //
        //        Vector2 GUIToScreen(Vector2 GUI);
        //        Vector3 GUIToWorld3D(Vector2 GUI);
        //
        //        Vector2 world3DToScreen(Vector3 world);
        //        Vector2 world3DToGUI(Vector3 world);
        //
        //        Vector2 viewportToScreen(Vector2 viewport);
        //        Vector2 viewportToGUI(Vector2 viewport);
        //        Vector3 viewportToWorld3D(Vector2 viewport);
    };
} // namespace ngine::graphics

#endif // VIEWPORT_HPP
