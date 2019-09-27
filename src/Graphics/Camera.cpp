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

#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
#include "OpenGL/OpenGL.h"
#endif

#include "Camera.h"

namespace NerdThings::Ngine::Graphics {
    // Public Methods

    void Camera::BeginCamera() const {
        // Force draw and load
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        OpenGL::GL::Draw();
        OpenGL::GL::LoadIdentity();
        OpenGL::GL::MultMatrix(GetTranslationMatrix());
#endif
    }

    void Camera::EndCamera() const {
        // Force draw and reload
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        OpenGL::GL::Draw();
        OpenGL::GL::LoadIdentity();
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
