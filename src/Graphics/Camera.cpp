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

#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
#include "OpenGL/OpenGL.hpp"
#endif

#include "Camera.hpp"

namespace NerdThings::Ngine::Graphics {
    // Public Methods

    void Camera::BeginCamera() const {
        // Force draw and load
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        OpenGL::GL::Draw();
        OpenGL::GL::MatrixMode(OpenGL::MATRIX_MODELVIEW);
        OpenGL::GL::LoadIdentity();
        OpenGL::GL::MultMatrix(GetTranslationMatrix());
#endif
    }

    void Camera::EndCamera() const {
        // Force draw and reload
#if defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
        OpenGL::GL::Draw();
        OpenGL::GL::MatrixMode(OpenGL::MATRIX_MODELVIEW);
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
