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

#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "../../Config.hpp"

#ifdef USE_EXPERIMENTAL_RENDERER

#include "../../Vector2.h"
#include "../../Vector3.h"
#include "../Color.h"
#include "../ShaderProgram.h"

namespace NerdThings::Ngine::Graphics::Rendering {
    class Renderer;

    /**
     * Vertex data.
     * `Vector3` for Position, `Vector2` for tex coords and `Color` for color.
     */
    struct VertexData {
        Vector3 Position;
        Color Color;
        Vector2 TexCoords;
    };

    /**
     * This holds information about something that can be rendered to the framebuffer.
     */
    class NEAPI Renderable {
    public:
        /**
         * `Renderable` Type.
         */
        enum Type
        {
            OBJECT_UNKNOWN,
            OBJECT_QUAD,
            OBJECT_PRIMITIVE,
            OBJECT_TRIANGLE
        };
    protected:

        /**
         * The object type.
         */
        Type _Type = OBJECT_UNKNOWN;

        /**
         * Rendering Z index.
         * Controls the order the objects are rendered in.
         */
        float _ZIndex = 0;

        /**
         * Custom shader program.
         *
         * @todo Replace with program state once implemented.
         */
        ShaderProgram *_ShaderProgram = nullptr;

    public:
        /**
         * Get the object type.
         *
         * @return The object type.
         */
        Type GetType() const;

        /**
         * Get the object Z index.
         *
         * @return The Z index.
         */
        float GetZIndex() const;

        /**
         * Set the object Z index.
         *
         * @param zindex_ The new Z index.
         */
        void SetZIndex(float zindex_);

        /**
         * Get the shader program.
         *
         * @warning This is temporary until I implement shader program states.
         * @return The attached shader program.
         */
        ShaderProgram *GetShaderProgram();
    };
}

#endif

#endif //RENDERABLE_HPP
