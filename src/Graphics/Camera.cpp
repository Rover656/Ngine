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

#include "OpenGL/OpenGL.h"
#include "Camera.h"

namespace NerdThings::Ngine::Graphics {
    // Public Methods

    void TCamera::BeginCamera() const {
        // Finish the previous mode
        OpenGL::GL::Draw();
        OpenGL::GL::LoadIdentity();

        auto mat = GetTranslationMatrix();
        OpenGL::GL::MultMatrix(mat);
    }

    void TCamera::EndCamera() const {
        // Finish the previous mode
        OpenGL::GL::Draw();
        OpenGL::GL::LoadIdentity();
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
