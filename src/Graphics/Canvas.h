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

#ifndef CANVAS_H
#define CANVAS_H

#include "../ngine.h"

#include "../Math/Vector2.h"
#include "RenderTarget.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * A canvas is a render target that is only redrawn when needed.
     */
    class NEAPI Canvas {
        // Private Fields

        /*
         * Cavas height
         */
        float _Height;

        /*
         * The render target
         */
        TRenderTarget *_RenderTarget;

        /*
         * Canvas width
         */
        float _Width;

    public:
        // Public Constructor(s)

        /*
         * Create a new canvas
         */
        Canvas(float width_, float height_);

        // Destructor

        virtual ~Canvas();

        // Public Methods

        /*
         * Draw the canvas
         */
        void Draw(Math::TVector2 pos_);

        /*
         * Get canvas height
         */
        float GetHeight();

        /*
         * Get canvas width
         */
        float GetWidth();

        /*
         * Redraw the contents of the canvas.
         * This should be called once after creation at least.
         * This will call the protected method, RenderTargetRedraw.
         */
        void ReDraw();

        /*
         * Set the dimensions of the canvas
         */
        void SetDimensions(float width_, float height_);

    protected:

        // Protected Methods

        /*
         * This handles the rendering to the render target.
         */
        virtual void RenderTargetRedraw() = 0;
    };
}

#endif // CANVAS_H
