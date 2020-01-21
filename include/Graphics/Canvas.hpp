/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "../Config.hpp"

#include "../Math.hpp"
#include "RenderTexture.hpp"

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
        RenderTexture *m_renderTarget = nullptr;

        /**
         * The graphics device.
         */
        GraphicsDevice *m_graphicsDevice = nullptr;

    public:
        /**
         * Create a new canvas.
         *
         * @param width_ The canvas width.
         * @param height_ The canvas height.
         */
        Canvas(GraphicsDevice *graphicsDevice_, unsigned int width_, unsigned int height_);
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
