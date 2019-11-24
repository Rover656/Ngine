/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Renderable.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
    Renderable::Type Rewrite::Renderable::GetType() const {
        return _Type;
    }

    float Rewrite::Renderable::GetZIndex() const {
        return _ZIndex;
    }

    void Rewrite::Renderable::SetZIndex(float zindex_) {
        _ZIndex = zindex_;
    }

    ShaderProgram *Renderable::GetShaderProgram() {
        return _ShaderProgram;
    }
}