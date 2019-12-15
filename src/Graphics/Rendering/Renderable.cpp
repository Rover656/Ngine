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

#include "Renderable.hpp"

#ifdef USE_EXPERIMENTAL_RENDERER

namespace NerdThings::Ngine::Graphics::Rendering {
    Renderable::Type Renderable::GetType() const {
        return _Type;
    }

    float Renderable::GetZIndex() const {
        return _ZIndex;
    }

    void Renderable::SetZIndex(float zindex_) {
        _ZIndex = zindex_;
    }

    ShaderProgram *Renderable::GetShaderProgram() {
        return _ShaderProgram;
    }
}

#endif
