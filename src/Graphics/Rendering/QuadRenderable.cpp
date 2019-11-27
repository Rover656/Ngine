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

#include "QuadRenderable.h"

#ifdef USE_EXPERIMENTAL_RENDERER

namespace NerdThings::Ngine::Graphics::Rendering {
    QuadRenderable::QuadRenderable(const std::vector<VertexData> &_vertices) : _Vertices(_vertices) {
        if (_vertices.size() % 4 != 0) throw std::runtime_error("Incorrect vertex count");
        _Type = OBJECT_QUAD;

        _QuadCount = _vertices.size() / 4;
    }

    Texture2D *QuadRenderable::GetTexture() const {
        return _Texture;
    }

    void QuadRenderable::SetTexture(Texture2D *tex_) {
        _Texture = tex_;
    }

    const VertexData *QuadRenderable::GetVertices() const {
        return _Vertices.data();
    }

    unsigned int QuadRenderable::GetQuadCount() const {
        return _QuadCount;
    }

    void QuadRenderable::SetQuadCount(unsigned int count_) {
        _QuadCount = count_;
    }

    Matrix QuadRenderable::GetModelView() const {
        return _ModelView;
    }
}

#endif
