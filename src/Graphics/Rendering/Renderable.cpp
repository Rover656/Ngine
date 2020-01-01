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
