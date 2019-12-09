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

#ifndef QUADRENDERABLE_H
#define QUADRENDERABLE_H

#include "../../Ngine.h"

#ifdef USE_EXPERIMENTAL_RENDERER

#include "../../Matrix.h"
#include "../Texture2D.h"
#include "Renderable.h"

namespace NerdThings::Ngine::Graphics::Rendering {
    /**
     * A Quad(s) Object.
     * Renders a quad/quads with an attached texture.
     */
    class NEAPI QuadRenderable : public Renderable {
    protected:
        /**
         * The attached texture.
         */
        Texture2D *Texture = nullptr;

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
        QuadRenderable() = default;
        /**
         * Create a new quads object
         */
        QuadRenderable(const std::vector<VertexData> &_vertices);
        ~QuadRenderable() = default;

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

#endif

#endif //QUADRENDERABLE_H
