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

#include "ngine/graphics/Camera.hpp"

namespace ngine::graphics {
    Vector2 Camera::screenToWorld(Vector2 pos) {
        return pos.transform(getViewMatrix().invert());
    }

    Vector2 Camera::worldToScreen(Vector2 pos) {
        return pos.transform(getViewMatrix());
    }

    Matrix Camera::getViewMatrix() const {
        Matrix ret = Matrix::Identity;
        ret = Matrix::translate(-Position.X, -Position.Y, 0) * ret;
        ret = Matrix::rotateZ(Rotation) *
              ret; // TODO: Test this doesn't break in Screen coords...
        ret = Matrix::scale(Zoom, Zoom, 1) * ret;
        ret = Matrix::translate(Origin.X, Origin.Y, 0) * ret;
        return ret;
    }
} // namespace ngine::graphics
