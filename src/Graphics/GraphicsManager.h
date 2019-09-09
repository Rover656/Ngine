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

#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "../ngine.h"

#include "RenderTarget.h"

namespace NerdThings::Ngine::Graphics {
    /*
     * Graphics management class
     */
    class NEAPI GraphicsManager {
        // Render Target Related Private Fields

        /*
         * The render target stack
         */
        static std::vector<std::shared_ptr<TRenderTarget>> _RenderTargetStack;

        // General Rendering Fields

        static unsigned int _CurrentHeight;

        static unsigned int _CurrentWidth;

        // Render Target Related Private Methods

        static void EndRenderTarget();

        static void UseRenderTarget(std::shared_ptr<TRenderTarget> target_);
    public:
        // General Rendering Methods

        /*
         * Gets the current render width
         */
        static unsigned int GetCurrentWidth();

        /*
         * Gets the current render height
         */
        static unsigned int GetCurrentHeight();

        /*
         * Loads the default orthographic matrix and viewport.
         */
        static void SetupFramebuffer();

        // Render Target Related Methods

        /*
         * Pop a target off the stack (if present)
         * If not present, returns empty target
         */
        static std::shared_ptr<TRenderTarget> PopTarget(bool &popped_);

        /*
         * Push a target onto the stack
         */
        static void PushTarget(std::shared_ptr<TRenderTarget> target_);

        /*
         const * Replace a t&arget on the stack
         */
        static void ReplaceTarget(std::shared_ptr<TRenderTarget> old_, std::shared_ptr<TRenderTarget> new_);
    };
}

#endif //GRAPHICSMANAGER_H
