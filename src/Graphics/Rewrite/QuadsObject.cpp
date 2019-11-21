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

#include "QuadsObject.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
    QuadsObject::QuadsObject(const std::vector<VertexData> &_vertices) : _Vertices(_vertices) {
        if (_vertices.size() % 4 != 0) throw std::runtime_error("Incorrect vertex count");
        _Type = OBJECT_QUAD;

        _QuadCount = _vertices.size() / 4;
    }

    Texture2D *QuadsObject::GetTexture() const {
        return _Texture;
    }

    const VertexData *QuadsObject::GetVertices() const {
        return _Vertices.data();
    }

    unsigned int QuadsObject::GetQuadCount() const {
        return _QuadCount;
    }

    void QuadsObject::SetQuadCount(unsigned int count_) {
        _QuadCount = count_;
    }

    Matrix QuadsObject::GetModelView() const {
        return _ModelView;
    }
}