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

#include "ngine/graphics/Viewport.hpp"

namespace ngine::graphics {
    Viewport::Viewport(float x, float y, float width, float height) {
        // Set
        update(x, y, width, height);
    }

    void Viewport::update(float x, float y, float width, float height) {
        if (m_x != x || m_y != y || m_width != width || m_height != height) {
            // Set
            m_x = x;
            m_y = y;
            m_width = width;
            m_height = height;

            // Recreate matrices
            m_guiProjection = Matrix::orthographic(x, width, height, y, -1, 1);
            m_screenProjection =
                Matrix::orthographic(x, width, y, height, -1, 1);
            // TODO: World3d... will we ever add this even?
        }
    }

    Matrix Viewport::getProjection(CoordinateSystem system) const {
        switch (system) {
        case CoordinateSystem::Screen:
            return m_screenProjection;
        case CoordinateSystem::GUI:
            return m_guiProjection;
            //            case CoordinateSystem::World3D:
            //                return m_world3DProjection;
        }
        return Matrix::Identity;
    }

    float Viewport::getX() const { return m_x; }

    float Viewport::getY() const { return m_y; }

    float Viewport::getWidth() const { return m_width; }

    float Viewport::getHeight() const { return m_height; }

    bool Viewport::operator==(const Viewport &b) {
        return m_x == b.m_x && m_y == b.m_y && m_width == b.m_width &&
               m_height == b.m_height;
    }

    bool Viewport::operator!=(const Viewport &b) {
        return m_x != b.m_x || m_y != b.m_y || m_width != b.m_width ||
               m_height != b.m_height;
    }
} // namespace ngine::graphics