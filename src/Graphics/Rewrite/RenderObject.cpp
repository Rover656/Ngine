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

#include "RenderObject.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
    RenderObject::Type Rewrite::RenderObject::GetType() const {
        return _Type;
    }

    float Rewrite::RenderObject::GetZIndex() const {
        return _ZIndex;
    }

    void Rewrite::RenderObject::SetZIndex(float zindex_) {
        _ZIndex = zindex_;
    }

    ShaderProgram *RenderObject::GetShaderProgram() {
        return _ShaderProgram;
    }
}