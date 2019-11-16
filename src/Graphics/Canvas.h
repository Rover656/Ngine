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

#ifndef CANVAS_H
#define CANVAS_H

#include "../Ngine.h"

#include "../Vector2.h"
#include "RenderTarget.h"

namespace NerdThings::Ngine::Graphics {
    /**
     * A canvas is a render target that is only redrawn when needed.
     */
    class NEAPI Canvas {
        /**
         * Canvas width
         */
        unsigned int _Width;

        /**
         * Canvas height
         */
        unsigned int _Height;

        /**
         * The render target
         */
        RenderTarget *_RenderTarget;

    public:
        /**
         * Create a new canvas.
         *
         * @param width_ The canvas width.
         * @param height_ The canvas height.
         */
        Canvas(unsigned int width_, unsigned int height_);
        virtual ~Canvas();

        /**
         * Draw the canvas.
         *
         * @param pos_ The position to draw the canvas at.
         */
        void Draw(Vector2 pos_);

        /**
         * Redraw the contents of the canvas.
         * This should be called once after creation at least.
         * This will call the protected method, RenderTargetRedraw.
         */
        void ReDraw();

        /**
         * Get canvas width.
         *
         * @return The canvas width.
         */
        unsigned int GetWidth();

        /**
         * Get canvas height.
         *
         * @return The canvas height.
         */
        unsigned int GetHeight();

        /**
         * Set the dimensions of the canvas.
         *
         * @warning Do not do this too often, as this will recreate the render target.
         * @param width_ The new canvas width.
         * @param height_ The new canvas height.
         */
        void SetDimensions(unsigned int width_, unsigned int height_);

    protected:
        /**
         * This handles the rendering to the render target.
         */
        virtual void RenderTargetRedraw() = 0;
    };
}

#endif // CANVAS_H
