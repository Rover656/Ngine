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

#ifndef QUADSOBJECT_H
#define QUADSOBJECT_H

#include "../../Ngine.h"

#include "../../Matrix.h"
#include "../Texture2D.h"
#include "RenderObject.h"

namespace NerdThings::Ngine::Graphics::Rewrite {
    /**
     * A Quad(s) Object.
     * Renders a quad/quads with an attached texture.
     */
    class NEAPI QuadsObject : public RenderObject {
    protected:
        /**
         * The attached texture.
         */
        Texture2D *_Texture = nullptr;

        /**
         * Quad vertex array.
         */
        std::vector<VertexData> _Vertices;

        /**
         * The number of quads to be rendered.
         */
        unsigned int _QuadCount = 0;

        /**
         * The model view matrix.
         */
        Matrix _ModelView = Matrix::Identity;
    public:
        QuadsObject() = default;
        /**
         * Create a new quads object
         */
        QuadsObject(const std::vector<VertexData> &_vertices);
        ~QuadsObject() = default;

        /**
         * Get the quads texture.
         *
         * @return The attached texture.
         */
        Texture2D *GetTexture() const;
        void SetTexture(Texture2D *tex_);

        /**
         * Get the quads vertex data.
         *
         * @return The vertex data.
         */
        const VertexData *GetVertices() const;

        /**
         * Get the quad count.
         *
         * @return The quad count.
         */
        unsigned int GetQuadCount() const;
        void SetQuadCount(unsigned int count_);

        Matrix GetModelView() const;
    };
}

#endif //QUADSOBJECT_H
