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

#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
#include "Graphics/OpenGL/OpenGL.hpp"
#endif

#include "Graphics/Camera.hpp"

namespace NerdThings::Ngine::Graphics {
    // Public Methods

    void Camera::BeginCamera(GraphicsDevice *graphicsDevice_) const {
        // Force draw and load
#ifdef USE_EXPERIMENTAL_RENDERER
        graphicsDevice_->LoadModelViewIdentity();
        graphicsDevice_->MultModelView(GetTranslationMatrix());
#else
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        OpenGL::GL::Draw();
        OpenGL::GL::MatrixMode(OpenGL::MATRIX_MODELVIEW);
        OpenGL::GL::LoadIdentity();
        OpenGL::GL::MultMatrix(GetTranslationMatrix());
#endif
#endif
    }

    void Camera::EndCamera(GraphicsDevice *graphicsDevice_) const {
        // Force draw and reload
#ifdef USE_EXPERIMENTAL_RENDERER
        graphicsDevice_->LoadModelViewIdentity();
#else
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        OpenGL::GL::Draw();
        OpenGL::GL::MatrixMode(OpenGL::MATRIX_MODELVIEW);
        OpenGL::GL::LoadIdentity();
#endif
#endif
    }

    Vector2 Camera::ScreenToWorld(Vector2 pos_) {
        return pos_.Transform(GetTranslationMatrix().Invert());
    }

    Vector2 Camera::WorldToScreen(Vector2 pos_) {
        return pos_.Transform(GetTranslationMatrix());
    }

    Matrix Camera::GetTranslationMatrix() const {
        Matrix ret = Matrix::Identity;
        ret = ret * Matrix::Translate(-Position.X, -Position.Y, 0);
        ret = ret * Matrix::RotateZ(Rotation);
        ret = ret * Matrix::Scale(Zoom, Zoom, 1);
        ret = ret * Matrix::Translate(Origin.X, Origin.Y, 0);
        return ret;
    }
}
