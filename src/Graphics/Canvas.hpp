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

#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "RenderTarget.hpp"

namespace NerdThings::Ngine::Graphics {
    /**
     * A canvas is a render target that is only redrawn when needed.
     * @deprecated This will be removed before Ngine V1.0 RC 1. Manage a render target yourself instead.
     */
    class NEAPI Canvas {
        /**
         * Canvas width
         */
        unsigned int m_width;

        /**
         * Canvas height
         */
        unsigned int m_height;

        /**
         * The render target
         */
        RenderTarget *m_renderTarget;

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
        void Draw(Graphics::Renderer *renderer_, Vector2 pos_);

        /**
         * Redraw the contents of the canvas.
         * This should be called once after creation at least.
         * This will call the protected method, RenderTargetRedraw.
         *
         * @warning Originally, this could be used in Update. That is no longer the case.
         */
        void ReDraw(Graphics::Renderer *renderer_);

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
        virtual void RenderTargetRedraw(Graphics::Renderer *renderer_) = 0;
    };
}

#endif //CANVAS_HPP
