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

#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "../Ngine.h"

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
        static std::vector<RenderTarget *> _RenderTargetStack;

        // General Rendering Fields

        static unsigned int _CurrentHeight;

        static unsigned int _CurrentWidth;

        // Render Target Related Private Methods

        static void EndRenderTarget();

        static void UseRenderTarget(RenderTarget *target_);
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
        static RenderTarget *PopTarget(bool &popped_);

        /*
         * Push a target onto the stack
         */
        static void PushTarget(RenderTarget *target_);

        /*
         * Replace a target on the stack
         */
        static void ReplaceTarget(RenderTarget *old_, RenderTarget *new_);
    };
}

#endif //GRAPHICSMANAGER_H
