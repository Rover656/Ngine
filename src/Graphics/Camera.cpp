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

#if defined(GRAPHICS_OPENGL33) || defined(GRPHICS_OPENGLES2)
#include "OpenGL/OpenGL.h"
#endif

#include "Camera.h"

namespace NerdThings::Ngine::Graphics {
    // Public Methods

    void TCamera::BeginCamera() const {
        // Force draw and load
#if defined(GRAPHICS_OPENGL33) || defined(GRPHICS_OPENGLES2)
        OpenGL::GL::Draw();
        OpenGL::GL::LoadIdentity();
        OpenGL::GL::MultMatrix(GetTranslationMatrix());
#endif
    }

    void TCamera::EndCamera() const {
        // Force draw and reload
#if defined(GRAPHICS_OPENGL33) || defined(GRPHICS_OPENGLES2)
        OpenGL::GL::Draw();
        OpenGL::GL::LoadIdentity();
#endif
    }

    TVector2 TCamera::ScreenToWorld(TVector2 pos_) {
        return pos_.Transform(GetTranslationMatrix().Invert());
    }

    TVector2 TCamera::WorldToScreen(TVector2 pos_) {
        return pos_.Transform(GetTranslationMatrix());
    }

    TMatrix TCamera::GetTranslationMatrix() const {
        TMatrix ret = TMatrix::Identity;
        ret = ret * TMatrix::Translate(-Target.X, -Target.Y, 0);
        ret = ret * TMatrix::RotateZ(Rotation);
        ret = ret * TMatrix::Scale(Zoom, Zoom, 1);
        ret = ret * TMatrix::Translate(Origin.X, Origin.Y, 0);
        return ret;
    }
}
