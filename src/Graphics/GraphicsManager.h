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
        static std::vector<TRenderTarget> _RenderTargetStack;

        // Render Target Related Private Methods

        static void EndRenderTarget();

        static void UseRenderTarget(const TRenderTarget &target_);
    public:
        // Render Target Related Methods

        /*
         * Pop a target off the stack (if present)
         * If not present, returns empty target
         */
        static TRenderTarget PopTarget(bool &popped_);

        /*
         * Push a target onto the stack
         */
        static void PushTarget(const TRenderTarget & target_);

        /*
         const * Replace a t&arget on the stack
         */
        static void ReplaceTarget(const TRenderTarget & old_, const TRenderTarget & new_);
    };
}

#endif //GRAPHICSMANAGER_H
